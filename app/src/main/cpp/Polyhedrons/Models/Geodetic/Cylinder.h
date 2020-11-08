//
// Created by Erez on 28/08/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_CYLINDER_H
#define AD_GLES_SCREENSAVER_SUIT_CYLINDER_H

#include "graphics/Model.h"



#include <cmath>
#include <string>

#include <android/log.h>

#include "graphics/GLincludes.h"
#include "graphics/Material.h"

class Cylinder : public Model{

    static const int POSITION_DATA_SIZE = 3;
    static const int NORMAL_DATA_SIZE = 3;
    static const int VERTEX_DATA_SIZE = POSITION_DATA_SIZE+NORMAL_DATA_SIZE;

    float m_radius;
    float m_height;
    glm::vec3 m_center;

    unsigned int m_nSides;
    unsigned int m_nRings;

    GLuint vbo;
    GLuint ibo[3];
    glm::vec3 *vertices;
    glm::vec3 *normals;
    int nVertices;



 public:
    Cylinder();
    Cylinder(const glm::vec3& center, float radius, float height, unsigned int nSides, unsigned int nRings);

     virtual ~Cylinder();

     virtual bool init();
     //bool loadModel(char* const path);
     virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) = 0;
     virtual void destroy();
     virtual void update(long time); // call this to update the model's state before rendering
     //virtual void postUpdate() = 0; // call this for post rendering update.
     virtual bool initVertices();
     virtual bool initFaces();
     virtual bool initBuffers();
     virtual bool addMaterials();

     //virtual void postUpdate() = 0; // call this for post rendering update.
     virtual bool initShaders();

};


#endif //AD_GLES_SCREENSAVER_SUIT_CYLINDER_H
