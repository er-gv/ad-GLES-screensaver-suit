//
// Created by nathan on 10/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_CUBE_H
#define ANDROIDOPENGLESLESSONSCPP_CUBE_H


#include "Polyhedron.h"
#include <GLES2/gl2.h>

namespace Polyhedrons {

    class Cube : public Polyhedrons::Polyhedron {

    private:
     GLfloat* verticesBuffer;
     GLshort* triangleStripsIndicesBuffer;

     static const int POSITION_DATA_SIZE = 3;
     static const int POSITION_OFFSET = 0;

    static const int NORMAL_DATA_SIZE = 3;
    static const int NORMAL_OFFSET = 3;

    static const int COLOR_DATA_SIZE = 3;
    static const int COLOR_OFFSET = 6;
    static const int VERTEX_DATA_SIZE = COLOR_DATA_SIZE +NORMAL_DATA_SIZE+ POSITION_DATA_SIZE;
    static const int stride = VERTEX_DATA_SIZE * BYTES_PER_FLOAT;
    float* mLightPos;
    static float* getVertices();




    public:
        Cube();

        virtual ~Cube();
    };

}
#endif //ANDROIDOPENGLESLESSONSCPP_CUBE_H
