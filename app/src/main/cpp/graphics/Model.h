//
// Created by nathan on 20/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_MODEL_H
#define ANDROIDOPENGLESLESSONSCPP_MODEL_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/Camera.h"
//#include "transform.h"

class Model {
protected:
    glm::mat4 initialTransform, activeTransform;
   // Transform t;

public:
    Model();
    Model(const glm::mat4& transform);
    Model(const Model& m);
    virtual ~Model();

    virtual bool init()=0;
    //bool loadModel(char* const path);
    virtual void render(Camera& camera) = 0;
    virtual void destroy()=0;
    virtual void update() = 0; // call this to update the model's state before rendering
    //virtual void postUpdate() = 0; // call this for post rendering update.

    void translate(float dx, float dy, float dz) ;
    void translate(const glm::vec3& delta);

    void  rotate(float angleInDegrees, float rx, float ry, float rz);
    void  rotate(float angleInDegrees, const glm::vec3& axis);

    void scale(float s);
    void scale(const glm::vec3& vs);

    void setTransform(const glm::mat4& t);
    void resetTransform();

    Model& operator=(const Model& m);
};


#endif //ANDROIDOPENGLESLESSONSCPP_MODEL_H
