//
// Created by nathan on 19/09/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_GEODETICBODIESRENDERER_H
#define AD_GLES_SCREENSAVER_SUIT_GEODETICBODIESRENDERER_H


#include <jni.h>
#include <GLES3/gl3.h>
#include "graphics/GLUtils.h"
#include "graphics/Matrix.h"
#include "utils/Scene.h"
//#include "utils/Logger.h"

class GeodeticBodiesRenderer {


    //Logger* logger;
    public:
        GeodeticBodiesRenderer();
        ~GeodeticBodiesRenderer();

        void create();
        void change(int width, int height);
        void draw();
        void destroy();
        void printGLString(const char* msg, GLenum);
};


#endif //AD_GLES_SCREENSAVER_SUIT_GEODETICBODIESRENDERER_H
