//
// Created by nathan on 09/04/20.
//

#include "utils/Scene.h"
#include "graphics/Model.h"
#include <vector>

Scene::Scene(){}
Scene::~Scene(){
    models.clear();
    for(auto m : models)
        delete( m);
}

void Scene::render() {
    glClearColor(.0f, .65f, .65f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for(auto model: models){
        model->render(camera);
    }
}

void Scene::setBounderies( Bounderies b){
    bounderyBox =b;
}

void Scene::addModel(Model* const m){
    models.push_back(m);
}
void Scene::setupCamera(){

}
void Scene::setLights(){

}

void Scene::viewport(int width, int height){
    glViewport(0, 0, bounderyBox.right -bounderyBox.left, bounderyBox.bottom -bounderyBox.top);

        // Create a new perspective projection matrix. The height will stay the same
        // while the width will vary as per aspect ratio.
    float ratio = (float) width / height;

    MVmatrix = camera.projection(45, ratio, bounderyBox.front, bounderyBox.back);
}

Bounderies::Bounderies(
        GLfloat front, GLfloat back, GLfloat left, GLfloat right, GLfloat top,  GLfloat bottom):
         front(front), back(back), left(left), right(right), top(top), bottom(bottom){}

Bounderies::Bounderies():
        front(2), back(-2), left(2), right(-2), top(2), bottom(-2){}



