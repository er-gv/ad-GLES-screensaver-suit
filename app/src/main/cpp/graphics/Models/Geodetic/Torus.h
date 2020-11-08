//
// Created by erez on 02/10/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_TORUS_H
#define AD_GLES_SCREENSAVER_SUIT_TORUS_H

#include "graphics/Models/HardCodedModel.h"
#include <cmath>
#include <string>
#include <map>

#include <android/log.h>

#include "graphics/GLincludes.h"
#include "graphics/Material.h"
#include "graphics/Geometry.h"

class Torus : public HardCodedModel{
public:
    Torus(float inner_rad, float outer_rad, int disks, int sides, int idx=0);
    Torus(const glm::mat4& transform, float inner_rad, float outer_rad, int disks, int sides, int idx=0);
    Torus& operator=(const Torus& m)=delete;
    virtual ~Torus();


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

    void setup(float rad_in, float rad_out, int disks, int sides);
    void renderWireframe();
    void renderSolid();
    float inner_radius, outer_radius;
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
    static inline unsigned int getNvertices(){return 24;}
    static inline unsigned int getNfaces(){return 6;}
    //VertexLayout layout;

    std::map<const char*, Geometry> components;



};


#endif //AD_GLES_SCREENSAVER_SUIT_TORUS_H
