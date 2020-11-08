//
// Created by nathan on 20/04/20.
//

#include "HardCodedModel.h"

HardCodedModel::HardCodedModel(){
    initialTransform = glm::mat4(1.0f);
    activeTransform = glm::mat4(1.0f);
}

HardCodedModel::HardCodedModel(const glm::mat4& t){
    initialTransform = initialTransform = t;
}


HardCodedModel::HardCodedModel(const HardCodedModel& m){ *this = m;}

HardCodedModel::~HardCodedModel() {

}

void HardCodedModel::setTransform(const glm::mat4& t){
    initialTransform.setTransform(t);
}

void HardCodedModel::resetTransform(){
    activeTransform = initialTransform;
};

HardCodedModel& HardCodedModel::operator=(const HardCodedModel& m){
    initialTransform = m.initialTransform;
    activeTransform = m.initialTransform;
    return *this;
}

void HardCodedModel::translate(float dx, float dy, float dz) {
    initialTransform.translate(glm::vec3(dx, dy, dz));
}

void HardCodedModel::translate(const glm::vec3& delta) {
    initialTransform.translate(delta);
}

void  HardCodedModel::rotate(float angleInDegrees, float rx, float ry, float rz) {
    initialTransform.rotate(angleInDegrees, glm::vec3(rx, ry, rz));
}

void  HardCodedModel::rotate(float angleInDegrees, const glm::vec3& axis){
    initialTransform.rotate(angleInDegrees, axis);
}

void HardCodedModel::scale(float s) {
    initialTransform.scale(glm::vec3(s, s, s));
}

void HardCodedModel::scale(const glm::vec3& vs) {
    initialTransform.scale(vs);
}

HardCodedModel *HardCodedModel::loadModel(char *srcObjFile) {
    return nullptr;
}
