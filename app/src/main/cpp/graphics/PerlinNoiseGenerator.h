//
// Created by nathan on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_PERLINNOISEGENERATOR_H
#define ANDROIDOPENGLESLESSONSCPP_PERLINNOISEGENERATOR_H

//#include "graphics/GLincludes.h"
#include <GLES3/gl3.h>
//#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>

class PerlinNoiseGenerator {
private:
    static GLubyte* make2DNoise(unsigned int size);
    static GLubyte* make3DNoise(unsigned int size);


public:
    static GLuint get3DTexture(unsigned int textureSize=16);
    static GLuint get2DTexture(unsigned int textureSize=16);
    static void releaseTextures();
};


#endif //ANDROIDOPENGLESLESSONSCPP_PERLINNOISEGENERATOR_H
