//
// Created by nathan on 15/05/20.
//

#include "TestCube.h"
#include "../graphics/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <android/log.h>
#include <GLES3/gl3.h>

#define LOG_TAG "TesterCubs"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

static GLint POSITION_DATA_SIZE = 3;

const static glm::vec3 CUBE_VERTICE[] = {
        // Front vertices
        glm::vec3(-1,-1,1), //front bottom left
        glm::vec3(-1,+1,1), //front top left
        glm::vec3(+1,+1,1), //front top right
        glm::vec3(+1,-1,1), //front bottom right

        glm::vec3(-1,-1,-1), //back bottom left
        glm::vec3(-1,+1,-1), //back top left
        glm::vec3(+1,+1,-1), //back top right
        glm::vec3(+1,-1,-1) //back bottom right

};


static const glm::vec3 CUBE_COLORS[] = {
    // R, G, B
    glm::vec3(1.0f, 0.0f, 0.0f), // Front face (red)
    glm::vec3(0.0f, 1.0f, 0.0f), // Right face (green)
    glm::vec3(0.0f, 0.0f, 1.0f), // Back face (blue)
    glm::vec3(1.0f, 1.0f, 0.0f), // Left face (yellow)
    glm::vec3(0.0f, 1.0f, 1.0f), // Top face (cyan)
    glm::vec3(1.0f, 0.0f, 1.0f)  // Bottom face (magenta)
};

static const glm::vec3 CUBE_NORMALS[] = {
    // X, Y, Z
    glm::vec3(0.0f, 0.0f, 1.0f), // Front face
    glm::vec3(1.0f, 0.0f, 0.0f), // Right face
    glm::vec3(0.0f, 0.0f, -1.0f), // Back face
    glm::vec3(-1.0f, 0.0f, 0.0f), // Left face
    glm::vec3(0.0f, 1.0f, 0.0f), // Top face
    glm::vec3(0.0f, -1.0f, 0.0f) // Bottom face
};

TestCube::TestCube() {


    nVertices = 8;
    mLightPosHandle = 0;
    mPositionHandle = 0;
    mColorHandle = 0;
    mNormalHandle = 0;

    mUniColorMaterial = nullptr;
    mUniColorVBO = nullptr;
    //mPerVertexProgramHandle = 0;
    //mPointProgramHandle = 0;

    //mLightPosInModelSpace = glm::vec4(0, 0, 0, 1);
    //mLightPosInWorldSpace = glm::vec4(0);
    //mLightPosInEyeSpace = glm::vec4(0);

    LOGD("Create TestCube instance successful");
}


//bool loadModel(char* const path);
//virtual void postUpdate() = 0; // call this for post rendering update.

bool TestCube::init() {
    LOGD("TestCube create");


    return initBuffers() && initShaders();
}

void TestCube::update(long time) {
    LOGD("TestCube change");
    //float angleInDegrees = (360.0f / 10000.0f) * ((int) time);
    radians = glm::radians((360.0f / 10000.0f) * ((int) time));

}



void TestCube::render(const glm::mat4& view, const glm::mat4& perspective) {
    // Set the OpenGL viewport to same size as the surface.

    // Do a compile rotation every 10 seconds;
    // Set out pre-vertex lighting program.
    //mUniColorMaterial->activate();

    // Set piewrogram handle for cube drawing.
    mMVPMatrixHandle = mUniColorMaterial->getUniform("u_MVPMatrix");
    mMVMatrixHandle = mUniColorMaterial->getUniform("u_MVMatrix");
    mLightPosHandle = mUniColorMaterial->getUniform("u_LightPos");
    mPositionHandle = mUniColorMaterial->getAttrib("a_Position");
    mColorHandle =  mUniColorMaterial->getAttrib("a_Color");
    mNormalHandle = mUniColorMaterial->getAttrib("a_Normal");

    // Calculate mPosition of the light
    // Rotate and then push into the distance.
    //mLightModelMatrix = glm::mat4();


    //mLightPosInEyeSpace =  view* mLightPosInModelSpace;

    // right
    resetTransform();
    translate(4.0f, 0.0f, -7.0f);
    rotate(radians, 1.0f, 0.0f, 0.0f);
    drawCubeIBO(view, perspective);

    // left
    //resetTransform();
    //translate(-4.0f, 0.0f, -7.0f);
    //rotate(radians, 0.0f, 1.0f, 0.0f);
    //drawCubeIBO(view, perspective);

    // top
    resetTransform();
    translate(0.0f, 4.0f, -7.0f);
    rotate(radians, 0.0f, 1.0f, 0.0f);
    drawCubeIBO(view, perspective);

    // bottom
    resetTransform();
    translate(0.0f, -4.0f, -7.0f);
    rotate(radians, 0.0f, 1.0f, 0.0f);
    drawCubeIBO(view, perspective);
    //mUniColorMaterial->deactivate();


    // center
    /*resetTransform();
    translate(0.0f, 0.0f, -5.0f);
    rotate(radians, 1.0f, 1.0f, 1.0f);
    drawCubeIBO(view, perspective);*/

}

void TestCube::destroy() {
}

TestCube::~TestCube() {
    destroy();
}


