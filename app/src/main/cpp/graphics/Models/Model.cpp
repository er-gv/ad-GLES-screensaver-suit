//
// Created by nathan on 14/10/20.
//

#include "Model.h"

void Model::render(glm::mat4 &viewMat, glm::mat4 &projectionMat, const glm::vec3 &lightPos) {
    for (auto& node : nodes){
        node.transform();
        node.render(viewMat, projectionMat, lightPos);
    }

}
