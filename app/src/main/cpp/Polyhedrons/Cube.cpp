//
// Created by nathan on 10/04/20.
//

#include "Cube.h"

Polyhedrons::Cube::Cube(): Polyhedron(){

}

Polyhedrons::Cube::~Cube(){

}

float* Polyhedrons::Cube::getVertices(){
    static float spatial[]={
            -0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, -0.5f,
            +0.5f, +0.5f, -0.5f,
            +0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, +0.5f,
            +0.5f, -0.5f, +0.5f,
            +0.5f, +0.5f, +0.5f,
            +0.5f, -0.5f, +0.5f

    };
    return spatial;
}
