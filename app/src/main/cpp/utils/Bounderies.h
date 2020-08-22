//
// Created by nathan on 11/05/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_BOUNDERIES_H
#define ANDROIDOPENGLESLESSONSCPP_BOUNDERIES_H

#include "graphics/GLincludes.h"
struct Bounderies{

    GLfloat front, back, left, right, top, bottom;
    Bounderies(GLfloat front, GLfloat back, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom);
    Bounderies();
};


#endif //ANDROIDOPENGLESLESSONSCPP_BOUNDERIES_H
