//
// Created by nathan on 14/10/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_MODELNODE_H
#define AD_GLES_SCREENSAVER_SUIT_MODELNODE_H

#include <map>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/VertexLayout.h>
#include "graphics/Camera.h"
#include "graphics/transform.h"
#include "graphics/Material.h"

class ModelNode {

Material* material;
VertexLayout layout;
GLuint aggregationType;
unsigned nVertices;
std::map<unsigned, std::tuple<unsigned, unsigned, float>> indexGroups;
GLuint vbo;
GLuint* ibo;
public:
    void render(glm::mat4 &viewMat, glm::mat4 &projectionMat, const glm::vec3 &lightPos);
    void transform();
};


#endif //AD_GLES_SCREENSAVER_SUIT_MODELNODE_H
