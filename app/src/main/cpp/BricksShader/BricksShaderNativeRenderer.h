//
// Created by nathan on 30/03/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_BRICKSSHADERNATIVERENDERER_H
#define ANDROIDOPENGLESLESSONSCPP_BRICKSSHADERNATIVERENDERER_H


#include <jni.h>
#include <GLES2/gl2.h>
#include "graphics/GLUtils.h"
#include "graphics/Matrix.h"
//#include "utils/Logger.h"

class BricksShaderNativeRenderer {

public:

    BricksShaderNativeRenderer();

    ~BricksShaderNativeRenderer();

    void create();
    void change(int width, int height);
    void draw();
    void destroy();
    inline static void log(int type, ...);

private:
    void drawCube(const GLfloat brickColor[3], const GLfloat mortarColor[3],
                  const GLfloat brickSize[2], const GLfloat brickPct[2]);
    void drawLight();

    void initAttribsAndUniforms();
    bool setupBuffers();
    static const GLint POSITION_DATA_SIZE = 4;
    static const GLint NORMAL_DATA_SIZE = 3;

    //scene size
    GLsizei mWidth;
    GLsizei mHeight;

    //programHandlers
    GLuint mLightProgram;
    GLuint mBricksProgram;

    //uniform handlers
    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;

    GLuint mBrickColorHandle;
    GLuint mMortarColorHandle;
    GLuint mBrickSizeHandle;
    GLuint mBrickPctHandle;

    //attrib handlers
    GLuint mPositionHandle;
    GLuint mNormalHandle;

    GLint pointMVPMatrixHandle;
    GLint pointPositionHandle;

    //buffer objects
    GLuint vbo, ibo;

    //matricias
    Matrix *mViewMatrix;
    Matrix *mModelMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVMatrix;
    Matrix *mMVPMatrix;
    Matrix *mLightModelMatrix;
    Matrix *mNormalMatrix;

    //light data
    float mLightPosInModelSpace[4];
    float mLightPosInWorldSpace[4];
    float mLightPosInEyeSpace[4];




};

#endif //ANDROIDOPENGLESLESSONSCPP_BRICKSSHADERNATIVERENDERER_H
