//
// Created by nathan on 15/05/20.
//

#include "PointLight.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <android/log.h>
PointLight::PointLight() :t(){
    mSize = 5.0f;
    mat =nullptr;
}


PointLight::~PointLight(){

    if(nullptr != mat){
        delete mat;
    }

}


bool PointLight::setup(){
    const char vertex_shader[] = "shaders/lightSource/pointLight_vertex.glsl";
    const char fragment_shader[] = "shaders/lightSource/pointlight_fragment.glsl";
    mat = Material::makeMaterial(
            vertex_shader, fragment_shader);
    __android_log_print(ANDROID_LOG_INFO, "PointLight", "done setup light material.");
    return (nullptr != mat && mat->isInitilized());
}

void PointLight::setColor(const glm::vec3& color){
    mColor = glm::vec3(color);
}

void PointLight::setPointSize(const float s){
    mSize = s;
}

void PointLight::updatePosition(long time){
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);
    float radians = glm::radians(angleInDegrees);
    t.identity();
    t.translate(glm::vec3(0, 2.0, -7));
    t.rotate(radians, glm::vec3(0.0f, 0.0f, 1.0f));
}

void PointLight::render(const glm::mat4& view, const glm::mat4& projection) {

    __android_log_print(ANDROID_LOG_INFO, "POINTLIGHT", "entering render");
    mat->activate();//glUseProgram(mPointProgramHandle);
    // Pass in the mPosition
    glVertexAttrib3fv(mat->getAttrib("a_Position"),  glm::value_ptr(position()));


    // Pass in the model*view*projection matrix.
    glUniformMatrix4fv(
            mat->getUniform("u_MVPMatrix"),
            1, GL_FALSE,
            glm::value_ptr(projection*view*t.get())
    );
    //Pass in light color
    glUniform3fv(mat->getUniform("u_Color"), 1, glm::value_ptr(mColor));

    //Pass light size
    glUniform1f( mat->getUniform("u_pointSize"), mSize);

    glDrawArrays(GL_POINTS, 0, 1);
    mat->deactivate();
    __android_log_print(ANDROID_LOG_INFO, "POINTLIGHT", "leaving render");
}

glm::vec3 PointLight::position(){
    return glm::vec3(t.get()*glm::vec4(.0f, .0f, .0f, 1.0));
};

void PointLight::setTransform(Transform &other) {
    t = other.get();
}