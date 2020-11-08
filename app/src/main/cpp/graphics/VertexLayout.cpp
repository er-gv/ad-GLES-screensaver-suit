//
// Created by nathan on 19/09/20.
//

#include "VertexLayout.h"

GLuint VertexLayout::getByteSize(){
    GLuint size = position+color+normal+tex;
    for(auto& property : aux){
        size+= property.second;
    }
    return size*sizeof(GLfloat);
}

GLuint VertexLayout::getComponentSize(const char* name) throw(std::out_of_range){
    if(strcmp(name, "POSITION"))
        return position;
    if(strcmp(name, "COLOR"))
        return color;
    if(strcmp(name, "NORMAL"))
        return normal;
    if(strcmp(name, "TEX"))
        return tex;
    auto val = aux.at(name);
    return val;
}