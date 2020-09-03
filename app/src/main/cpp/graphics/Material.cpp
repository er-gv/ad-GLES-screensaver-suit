//
// Created by nathan on 28/04/20.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Material.h"
#include "GLUtils.h"
#include "GLincludes.h"
#include <vector>
#include <string>
#include <utility>


Material::Material(GLuint program) :program(program){}

Material::~Material() {
    glDeleteProgram(program);
}

bool Material::isInitilized(){
    return (program>0);
}

GLuint Material::getAttrib(const char* name) {
    return static_cast<GLuint>(glGetAttribLocation(program, name));
}

GLuint Material::getUniform(const char* name) {
    return static_cast<GLuint>(glGetUniformLocation(program, name));
}

Material* Material::makeMaterial(const char* vertexSrc, const char* fragmentSrc){

    Material* mat;
    const char *fragment = GLUtils::openTextFile(fragmentSrc);
    const char *vertex = GLUtils::openTextFile(vertexSrc);


    // Set program handles
    int program = GLUtils::createProgram(&vertex, &fragment);
    if (!program) {
        //LOGD("Could not create program");
        return nullptr;
    }
    mat = new Material(program);
    return mat;
}
