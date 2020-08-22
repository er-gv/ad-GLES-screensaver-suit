//
// Created by nathan on 09/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_JULIASETRENDERER_H
#define ANDROIDOPENGLESLESSONSCPP_JULIASETRENDERER_H


#include <jni.h>
#include <GLES2/gl2.h>
#include <string>
#include "graphics/GLUtils.h"
#include "graphics/Matrix.h"
#include "utils/Logger.h"
//#include <glm/mat4x4.hpp>


class JuliaSetRenderer {

private:

    Logger *JuliaLogger;
    /** some matrix objects for transformations */
    Matrix* mModelMatrix;
    Matrix* mViewMatrix;
    Matrix* mProjectionMatrix;
    Matrix* mMVPMatrix;
    Matrix* mLightModelMatrix;

    /** the julia program handle accompanied by the relevant attribs and uniforms */
    GLuint mJuliaProgramHandle;
    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mSpectrumTexHandle;
    GLuint mPositionHandle;
    GLuint mLightPosHandle;
    GLuint mColorHandle;
    GLuint mNormalHandle;
    GLuint mTextureCoordinateHandle;
    GLuint mJuliaSeedHandle;

    /** and the two handlers for the point light the julia shader programs */
    /** the light program handle accompanied by the relevant attribs and uniforms */
    GLuint mLightProgramHandle;
    GLuint mLightMVPMatHandle;
    GLint mPointLightPosHandle;
    /** Used to hold a light centered on the origin in model space. We need a 4th coordinate so we can get translations to work when
     *  we multiply this by our transformation matrices. */

    GLfloat mLightPosInWorldSpace[4];
    GLfloat mLightPosInEyeSpace[4];

    static constexpr float sqrt2_over_2 = M_SQRT2/2.0f;
    /** This is a handle to our texture data. */

    //scene dimensions
    GLsizei mWidth;
    GLsizei mHeight;

    GLfloat tick();

    void create();
    bool compileShaders();
    void drawCube(float seed, int idx);
    void drawLight();
/*
 * 0.29296875f, 0.44921875f, good
            0.1953125f, 0.5859375f,*/
public:
    static constexpr char TAG[] = "JuliaSetRenderer";
    JuliaSetRenderer();
    ~JuliaSetRenderer();

    void drawScene();

    void init();
    static void nativeSurfaceCreate();
    void surfaceChange(int width, int height);

    static void printGLString(const char *name, GLenum s) ;

    static void checkGlError(const char *op) ;

private:
    void initAttribsAndUniforms();
    bool setupBuffers();
    GLuint vbo;
    GLuint ibo;

    static GLfloat *getSpectrumParams();
};


#endif //ANDROIDOPENGLESLESSONSCPP_JULIASETRENDERER_H
