//
// Created by nathan on 16/04/20.
//

#include "PerlinNoiseGenerator.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/noise.hpp>

GLubyte* PerlinNoiseGenerator::make3DNoise(unsigned int size){

    GLubyte* noiseBuffer = new GLubyte[size*size*size*4];
    if(!noiseBuffer){
        return nullptr;
    }
    float xFactor = 1.0f / (size - 1);
    float yFactor = 1.0f / (size - 1);
    float zFactor = 1.0f / (size - 1);
    for( int plane = 0; plane < size; plane++ ) {
        for( int row = 0; row < size; row++ ) {
            for( int col = 0 ; col < size; col++ ) {
                float x = xFactor * col;
                float y = yFactor * row;
                float z = zFactor * plane;
                float sum = 0.0f;
                float freq = 4.0;
                float amp = 0.5;
/*
                // Compute the sum for each octave
                for( int oct = 0; oct < 4; oct++ ) {
                    glm::vec3 p(x * freq, y * freq, z*freq);
                    float val = glm::perlin(p) * amp;
                    sum += val;
                    float result = (sum + 1.0f)* amp;

                    // Store in texture buffer
                    noiseBuffer[((plane*size*size + row * size + col) * 4) + oct] =
                            (GLubyte) ( result * 255.0f );
                    freq *= 2.0f;   // Double the frequency
                    amp *= 0.5;     // Next power of b
                }


*/
                for( int oct = 0; oct < 4; oct++ ) {
                    glm::vec3 p(x * freq, y * freq, z*freq);
                    float val = glm::perlin(p) * amp;
                    //sum += val;
                    //float result = (sum + 1.0f)* amp;

                    // Store in texture buffer
                    noiseBuffer[((plane*size*size + row * size + col) * 4) + oct] =
                            (GLubyte) ( val * 255.0f );
                    freq *= 2.0f;   // Double the frequency
                    amp *= 0.5;     // Next power of b
                }
            }// col
        }// row
    }// plane


    return noiseBuffer;
}

GLubyte* PerlinNoiseGenerator::make2DNoise(unsigned int size){

    GLubyte* noiseBuffer = new GLubyte[size*size*4];
    if(!noiseBuffer){
        return nullptr;
    }
    float xFactor = 1.0f / (size - 1);
    float yFactor = 1.0f / (size - 1);

    for( int row = 0; row < size; row++ ) {
        for( int col = 0 ; col < size; col++ ) {
            float x = xFactor * col;
            float y = yFactor * row;

            float sum = 0.0f;
            float freq = 4.0;
            float amp = 0.5;
            for( int oct = 0; oct < 4; oct++ ) {
                glm::vec2 p(x * freq, y * freq);
                float val = glm::perlin(p) * amp;
                sum += val;
                float result = (sum + 1.0f)* amp;

                // Store in texture buffer
                noiseBuffer[((row * size + col) * 4) + oct] =
                        (GLubyte) ( result * 255.0f );
                freq *= 2.0f;   // Double the frequency
                amp *= 0.5;     // Next power of b
            }
        }// col
    }// row

    return noiseBuffer;
}

static GLuint tex2D =0;
static GLuint tex3D =0;

void PerlinNoiseGenerator::releaseTextures(){
    tex2D = tex3D = 0;
}

GLuint PerlinNoiseGenerator::get3DTexture(unsigned int textureSize){
    GLubyte* noise3D = nullptr;
    if(0==tex3D) {
        noise3D = make3DNoise(textureSize);
        if(nullptr==noise3D)
            return 0;
        glGenTextures(1, &tex3D);
        glBindTexture(GL_TEXTURE_3D, tex3D);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8,
                     textureSize, textureSize, textureSize, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, noise3D);
        glBindTexture(GL_TEXTURE_3D, 0);
        delete noise3D;
    }
    return tex3D;
}



GLuint PerlinNoiseGenerator::get2DTexture(unsigned int textureSize){
    GLubyte* noise2D = nullptr;

    if(0==tex2D) {
        noise2D = make2DNoise(textureSize);
        if(nullptr==noise2D)
            return 0;
        glGenTextures(1, &tex2D);
        glBindTexture(GL_TEXTURE_2D, tex2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                     textureSize, textureSize, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, noise2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        delete noise2D;
    }
    return tex2D;
}