//
// Created by nathan on 20/04/20.
//

#include "Model.h"

Model::Model(){
    initialTransform = glm::mat4(1.0f);
    activeTransform = glm::mat4(1.0f);
}

Model::Model(const glm::mat4& t){
    initialTransform = activeTransform = t;
}


Model::Model(const Model& m){ *this = m;}

Model::~Model() {

}

void Model::setTransform(const glm::mat4& t){
    activeTransform = t;
}

void Model::resetTransform(){
    activeTransform = initialTransform;
};

Model& Model::operator=(const Model& m){
    initialTransform = m.initialTransform;
    activeTransform = m.activeTransform;
    return *this;
}

void Model::translate(float dx, float dy, float dz) {
    glm::translate(initialTransform, glm::vec3(dx, dy, dz));
}

void Model::translate(const glm::vec3& delta) {
    glm::translate(initialTransform, delta);
}

void  Model::rotate(float angleInDegrees, float rx, float ry, float rz) {
    glm::rotate(activeTransform, angleInDegrees, glm::vec3(rx, ry, rz));
}

void  Model::rotate(float angleInDegrees, const glm::vec3& axis){
    glm::rotate(activeTransform, angleInDegrees, axis);
}

void Model::scale(float s) {
    glm::scale(activeTransform, glm::vec3(s, s, s));
}

void Model::scale(const glm::vec3& vs) {
    glm::scale(activeTransform, vs);
}
