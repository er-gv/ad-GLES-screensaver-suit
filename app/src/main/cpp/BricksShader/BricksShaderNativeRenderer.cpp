//
// Created by nathan on 30/03/20.
//


#include "BricksShaderNativeRenderer.h"
#include <android/log.h>
#include <glm/vec3.hpp>
#define  LOG_TAG    "bricks_shader"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGD(LOG_TAG, "GL %s = %s \n", name, v);
    glm::vec3 viki = glm::vec3(2,4,66);
}

static void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI(LOG_TAG, "after %s() glError (0x%x)\n", op, error);
    }
}


static const GLfloat CUBE_NORMAL_DATA[] = {

       0.0f,  0.0f, +1.0f,
        +1.0f,  0.0f,  0.0f,
        0.0f,  0.0f, -1.0f,
        -1.0f,  0.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f, -1.0f,  0.0f
};

// X, Y, Z
// The normal is used in light calculations and is a vector which points
// orthogonal to the plane of the surface. For a cube model, the normals
// should be orthogonal to the points of each face.

static const GLfloat mMortarColor[][3] {

        {0.359375f, 0.26953125f, 0.05078125f},
        {0.050f, 0.368f, 0.647f},
        {0.988f, 0.870f, 0.909f},
        {0.007f, 0.588f, 0.635f},
        {0.007f, 0.635f, 0.49f}
};

static const GLfloat mBrickColor[][3] {
        {0.952f, 0.6901f, 0.188f},
        {0.75390625f, 0.53828125f, 0.69921875f},

        {0.557f, 0.094f, 0.129f},
        {0.635f, 0.305f, 0.007f},
        {0.788f, 0.968f, 0.0392f}
};

static const GLfloat mBrickSize[][2]{
    {0.3f, 0.15f},
    {0.7, 0.65f},
    {0.3f, 0.15f},
    {0.3f, 0.15f},
    {0.3f, 0.15f}
};

static const GLfloat mBrickPct[][2] {
    {0.9f, 0.85},
    {0.9, 0.85},
    {0.9f, 0.85},
    {0.9f, 0.85},
    {0.9f, 0.85}

};

BricksShaderNativeRenderer::BricksShaderNativeRenderer() {

    mWidth = 0;
    mHeight = 0;

    mViewMatrix = NULL;
    mModelMatrix = NULL;
    mProjectionMatrix = NULL;
    mMVPMatrix = NULL;
    mLightModelMatrix = NULL;

    mLightProgram = 0;
    mBricksProgram = 0;

    //handlers for the vertex shader
    mMVPMatrixHandle = 0;
    mMVMatrixHandle = 0;
    mNormalHandle = 0;
    mPositionHandle = 0;

    mBrickColorHandle = 0;
    mMortarColorHandle = 0;
    mBrickSizeHandle = 0;
    mBrickPctHandle = 0;


    mLightPosHandle = 0;

    //mColorHandle = 0;

    mLightPosInModelSpace[0] = 0.0f;
    mLightPosInModelSpace[1] = 0.0f;
    mLightPosInModelSpace[2] = 0.0f;
    mLightPosInModelSpace[3] = 1.0f;

    mLightPosInWorldSpace[0] = 0.0f;
    mLightPosInWorldSpace[1] = 0.0f;
    mLightPosInWorldSpace[2] = 0.0f;
    mLightPosInWorldSpace[3] = 0.0f;

    mLightPosInEyeSpace[0] = 0.0f;
    mLightPosInEyeSpace[1] = 0.0f;
    mLightPosInEyeSpace[2] = 0.0f;
    mLightPosInEyeSpace[3] = 0.0f;

    LOGD("Create BricksShader instance successful");
}

