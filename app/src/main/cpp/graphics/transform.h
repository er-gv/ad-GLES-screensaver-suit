//
// Created by nathan on 15/05/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_TRANSFORM_H
#define ANDROIDOPENGLESLESSONSCPP_TRANSFORM_H

#include "graphics/GLincludes.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform{
    glm::mat4 t;

    inline Transform():t(glm::mat4(1)){}
    inline Transform(glm::mat4 m):t(m){}
    inline ~Transform(){}

    inline void translate(float dx, float dy, float dz) {
        t = glm::translate(t, glm::vec3(dx, dy, dz));
    }

    inline void translate(const glm::vec3& delta){
        t = glm::translate(t, delta);
    }

    inline void  rotate(float angleInRadians, float rx, float ry, float rz){
        t = glm::rotate(t, angleInRadians, glm::vec3(rx,ry,rz));
    }

    inline void  rotate(float angleInRadians, const glm::vec3& axis){
        t = glm::rotate(t, angleInRadians, axis);
    }

    inline void scale(float s){
        t = glm::scale(t, glm::vec3(s,s,s));
    }
    inline void scale(const glm::vec3& vs){
        t = glm::scale(t, vs);
    }

    inline void setTransform(const glm::mat4& m){
        t=m;
    }
    inline void multiply(const glm::mat4& m){
        t*=m;
    }
    inline void concat(const glm::mat4& m){
        t*=m;
    }
    inline void identity(){
        t= glm::mat4(1.0f);
    }
    inline void zero(){
        t= glm::mat4(0.0f);
    }

    inline glm::mat4& get(){
        return t;
    }

};
#endif //ANDROIDOPENGLESLESSONSCPP_TRANSFORM_H
