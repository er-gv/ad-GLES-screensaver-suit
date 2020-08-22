//
// Created by nathan on 30/03/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_QUANTIZEDCOLORNATIVE_H
#define ANDROIDOPENGLESLESSONSCPP_QUANTIZEDCOLORNATIVE_H



#include <GLES2/gl2.h>
#include "../graphics/Matrix.h"


class QuantizedColorNative {

public:

    QuantizedColorNative();

    ~QuantizedColorNative();

    virtual void create();

    void change(int width, int height);

    virtual void draw();

    void drawTriangle(const GLfloat *verticesData, const GLfloat quanta);

private:


    GLuint mPositionHandle;
    GLuint mColorHandle;
    GLfloat mQuantaHandle;

    GLsizei mWidth;
    GLsizei mHeight;

    Matrix *mLightModelMatrix;

    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;

    GLuint mNormalHandle;

    GLuint mTextureCoordinateHandle;


    GLuint mPointProgramHandle;

    float mLightPosInModelSpace[4];
    float mLightPosInWorldSpace[4];
    float mLightPosInEyeSpace[4];

    Matrix *mViewMatrix;
    Matrix *mModelMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;
    GLuint mProgram;


    static GLfloat *getTriangleData(uint8_t triangleIndex) {
        static constexpr GLfloat triangleVerticesData[][18] = {
                // X, Y, Z,
                // R, G, B
                {//t1
                        -0.5f, -0.25f, 0.0f,
                        1.0f, 1.0f, 1.0f,

                        0.5f, -0.25f, 0.0f,
                        0.5f, 0.5f, 0.5f,

                        0.0f, 0.559016994f, 0.0f,
                        0.0f, 0.0f, 0.0f},

                {//t2
                        // X, Y, Z,
                        // R, G, B
                        -0.5f, -0.25f, 0.0f,
                        1.0f, 0.0f, 0.0f,

                        0.5f, -0.25f, 0.0f,
                        0.0f, 0.0f, 1.0f,

                        0.0f, 0.559016994f, 0.0f,
                        0.0f, 1.0f, 0.0f },
                {
                        // X, Y, Z,
                        // R, G, B, A
                        -0.5f, -0.25f, 0.0f,
                        1.0f, 1.0f, 0.0f,

                        0.5f, -0.25f, 0.0f,
                        0.0f, 1.0f, 1.0f,

                        0.0f, 0.559016994f, 0.0f,
                        1.0f, 0.0f, 1.0f}

        };
        return (GLfloat *) triangleVerticesData[triangleIndex];

    }


};
// This triangle is white, gray, and black.

#endif //ANDROIDOPENGLESLESSONSCPP_QUANTIZEDCOLORNATIVE_H