void BricksShaderNativeRenderer::create() {
    LOGD("BricksShader create");

    // Use culling to remove back face.
    glEnable(GL_CULL_FACE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);



    // Set bricks and mortar program handle
    {
        const char* vertex = GLUtils::openTextFile("shaders/vertex/bricks_vertex_shader.glsl");
        const char* fragment = GLUtils::openTextFile("shaders/fragment/bricks_fragment_shader.glsl");
        mBricksProgram = GLUtils::createProgram(&vertex, &fragment);
        if (!mBricksProgram) {
            LOGD("Could not create program");
            return;
        }
    }

    // Set Point program handle
    {
        const char *vertex = GLUtils::openTextFile("shaders/vertex/point_vertex_shader.glsl");
        const char *fragment = GLUtils::openTextFile("shaders/fragment/point_fragment_shader.glsl");

        // Set program handles
        mLightProgram = GLUtils::createProgram(&vertex, &fragment);
        if (!mLightProgram) {
            LOGD("Could not create program");
            return;
        }
    }

    mModelMatrix = new Matrix();
    mNormalMatrix = new Matrix();
    mMVPMatrix = new Matrix();

    mLightModelMatrix = new Matrix();

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 1.5f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = -5.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    initAttribsAndUniforms();
    setupBuffers();
}

void BricksShaderNativeRenderer::change(int width, int height) {
    LOGD("BricksShader change");

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

void BricksShaderNativeRenderer::draw() {
    // Set the OpenGL viewport to same size as the surface.
    //scean->render();
    glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    // Set out pre-vertex lighting program.
    glUseProgram(mBricksProgram);

    // Calculate mPosition of the light
    // Rotate and then push into the distance.
    mLightModelMatrix->identity();
    mLightModelMatrix->translate(0, 0, -5);
    mLightModelMatrix->rotate(angleInDegrees, 0, 1, 0);
    mLightModelMatrix->translate(0, 0, 2);

    Matrix::multiplyMV(mLightPosInWorldSpace, mLightModelMatrix->mData, mLightPosInModelSpace);
    Matrix::multiplyMV(mLightPosInEyeSpace, mViewMatrix->mData, mLightPosInWorldSpace);

    // right
    mModelMatrix->identity();
    mModelMatrix->translate(4.0f, 0.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 1.0f, 0.0f, 0.0f);

    drawCube(mBrickColor[0], mMortarColor[0],  mBrickSize[0], mBrickPct[0]);

    // left
    mModelMatrix->identity();
    mModelMatrix->translate(-4.0f, 0.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube(mBrickColor[1], mMortarColor[1],  mBrickSize[1], mBrickPct[1]);

    // top
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 4.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube(mBrickColor[2], mMortarColor[2],  mBrickSize[2], mBrickPct[2]);

    // bottom
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, -4.0f, -7.0f);
    mModelMatrix->rotate(angleInDegrees, 0.0f, 1.0f, 0.0f);
    drawCube(mBrickColor[3], mMortarColor[3],  mBrickSize[3], mBrickPct[3]);

    // center
    mModelMatrix->identity();
    mModelMatrix->translate(0.0f, 0.0f, -5.0f);
    mModelMatrix->rotate(angleInDegrees, 1.0f, 1.0f, 1.0f);
    drawCube(mBrickColor[4], mMortarColor[4],  mBrickSize[4], mBrickPct[4]);

    // Draw a point to indicate the light
    glUseProgram(mLightProgram);
    drawLight();
}

void BricksShaderNativeRenderer::destroy() {
    delete mModelMatrix;
    mModelMatrix = NULL;
    delete mViewMatrix;
    mViewMatrix = NULL;
    delete mProjectionMatrix;
    mProjectionMatrix = NULL;
    delete mLightModelMatrix;
    mLightModelMatrix = NULL;
}

BricksShaderNativeRenderer::~BricksShaderNativeRenderer() {
    destroy();
}

void BricksShaderNativeRenderer::drawCube(
        const GLfloat brickColor[3], const GLfloat mortarColor[3],
        const GLfloat brickSize[2], const GLfloat brickPct[2]) {

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(mPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mPositionHandle);

    // This multiplies the view by the model matrix
    // and stores the result the MVP matrix.
    // which currently contains model * view
    mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);

    // Pass in the model view matrix
    glUniformMatrix4fv( mMVMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData );

    // This multiplies the model view matrix by the projection matrix
    // and stores the result in the MVP matrix.
    // which no contains model * view * projection
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    // Pass in the model view projection matrix
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);

    // Pass in the light mPosition in eye space
    glUniform3f(mLightPosHandle,
                mLightPosInEyeSpace[0],
                mLightPosInEyeSpace[1],
                mLightPosInEyeSpace[2]
    );

    glUniform3f(mBrickColorHandle, brickColor[0], brickColor[1], brickColor[2]);
    glUniform3f(mMortarColorHandle, mortarColor[0], mortarColor[1], mortarColor[2]);
    glUniform2f(mBrickSizeHandle, brickSize[0], brickSize[1]);
    glUniform2f( mBrickPctHandle, brickPct[0], brickPct[1]);

    // Draw the cube
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    for(int k=0; k<6; k++) {
        int n = 3*k;
        glUniform3fv(mNormalHandle, 1, &CUBE_NORMAL_DATA[n]);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(4 * k * sizeof(GL_UNSIGNED_INT)));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void BricksShaderNativeRenderer::drawLight() {


    // Pass in the mPosition
    glVertexAttrib3f(
            pointPositionHandle,
            mLightPosInModelSpace[0],
            mLightPosInModelSpace[1],
            mLightPosInModelSpace[2]);

    // Since we are not using a buffer object,
    // disable vertex arrays for the attribute
    glDisableVertexAttribArray(pointPositionHandle);

    // Pass in the transformation matrix.
    mMVPMatrix->identity();
    mMVPMatrix->multiply(*mViewMatrix, *mLightModelMatrix);
    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);

    glUniformMatrix4fv(
            pointMVPMatrixHandle,
            1,
            GL_FALSE,
            mMVPMatrix->mData
    );

    glDrawArrays(GL_POINTS, 0, 1);
}

