//
// Created by nathan on 10/04/20.
//

#include "utils/Scene.h"

#include <jni.h>
#include <GLES2/gl2.h>
#include "../graphics/GLUtils.h"
//#include "utils/Logger.h"

//#include "Polyhedrons/Tetrahedron.h"
static Scene scene;



extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_Polyhedrons_PolyhedronsRenderer_nativeSurfaceCreate(
        JNIEnv *env, jclass type, jobject asset_manager) {

    GLUtils::setEnvAndAssetManager(env, asset_manager);

    // Print some OpenGL info
   /* renderer->printGLString("Version", GL_VERSION);
    renderer->printGLString("Vendor", GL_VENDOR);
    renderer->printGLString("Renderer", GL_RENDERER);
    renderer->printGLString("Extensions", GL_EXTENSIONS);*/
    //Polyhedrons::Tetrahedron* t = new Polyhedrons::Tetrahedron();

    scene.setupCamera();
    scene.setLights();


}

extern "C" JNIEXPORT
void JNICALL
Java_com_learnopengles_android_Polyhedrons_PolyhedronsRenderer_nativeSurfaceChange(
        JNIEnv *env, jclass type, jint width, jint height) {

    scene.viewport(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_learnopengles_android_Polyhedrons_PolyhedronsRenderer_nativeDrawFrame(
        JNIEnv *env, jclass type) {

    scene.render();
}


