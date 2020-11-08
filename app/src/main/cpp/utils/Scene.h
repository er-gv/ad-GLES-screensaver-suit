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
#include "graphics/Models/Platonic/Polyhedron.h"
#include "graphics/Models/HardCodedModel.h"
#include "graphics/lights/PointLight.h"

class Scene {

    std::vector<HardCodedModel*> models;
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
    void addModel(HardCodedModel* const m);
    void setupCamera(double ratio =1.0);
    void setLights();
    void viewport(int width, int height);

    Bounderies bounderyBox;
};


#endif //ANDROIDOPENGLESLESSONSCPP_SCEANE_H