bool BricksShaderNativeRenderer::setupBuffers() {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    if(0== vbo || 0 == ibo){
        return false;
    }
    GLfloat CUBE_POSITION_DATA[] = {
            -1.0f, +1.0f, +1.0f, 1.0f,  //0
            -1.0f, -1.0f, +1.0f, 1.0f,  //1
            +1.0f, +1.0f, +1.0f, 1.0f, //2
            +1.0f, -1.0f, +1.0f, 1.0f, //3

            -1.0f, +1.0f, -1.0f, 1.0f,  //4
            -1.0f, -1.0f, -1.0f, 1.0f,  //5
            +1.0f, +1.0f, -1.0f, 1.0f, //6
            +1.0f, -1.0f, -1.0f, 1.0f //7
    };


    GLuint indices[]={
            0,1,2,3,
            2,3,6,7,
            6,7,4,5,
            4,5,0,1,
            4,0,6,2,
            7,3,5,1
    };


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_POSITION_DATA), CUBE_POSITION_DATA, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return true;
}

void BricksShaderNativeRenderer::initAttribsAndUniforms() {

    // Set program handle for cube drawing.
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_MVMatrix");
    mLightPosHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_LightPosition");

    mPositionHandle = (GLuint) glGetAttribLocation(mBricksProgram, "MCvertex");
    mNormalHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_MCnormal");


    mBrickColorHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_BrickColor");
    mMortarColorHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_MortarColor");
    mBrickSizeHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_BrickSize");
    mBrickPctHandle = (GLuint) glGetUniformLocation(mBricksProgram, "u_BrickPct");

    pointMVPMatrixHandle = glGetUniformLocation(mLightProgram, "u_MVPMatrix");
    pointPositionHandle = glGetAttribLocation(mLightProgram, "a_Position");

}


///=======

static BricksShaderNativeRenderer *renderer;


extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_bricksShader_BricksShaderNativeRenderer_nativeSurfaceCreate(
        JNIEnv *env,
        jclass type,
        jobject asset_manager) {

    GLUtils::setEnvAndAssetManager(env, asset_manager);
    if (renderer) {
        delete renderer;
        renderer = NULL;
    }

    // Print some OpenGL info
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    renderer = new BricksShaderNativeRenderer();
    renderer->create();

}

extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_bricksShader_BricksShaderNativeRenderer_nativeSurfaceChange(
        JNIEnv *env,
        jclass type,
        jint width, jint height) {

    if (renderer) {
        renderer->change(width, height);
    }

}

extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_bricksShader_BricksShaderNativeRenderer_nativeDrawFrame(
        JNIEnv *env, jclass type) {

    if (renderer) {
        renderer->draw();
    }
}