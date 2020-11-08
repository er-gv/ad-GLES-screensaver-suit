//
// Created by Erez on 28/08/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_SPHERE_H
#define AD_GLES_SCREENSAVER_SUIT_SPHERE_H

#include "graphics/Models/HardCodedModel.h"
#include <cmath>
#include <string>
#include <map>

#include <android/log.h>

#include "graphics/GLincludes.h"
#include "graphics/Material.h"
#include "graphics/Geometry.h"

class Sphere : public HardCodedModel{
public:
    Sphere(float rad, int disks, int sides, int idx=0);
    Sphere(const glm::mat4& transform, float rad, int disks, int sides, int idx=0);
    Sphere& operator=(const Sphere& m);
    virtual ~Sphere();


    virtual bool init();
    virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);
    virtual void destroy();
    virtual void update(long time);
private:
    typedef std::pair<const char*, Geometry> component;
    virtual bool initVertices();
    virtual bool initFaces();
    virtual bool initBuffers();
    virtual bool addMaterials();

    void setup(float rad, int disks, int sides);

    float radius;
    int nRings, nSides;

    GLuint vbo[3];
    GLuint ibo[3];
    GLuint positionAttributeHandle;
    GLuint normalAttributeHandle;
    //GLuint colorAttributeHandle;

    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;

    GLuint mBrickColorHandle;
    GLuint mMortarColorHandle;
    GLuint mBrickSizeHandle;
    GLuint mBrickPctHandle;

    int m_idx;
    int nVertices;
    static const GLuint POSITION_DATA_SIZE =4;
    static const GLuint NORMAL_DATA_SIZE = 3;
    static const GLuint COLOR_DATA_SIZE =0;
    static const GLuint TEX_DATA_SIZE =0;

    static const GLuint POSITION_OFFSET = 0;
    static const GLuint NORMAL_OFFSET = sizeof(GLfloat)* POSITION_DATA_SIZE;
    static const GLuint COLOR_OFFSET = sizeof(GLfloat)*(POSITION_DATA_SIZE+NORMAL_DATA_SIZE);

    static const GLuint VERTEX_DATA_SIZE = POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE;
    static const GLuint VERTEX_DATA_BYTESIZE = VERTEX_DATA_SIZE*sizeof(GLfloat);

    Material* material;
    glm::vec4* vertices;
    glm::vec3* normals;




};

#endif //AD_GLES_SCREENSAVER_SUIT_SPHERE_H
