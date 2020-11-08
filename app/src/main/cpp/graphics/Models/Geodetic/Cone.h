//
// Created by nathan on 28/08/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_CONE_H
#define AD_GLES_SCREENSAVER_SUIT_CONE_H


#include <graphics/Models/HardCodedModel.h>
#include <cmath>
#include <string>
#include <map>

#include <android/log.h>

#include "graphics/GLincludes.h"
#include "graphics/Material.h"
#include "graphics/Geometry.h"
//
// Created by nathan on 28/08/20.
//

class Cone : public HardCodedModel{
public:
    Cone(float rad, float h, int sides, int idx=0);
    Cone(const glm::mat4& transform, float rad, float h, int sides, int idx=0);
    Cone& operator=(const Cone& m);
    virtual ~Cone();


    virtual bool init();
    virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);
    virtual void destroy();
    virtual void update(long time);
private:

    virtual bool initVertices();
    virtual bool initFaces();
    virtual bool initBuffers();
    virtual bool addMaterials();

    void setup(float rad, float h, int sides);
    void renderBody(glm::mat4& viewMat);
    void renderLids(glm::mat4& viewMat);

    float radius, height;
    int nSides;
    bool isOpen = false; //whatever the Cone is rendered as an open tube or closed both ends.
    GLuint vbo[1];
    GLuint ibo[2];
    GLuint positionAttributeHandle;
    //GLuint normalAttributeHandle;
    //GLuint colorAttributeHandle;

    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mLightPosHandle;

    GLuint mBrickColorHandle;
    GLuint mMortarColorHandle;
    GLuint mBrickSizeHandle;
    GLuint mBrickPctHandle;

    GLuint mNormalHandle;

    int m_idx;
    static const GLuint POSITION_DATA_SIZE =4;
    static const GLuint NORMAL_DATA_SIZE = 0;
    static const GLuint COLOR_DATA_SIZE =0;
    static const GLuint TEX_DATA_SIZE =0;

    static const GLuint POSITION_OFFSET = 0;
    static const GLuint NORMAL_OFFSET = sizeof(GLfloat)* POSITION_DATA_SIZE;
    static const GLuint COLOR_OFFSET = sizeof(GLfloat)*(POSITION_DATA_SIZE+NORMAL_DATA_SIZE);

    static const GLuint VERTEX_DATA_SIZE = POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE;
    static const GLuint VERTEX_DATA_BYTESIZE = VERTEX_DATA_SIZE*sizeof(GLfloat);

    Material* material;
    unsigned nVertices;
    std::vector<glm::vec3> faceNormals;

};


#endif //AD_GLES_SCREENSAVER_SUIT_CONE_H
