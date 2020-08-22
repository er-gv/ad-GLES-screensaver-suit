//
// Created by nathan on 05/04/20.
//

#include "Camera.h"
#include <cstring>
#include <android/log.h>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(): m_eye(glm::vec3()), m_up(glm::vec3(0,1,0)), m_bearing(glm::vec3(0,0, -1)){}


Camera::Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up):
        m_eye(eye), m_bearing(lookat), m_up(up)
{

}

Camera::Camera(GLfloat eye[3], GLfloat lookat[3], GLfloat up[3]):
        m_eye(glm::make_vec3(eye)), m_up(glm::make_vec3(lookat)), m_bearing(glm::make_vec3(up))
{

}

Camera::Camera( GLfloat eyex, GLfloat eyey, GLfloat eyez,
                GLfloat lookatx, GLfloat lookaty, GLfloat lookatz,
                GLfloat upx, GLfloat upy,  GLfloat upz):
        m_eye(glm::vec3(eyex, eyey, eyez)),
        m_up(glm::vec3(upx,upy,upz)),
        m_bearing(glm::vec3(lookatx,lookaty,lookatz))
{

}

Camera::Camera(Camera const & c){
    m_eye=c.m_eye;
    m_bearing=c.m_bearing;
    m_up=c.m_up;
}

Camera::~Camera(){}


glm::vec3& Camera::eye() {
    return m_eye;
}

void Camera::eye(glm::vec3& e){
    m_eye = e;
}

glm::vec3& Camera::bearing() {
    return m_bearing;
}

void Camera::bearing(glm::vec3& b){
    m_bearing = b;
}

glm::vec3& Camera::up() {
    return m_up;
}
void Camera::up(glm::vec3& u){
    m_up = u;
}


glm::mat4 Camera::lookAt(){
    return glm::lookAt(m_eye, m_bearing, m_up);
}
glm::mat4 Camera::lookAtLH(){
    return glm::lookAtLH(m_eye, m_bearing, m_up);
}
glm::mat4 Camera::lookAtRH(){
    return glm::lookAtRH(m_eye, m_bearing, m_up);
}


glm::mat4 Camera::projection(float fov, float ratio, float near, float far) {
    return glm::perspective(fov, ratio, near, far);
}


glm::mat4 Camera::projection() {
    return glm::perspective(45, 1, -1, -10);
}

glm::mat4 Camera::frustum(const Bounderies &b) {
    return glm::frustum(b.left, b.right, b.bottom, b.top, b.front, b.back);
}



