//
// Created by nathan on 15/05/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H
#define ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H


#include "Polyhedron.h"

#include <GLES2/gl2.h>
#include <graphics/Material.h>
#include "../graphics/Matrix.h"


class TestCube : public Polyhedrons::Polyhedron {
public:
    TestCube();
    virtual ~TestCube();

    virtual bool init();

    virtual void update(long time);
    //void change(int width, int height);

    virtual void render(const glm::mat4& view, const glm::mat4& perspective);

    void destroy();
    virtual bool initVertices();
    virtual bool initFaces();
    virtual bool initBuffers();
    virtual bool addMaterials();
    virtual bool initShaders();


private:

    Material* mUniColorMaterial;
    Material* mUniColorVBO;
    glm::mat4 mLightModelMatrix;

    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;
    GLuint mPositionHandle;
    GLuint mColorHandle;
    GLuint mNormalHandle;

    GLuint mPerVertexProgramHandle;
    GLuint mPointProgramHandle;

    glm::vec4 mLightPosInEyeSpace;

    void drawCube(const glm::mat4 &view, const glm::mat4 &perspective);

    bool setupCubeMaterial();

    float radians;

    bool setupCubeIBOMaterial();

    void drawCubeIBO(const glm::mat4 &view, const glm::mat4 &perspective);
};


#endif //ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H
