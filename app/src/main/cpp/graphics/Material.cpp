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

Material* Material::makeMaterial(
        const char* vertexSrc, const char* fragmentSrc,
        const char* attribs[], int nAttribs, const char* uniforms[], int nUniforms){

    Material* mat;
    const char *vertex = GLUtils::openTextFile(vertexSrc);
    const char *fragment = GLUtils::openTextFile(fragmentSrc);

    // Set program handles
    int program = GLUtils::createProgram(&vertex, &fragment);
    if (!program) {
        //LOGD("Could not create program");
        return nullptr;
    }
    mat = new Material(program);
    int k = nAttribs-1;
    while(k>=0){
        mat->attribs[attribs[k]] = mat->getAttrib(attribs[k]);
        k--;
    }

    k = nUniforms -1;
    while(k>=0){
        mat->uniforms[uniforms[k]] = mat->getUniform(uniforms[k]);
        k--;
    }
    return mat;
}