void TestCube::drawCubeIBO(const glm::mat4& view, const glm::mat4& perspective) {
    //activate the shader
    mUniColorVBO->activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    //bind attribs
    GLuint positionAttr = mUniColorVBO->getAttrib("a_Position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint u_mvp = mUniColorVBO->getUniform("u_MVPMatrix");
    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(perspective *view* activeTransform));

    GLuint u_mv = mUniColorVBO->getUniform("u_mvMat");
    glUniformMatrix4fv(u_mv, 1, GL_FALSE, glm::value_ptr(view* activeTransform));

    GLuint u_LightPos = mUniColorVBO->getUniform("u_LightPos");
    glUniform3fv(u_LightPos, 3, glm::value_ptr(mLightPosInEyeSpace));

    GLuint u_Color = mUniColorVBO->getUniform("u_Color");
    GLuint u_FaceNormal = mUniColorVBO->getUniform("u_FaceNormal");


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    for(unsigned short  i=0; i<6; ++i) {
        glUniform3fv(u_Color, 1, glm::value_ptr(CUBE_COLORS[i]));
        glUniform3fv(u_FaceNormal, 1, glm::value_ptr(CUBE_NORMALS[i]));
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT,
        reinterpret_cast<const void *>(4*i*sizeof(unsigned short)));
    }

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glUniform3f(u_Color, 1.84f, 1.984f, 1.984f);
    for(unsigned short i=0; i<4; ++i) {
        glUniform3fv(u_FaceNormal, 3, glm::value_ptr(CUBE_NORMALS[i]));
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT,
        reinterpret_cast<const void *>(4*i*sizeof(unsigned short)));
    }*/

    //deactivate current shader
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    mUniColorVBO->deactivate();
}




bool TestCube::initVertices(){

    //here we should lode vertices data from a file
    return true;

}

bool TestCube::initFaces(){

    Polyhedron::Polygon** faces;
    nFaces=6;
    return true;
}

bool TestCube::initBuffers(){
    nVbos=1;
    nIbos=2;
    vbo = new GLuint[nVbos];
    ibo = new GLuint[nIbos];
    bool result = false;
    unsigned short constexpr trianglesStripBuffer[] ={0,3, 1,2,
                                                       3,7,2,6,
                                                       7,4,6,5,
                                                       4,0,5,1,//
                                                       1,2,5,6,
                                                       4,7,0,3};


    unsigned short constexpr wireFrameLinesIndexBuffer[] = {
            0,1, 2,3,
            2,3,7,6,
            4, 5, 6, 7,
            0,1,5,4,
            1,2,6,5,
            4,7,3,0
    };


    vertexDataBuffer = new GLfloat[nVertices*POSITION_DATA_SIZE];
    char buffer[1000];
    for(int i=0; i< nVertices; i++){
        //CUBE_NORMALS  CUBE_COLORS
        sprintf(buffer, "spatial[%d] = (%f, %f, %f).", i, CUBE_VERTICE[i].x, CUBE_VERTICE[i].y, CUBE_VERTICE[i].z);
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s",
                            buffer);
        vertexDataBuffer[i*3]   = CUBE_VERTICE[i].x;
        vertexDataBuffer[i*3+1] = CUBE_VERTICE[i].y;
        vertexDataBuffer[i*3+2] = CUBE_VERTICE[i].z;
        sprintf(buffer, "vertexDataBuffer[%d] = (%f, %f, %f ).\n",
                i,
                vertexDataBuffer[i*POSITION_DATA_SIZE],
        vertexDataBuffer[i*3+1],
        vertexDataBuffer[i*3+2]
        );
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s",
                            buffer);

     }
    glGenBuffers(1, vbo);
    glGenBuffers(2, ibo);
    if (vbo[0]>0 && ibo[0]>0 && ibo[1]>0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, nVertices*POSITION_DATA_SIZE*sizeof(GLfloat),  vertexDataBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesStripBuffer),
                     trianglesStripBuffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireFrameLinesIndexBuffer),
                     wireFrameLinesIndexBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        result = true;
    }
    delete vertexDataBuffer;
    return result;
}


bool TestCube::addMaterials(){
    return setupCubeMaterial() && setupCubeIBOMaterial();
}


bool TestCube::setupCubeMaterial(){
    const char vertex_shader[] = "shaders/vertex/per_pixel_vertex_shader_no_tex.glsl";
    const char fragment_shader[] = "shaders/fragment/per_pixel_fragment_shader_no_tex.glsl";
    mUniColorMaterial = Material::makeMaterial(
            vertex_shader, fragment_shader,
            (const char*[]){"a_Position", "a_Color", "a_Normal"}, 3,
            (const char*[]){"u_MVMatrix", "u_MVPMatrix", "u_LightPos"}, 3);
    return (nullptr != mUniColorMaterial);

}

bool TestCube::setupCubeIBOMaterial(){
    const char vertex_shader[] = "shaders/vertex/monochrome_face_vertex.glsl";
    const char fragment_shader[] = "shaders/fragment/monochrome_face_fragment.glsl";
    mUniColorVBO = Material::makeMaterial(
            vertex_shader, fragment_shader,
            nullptr, 0, nullptr, 0);
    return (nullptr != mUniColorVBO);
}

bool TestCube::initShaders(){
    if(addMaterials()){
        //mLightMaterial->
        return (mUniColorMaterial->isInitilized() && mUniColorVBO->isInitilized());
    }
    return false;
}

///=======

//static TestCube *TestCube;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s \n", name, v);
}

static void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
