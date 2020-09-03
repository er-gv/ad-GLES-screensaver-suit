//
// Created by nathan on 09/04/20.
//

#include "utils/Scene.h"
#include "graphics/Model.h"
#include <vector>

Scene::Scene(): bounderyBox(Bounderies()), camera(){
    camera.set_bearing(glm::vec3(0.0, 0.0, bounderyBox.back));
}

Scene::~Scene(){
    models.clear();
    for(auto m : models)
        delete( m);
}

void Scene::render() {
    glClearColor(.2f, .2f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    light.render(viewMat, projectionMat);
    for(auto model: models){
        model->render(viewMat, projectionMat, light.position());
    }
}

void Scene::setBounderies( Bounderies b){
    bounderyBox =b;
}

void Scene::addModel(Model* const m){
    models.push_back(m);
}
void Scene::setupCamera(double ratio){



    camera.set_eye(glm::vec3(0.0, 0.0, bounderyBox.front+12));
    camera.set_bearing(glm::vec3(0.0, 0.0, bounderyBox.back));
    projectionMat = camera.projection(45, ratio, bounderyBox.front, bounderyBox.back);
    viewMat = camera.lookAt();
}
void Scene::setLights(){
    light.setup();
    light.setColor(glm::vec3(1.0, 1.0, 1.0));
}

void Scene::viewport(int width, int height){
    glViewport(0, 0, width, height);
    //setBounderies(Bounderies(10.0f, -10.0, -w, w, h, -h));
        // Create a new perspective projection matrix. The height will stay the same
        // while the width will vary as per aspect ratio.

    setupCamera((double) width / height);
    //double angle =atan(22.5);
}

void Scene::update() {
    struct timeval tv;
    gettimeofday(&tv, (struct timezone *) NULL);
    long time = (tv.tv_sec * 1000 + tv.tv_usec / 1000)% 10000L;
    light.updatePosition(time);
    for(auto& model : models){
        model->update(time);
    }
}

Bounderies::Bounderies(
        GLfloat front, GLfloat back, GLfloat left, GLfloat right, GLfloat top,  GLfloat bottom):
         front(front), back(back), left(left), right(right), top(top), bottom(bottom){}

Bounderies::Bounderies():
        front(12), back(-12), left(12), right(-12), top(12), bottom(-12){}



