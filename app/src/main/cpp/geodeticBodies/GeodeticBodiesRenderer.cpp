//
// Created by nathan on 19/09/20.
//

#include "GeodeticBodiesRenderer.h"

#include "utils/Scene.h"

#include <jni.h>
#include <GLES3/gl3.h>
#include <string>
//#include "graphics/Matrix.h"
#include "utils/Logger.h"


#include "graphics/Models/Geodetic/Cone.h"
#include "graphics/Models/Geodetic/Cylinder.h"
#include "graphics/Models/Geodetic/Sphere.h"
#include "graphics/Models/Geodetic/Torus.h"
#include "graphics/PerlinNoiseGenerator.h"
#include "graphics/GLUtils.h"

static Scene scene;



extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_geodeticBodies_GeodeticBodiesRenderer_nativeSurfaceCreate(
    JNIEnv* env, jobject thiz, jobject  asset_manager) {

    GLUtils::setEnvAndAssetManager(env, asset_manager);


    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_3D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    HardCodedModel* models[4];
    models[0] = new Torus(4.0, 10.0, 25, 42, 0);
    models[1] = new Sphere(4.0, 25, 42, 3);
    models[2] = new Cone(4.0, 8.0, 42, 2);
    models[3] = new Cylinder(2.0, 5, 42, 1);

    Transform t;
    t.translate(-4.0, 6.0, -3.0);
    //t.rotate(glm::radians(90.0), glm::vec3(1.0,0.0, 0.0));
    models[0]->setTransform(t.get());

    t.identity();
    t.translate(+1.5, -5.0, -1.0);
    models[1]->setTransform(t.get());

    t.identity();
    t.translate(5.5, +4.5, -1.0);
    models[2]->setTransform(t.get());


    t.identity();
    t.translate(-4.5, -5.5, +4.0);
    models[3]->setTransform(t.get());


    for(auto* poly: models){
        if(poly->init()) {
            scene.addModel(poly);
        }
    }
    scene.setLights();
}


extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_geodeticBodies_GeodeticBodiesRenderer_nativeSurfaceChange(
        JNIEnv *env, jobject type, jint width, jint height) {

scene.viewport(width, height);
PerlinNoiseGenerator::releaseTextures();
}

extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_geodeticBodies_GeodeticBodiesRenderer_nativeDrawFrame(
        JNIEnv *env, jobject type) {
scene.update();
scene.render();
}


