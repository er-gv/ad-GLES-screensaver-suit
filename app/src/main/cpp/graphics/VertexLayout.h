//
// Created by nathan on 19/09/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_VERTEXLAYOUT_H
#define AD_GLES_SCREENSAVER_SUIT_VERTEXLAYOUT_H


#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <map>
#include <string>

struct VertexLayout {
    unsigned int position;
    unsigned int color;
    unsigned int normal;
    unsigned int tex;
    std::map<const char*, unsigned int> aux;

    GLuint getByteSize();

    GLuint getComponentSize(const char* name) throw(std::out_of_range);

};


#endif //AD_GLES_SCREENSAVER_SUIT_VERTEXLAYOUT_H
