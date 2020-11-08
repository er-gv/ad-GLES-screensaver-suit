//
// Created by erez on 14/10/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_MODEL_H
#define AD_GLES_SCREENSAVER_SUIT_MODEL_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/Camera.h"
#include "graphics/transform.h"


class Model {

public:
    virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);

private:
   std::vector<Node> nodes;
    //vertices;
    //indices;
};


#endif //AD_GLES_SCREENSAVER_SUIT_MODEL_H
