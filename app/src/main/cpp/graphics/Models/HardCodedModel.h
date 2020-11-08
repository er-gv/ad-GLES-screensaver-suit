//
// Created by nathan on 20/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_MODEL_H
#define ANDROIDOPENGLESLESSONSCPP_MODEL_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/Camera.h"
#include "graphics/transform.h"

class HardCodedModel {
protected:
    Transform initialTransform, activeTransform;
    GLint mMVPMatrixHandle;
    GLint mMVMatrixHandle;


public:
    HardCodedModel();
    HardCodedModel(const glm::mat4& transform);
    HardCodedModel(const HardCodedModel& m);
    virtual ~HardCodedModel();

    virtual bool init()=0;
    //bool loadModel(char* const path);
    virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) = 0;
    //virtual void render(const Camera& cam, const Lighting& lights) = 0;
    virtual void destroy()=0;
    virtual void update(long time) = 0; // call this to update the model's state before rendering
    //virtual void postUpdate() = 0; // call this for post rendering update.

    void translate(float dx, float dy, float dz) ;
    void translate(const glm::vec3& delta);

    void  rotate(float angleInDegrees, float rx, float ry, float rz);
    void  rotate(float angleInDegrees, const glm::vec3& axis);

    void scale(float s);
    void scale(const glm::vec3& vs);

    void setTransform(const glm::mat4& t);
    void resetTransform();

    HardCodedModel& operator=(const HardCodedModel& m);

    static HardCodedModel* loadModel(char* srcObjFile);
};


#endif //ANDROIDOPENGLESLESSONSCPP_MODEL_H
