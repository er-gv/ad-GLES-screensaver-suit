//
// Created by nathan on 30/03/20.
//

#include "QuantizedColorNative.h"
#include "../graphics/GLUtils.h"
#include <android/log.h>

#define  LOG_TAG    "quantized_color"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s \n", name, v);
}

static void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}


QuantizedColorNative::QuantizedColorNative() {

}

QuantizedColorNative::~QuantizedColorNative()  {

}

void QuantizedColorNative::create() {

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);


    const char *vertex = GLUtils::openTextFile("shaders/vertex/color_vertex_shader.glsl");
    const char *fragment = GLUtils::openTextFile("shaders/fragment/quantized_color_fragment_shader.glsl");

    // Set program handles
    mProgram = GLUtils::createProgram(&vertex, &fragment);
    if (!mProgram) {
        LOGD("Could not create program");
        return;
    }

    mModelMatrix = new Matrix();
    mMVPMatrix = new Matrix();

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 1.5f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void QuantizedColorNative::change(int width, int height) {

    mWidth = width;
    mHeight = height;

    glViewport(0, 0, mWidth, mHeight);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float ratio = (float) width / height;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 1.0f;
    float far = 10.0f;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);
}

void QuantizedColorNative::draw() {
    glClearColor(0.5F, 0.5F, 0.5F, 0.5F);
    glClear(GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(mProgram);

    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
    mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
    mColorHandle = (GLuint) glGetAttribLocation(mProgram, "a_Color");
    mQuantaHandle = (GLuint) glGetUniformLocation(mProgram, "u_Quanta");
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    // Draw the triangle facing straight on.
    mModelMatrix->identity();
    mModelMatrix->rotate(angleInDegrees, 0.0f, 0.0f, 1.0f);
    drawTriangle(QuantizedColorNative::getTriangleData(0), 45.0f);

    // Draw one translated a bit down and rotated to be flat on the ground.
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, -1.0f, 0.0f);
    mModelMatrix->rotate(90.0f, 1.0f, 0.0f, 0.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 0.0f, 1.0f);
    drawTriangle(QuantizedColorNative::getTriangleData(1), 15.0f);

    // Draw one translated a bit to the right and rotated to be facing to the left.
    mModelMatrix->identity();
    mModelMatrix->translate(1.0f, 0.0f, 0.0f);
    mModelMatrix->rotate(90.0f, 0.0f, 1.0f, 0.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 0.0f, 1.0f);
    drawTriangle(QuantizedColorNative::getTriangleData(2), 5.0f);
}

void QuantizedColorNative::drawTriangle(const GLfloat *verticesData, const GLfloat quanta) {

    glVertexAttribPointer(
            (GLuint) mPositionHandle,
            3,
            GL_FLOAT,
            GL_FALSE,
            6 * sizeof(float),
            verticesData
    );
    glEnableVertexAttribArray((GLuint) mPositionHandle);

    glVertexAttribPointer(
            (GLuint) mColorHandle,
            3,
            GL_FLOAT,
            GL_FALSE,
            6 * sizeof(float),
            verticesData + 3
    );
    glEnableVertexAttribArray((GLuint) mColorHandle);
    glUniform1f(mQuantaHandle, quanta);
    // model * view
    mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);

    // model * view * projection
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}


/// =======================================================

static QuantizedColorNative *renderer;

extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_quantizedColor_QuantizedColorNativeRenderer_nativeSurfaceCreate(
        JNIEnv *env,  jclass type, jobject assetManager) {

    GLUtils::setEnvAndAssetManager(env, assetManager);
    if (renderer) {
        delete renderer;
        renderer = NULL;
    }
    renderer = new QuantizedColorNative();
    renderer->create();
}


extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_quantizedColor_QuantizedColorNativeRenderer_nativeSurfaceChange(
        JNIEnv *env, jclass type, jint width, jint height) {
    if (renderer != nullptr) {
        renderer->change(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_quantizedColor_QuantizedColorNativeRenderer_nativeDrawFrame(
        JNIEnv *env,jclass type) {

    if (renderer != nullptr) {
        renderer->draw();
    }
}

