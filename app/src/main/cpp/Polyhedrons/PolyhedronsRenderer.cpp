//
// Created by nathan on 10/04/20.
//

#include "utils/Scene.h"

#include <jni.h>
#include <GLES2/gl2.h>
#include <graphics/PerlinNoiseGenerator.h>
#include "graphics/GLUtils.h"
//#include "utils/Logger.h"

#include "Polyhedrons/Models/Platonic/Tetrahedron.h"
#include "Polyhedrons/Models/Platonic/TestCube.h"
#include "Polyhedrons/Models/Platonic/Octahedron.h"
#include "Polyhedrons/Models/Platonic/Icosahedron.h"
#include "Polyhedrons/Models/Platonic/Dodecahedron.h"
static Scene scene;



extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_Polyhedrons_PolyhedronsRenderer_nativeSurfaceCreate(
        JNIEnv *env, jclass type, jobject asset_manager) {

    GLUtils::setEnvAndAssetManager(env, asset_manager);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Polyhedrons::Polyhedron* models[5];
    models[0] =  new Polyhedrons::TestCube();
    models[1] =  new Polyhedrons::Octahedron();
    models[2] =  new Polyhedrons::Icosahedron();
    models[3] =  new Polyhedrons::Dodecahedron();
    models[4] =  new Polyhedrons::Tetrahedron();

    Transform t;
    t.translate(5.5, -3.5, -1.0);
    models[1]->setTransform(t.get());

    t.identity();
    t.translate(-3.5, -5.5, -1.0);
    models[2]->setTransform(t.get());

    t.identity();
    t.translate(+3.5, +5.5, +4.0);
    models[3]->setTransform(t.get());

    t.identity();
    t.translate(1.8, -5.85, +4.0);
    models[4]->setTransform(t.get());
    for(auto* poly: models){
        if(poly->init()) {
            scene.addModel(poly);
        }
    }

    scene.setLights();


}

extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_Polyhedrons_PolyhedronsRenderer_nativeSurfaceChange(
        JNIEnv *env, jclass type, jint width, jint height) {

    scene.viewport(width, height);
    PerlinNoiseGenerator::releaseTextures();
}

extern "C" JNIEXPORT void JNICALL
Java_com_ergv_glScreenSavers_Polyhedrons_PolyhedronsRenderer_nativeDrawFrame(
        JNIEnv *env, jclass type) {
    scene.update();
    scene.render();
}


