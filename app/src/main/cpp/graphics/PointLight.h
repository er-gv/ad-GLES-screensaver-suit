//
// Created by nathan on 15/05/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_POINTLIGHT_H
#define ANDROIDOPENGLESLESSONSCPP_POINTLIGHT_H

#include "graphics/GLincludes.h"
#include "Material.h"
#include "graphics/transform.h"
#include <glm/vec3.hpp>

class PointLight {


    Transform t;
    glm::vec3 mColor;
    float mSize;
    Material* mat;
public:
    PointLight();
    ~PointLight();

    void render(const glm::mat4& view, const glm::mat4& projection);
    bool setup();

    void setColor(const glm::vec3 &color);

    void setPointSize(const float s);
    void setTransform(Transform& other);
    void updatePosition(long time);
    glm::vec4 position();
};


#endif //ANDROIDOPENGLESLESSONSCPP_POINTLIGHT_H
