//
// Created by nathan on 28/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_MATERIAL_H
#define ANDROIDOPENGLESLESSONSCPP_MATERIAL_H

#include <string>
#include <map>
#include <vector>

#include "GLincludes.h"


class Material {

    std::map<std::string, GLuint> attribs;
    std::map<std::string, GLuint> uniforms;
    GLuint program;

    Material(GLuint program);
public:
    ~Material();
    bool isInitilized();
    GLuint getAttrib(const char* name);
    GLuint getUniform(const char* name);
    static Material* makeMaterial(const char* vertexSrc, const char* fragmentSrc,
                                  const char* attribs[], int nAttribs,
                                  const char* uniforms[], int nUniforms);
};


#endif //ANDROIDOPENGLESLESSONSCPP_MATERIAL_H
