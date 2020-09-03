//
// Created by nathan on 09/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_SCEANE_H
#define ANDROIDOPENGLESLESSONSCPP_SCEANE_H

#include "graphics/GLUtils.h"
#include "Bounderies.h"
#include <jni.h>
#include <vector>
#include "graphics/Camera.h"
#include "Polyhedrons/Models/Platonic/Polyhedron.h"
#include "graphics/Model.h"
#include "graphics/PointLight.h"

class Scene {

    std::vector<Model*> models;
    Camera camera;
    glm::mat4 projectionMat;
    glm::mat4 viewMat;
    PointLight light;

    //light sources
    //list of modelsmodels;    //camera
    //gl surface interface
public:
    Scene();
    ~Scene();


    void render();
    void update();
    void setBounderies( Bounderies b);
    void addModel(Model* const m);
    void setupCamera(double ratio =1.0);
    void setLights();
    void viewport(int width, int height);

    Bounderies bounderyBox;
};


#endif //ANDROIDOPENGLESLESSONSCPP_SCEANE_H
