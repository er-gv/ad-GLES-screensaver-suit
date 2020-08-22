//
// Created by nathan on 05/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_CAMERA_H
#define ANDROIDOPENGLESLESSONSCPP_CAMERA_H


#include "graphics/GLincludes.h"
#include "utils/Bounderies.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Camera {
    glm::vec3 m_eye;
    glm::vec3 m_bearing;
    glm::vec3 m_up;


public:
    Camera();
    Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
    Camera(GLfloat eye[3], GLfloat lookat[3], GLfloat up[3]);
    Camera(GLfloat eyex, GLfloat eyey, GLfloat eyez,
           GLfloat lookatx, GLfloat lookaty, GLfloat lookatz,
           GLfloat upx, GLfloat upy,  GLfloat upz);
    Camera(Camera const & c);

    ~Camera();

    glm::vec3& eye() ;
    void eye(glm::vec3& e);
    glm::vec3& bearing() ;
    void bearing(glm::vec3& b);
    glm::vec3& up();
    void up(glm::vec3& u);

    glm::mat4 lookAt();
    glm::mat4 lookAtLH();
    glm::mat4 lookAtRH();

    glm::mat4 projection(float fov, float ratio, float near, float far);

    glm::mat4 projection();
    glm::mat4 frustum(const Bounderies& b);

};


#endif //ANDROIDOPENGLESLESSONSCPP_CAMERA_H
