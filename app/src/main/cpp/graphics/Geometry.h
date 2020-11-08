//
// Created by nathan on 21/09/20.
//

#ifndef AD_GLES_SCREENSAVER_SUIT_GEOMETRY_H
#define AD_GLES_SCREENSAVER_SUIT_GEOMETRY_H

#include "VertexLayout.h"
#include "GLincludes.h"

class Geometry {
enum Attribes {POSITION, NORMAL, COLOR, TEX};
public:
    Geometry(int nVertices, GLenum topo);
    GLuint vbo;
    GLuint ibo;
    void enableAttribute(Attribes attr, bool enable);
    int nVertices;
    GLenum topology;
    VertexLayout layout;
};


#endif //AD_GLES_SCREENSAVER_SUIT_GEOMETRY_H
