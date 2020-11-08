//
// Created by Erez on 28/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_MATERIAL_H
#define ANDROIDOPENGLESLESSONSCPP_MATERIAL_H

#include <string>
#include <map>
#include <vector>

#include "graphics/GLincludes.h"


class Material {

    std::map<std::string, GLuint> attribs;
    std::map<std::string, GLuint> uniforms;
    GLuint program;

    Material(GLuint program);
public:
    ~Material();
    bool isInitilized();
    inline void activate(){glUseProgram(program);}
    inline void deactivate(){glUseProgram(0);}
    GLuint getAttrib(const char* name);
    GLuint getUniform(const char* name);
    static Material* makeMaterial(const char* vertexSrc, const char* fragmentSrc);
};


#endif //ANDROIDOPENGLESLESSONSCPP_MATERIAL_H
