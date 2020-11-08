//
// Created by nathan on 15/05/20.
//

#include "TestCube.h"
#include "graphics/PerlinNoiseGenerator.h"
#include <android/log.h>


#include <vector>
#include <tuple>


Polyhedrons::TestCube::TestCube() :
        Polyhedron(), LogTag("TestCube"){
    textureTapestry= nullptr;
    textureBuffer = nullptr;
}

Polyhedrons::TestCube::~TestCube() {
    destroy();
}

bool Polyhedrons::TestCube::init(){

    return initVertices() && initFaces() && initBuffers() && initShaders() && initTexture();
}

void Polyhedrons::TestCube::render(glm::mat4 &viewMat, glm::mat4 &projectionMat,
        const glm::vec3 &lightPos){

    static const GLfloat faceNormals[] ={
             0.0, 0.0, 1.0,   +1.0,  0.0, 0.0,   0.0, 0.0, -1.0
            -1.0, 0.0, 0.0,    0.0, +1.0, 0.0,   0.0, -1.0, 0.0
    };
    textureTapestry->activate();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE_BYTES, reinterpret_cast<const void *>(POS_OFFSET));
    glEnableVertexAttribArray( positionAttributeHandle);

   /* glVertexAttribPointer(normalAttributeHandle, NORMAL_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE_BYTES, reinterpret_cast<const void *>(NORMAL_OFFSET));
    glEnableVertexAttribArray(normalAttributeHandle);*/

    glVertexAttribPointer(texcoordAttributeHandle, TEX_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE_BYTES, reinterpret_cast<const void *>(TEX_OFFSET));
    glEnableVertexAttribArray(texcoordAttributeHandle);


    //draw triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;
    glm::mat4 normalsT = glm::inverse(glm::transpose(activeTransform.get()));
    glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(MVMatrixHandle, 1, false, glm::value_ptr(viewT));
    glUniformMatrix4fv(NormalMatrixHandle, 1, false, glm::value_ptr(normalsT));
    glUniform3fv(lightPositionHandler, 1, glm::value_ptr(lightPos));

    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to this unit
    glBindTexture(GL_TEXTURE_2D, textureDataHandler);
    // Tell the texture uniform sampler to use the texture
    // in the shader by binding to texture unit 0.
    glUniform1i(textureSamplerHandler, 0);

    for(int i=0; i<NUM_FACES; ++i) {
        glUniform3fv(normalHandle, 1, &faceNormals[3*i]);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(4 * i * sizeof(GLuint)));
    }

    glDisableVertexAttribArray(positionAttributeHandle);
    //glDisableVertexAttribArray(normalAttributeHandle);
    glDisableVertexAttribArray(texcoordAttributeHandle);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);
    textureTapestry->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Polyhedrons::TestCube::destroy(){
    if(textureTapestry)
        delete textureTapestry;
    if(textureBuffer)
        delete [] textureBuffer;

}

void Polyhedrons::TestCube::update(long time){

    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());

    activeTransform.translate(glm::vec3(-3.3, 5.75,-0.0));
    activeTransform.rotate(glm::radians(90.0), glm::vec3(0.0, 0.0, 1.0)) ;
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.4, 1.0, -0.8)) ;
    activeTransform.scale(2.75f);
} // call this to update the model's state before rendering
//virtual void postUpdate() = 0; // call this for post rendering update.

bool Polyhedrons::TestCube::initVertices(){
    return true;
}

bool Polyhedrons::TestCube::initFaces(){
    bool result = true;
    return result;
}

bool Polyhedrons::TestCube::initBuffers(){
    const static GLfloat CUBE_VERTICES[] = {
            //apatial-XYZ /normal-XYZ / tex-STV
            // Front face
            -1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, +1.0f, 0.0f, 1.0f,
            +1.0f, +1.0f, +1.0f, 1.0f, 0.0f,
            +1.0f, -1.0f, +1.0f, 1.0f, 1.0f,


            // Right face
            +1.0f, +1.0f, +1.0f, 0.0f, 0.0f,
            +1.0f, -1.0f, +1.0f, 0.0f, 1.0f,
            +1.0f, +1.0f, -1.0f, 1.0f, 0.0f,
            +1.0f, -1.0f, -1.0f, 1.0f, 1.0f,

            // Back face
            +1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
            +1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, +1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,


            // Left face
            -1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, +1.0f, +1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, +1.0f, 1.0f, 1.0f,

            // Top face
            -1.0f, +1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, +1.0f, +1.0f, 0.0f, 1.0f,
            +1.0f, +1.0f, -1.0f, 1.0f, 0.0f,
            +1.0f, +1.0f, +1.0f, 1.0f, 1.0f,

            // Bottom face
            +1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            +1.0f, -1.0f, +1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, +1.0f, 1.0f, 1.0f
    };

    bool result = false;

    GLuint triangleStripIndexBuffer[]= {0,1,2,3,  4,5,6,7,  8,9,10,11,
                                        12,13,14,15,  16,17,18,19,  20,21,22,23};

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    if (m_vbo > 0 && m_ibo > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof( CUBE_VERTICES),  CUBE_VERTICES, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleStripIndexBuffer),
                     triangleStripIndexBuffer, GL_STATIC_DRAW);
        result = true;

    }
    return result;

}

bool Polyhedrons::TestCube::addMaterials(){
    const char vertex_shader[] = "shaders/turbulence/vertex_texture_2D.glsl";
    const char fragment_shader[] = "shaders/sky/fragment_texture_2D.glsl";
    textureTapestry = Material::makeMaterial(vertex_shader, fragment_shader);
    return (textureTapestry != nullptr);

}

bool Polyhedrons::TestCube::initShaders() {
    if(!addMaterials()){
        return false;
    }
    positionAttributeHandle = textureTapestry->getAttrib("a_Position");
    texcoordAttributeHandle = textureTapestry->getAttrib("a_TexCoord");

    MVPMatrixHandle = textureTapestry->getUniform("u_MVPMatrix");
    MVMatrixHandle = textureTapestry->getUniform("u_MVMatrix");
    NormalMatrixHandle = textureTapestry->getUniform("u_NormalMatrix");

    normalHandle = textureTapestry->getUniform("u_Normal");
    lightPositionHandler = textureTapestry->getUniform("u_LightPos");
    textureSamplerHandler = textureTapestry->getUniform("u_Texture");

    return true;

}

bool Polyhedrons::TestCube::initTexture() {
    textureDataHandler = PerlinNoiseGenerator::get2DTexture();

    return true;
}

void Polyhedrons::TestCube::printGLString(const char *msg, GLenum gle) {
    __android_log_print(ANDROID_LOG_INFO, "TestCube", msg, gle);
}

