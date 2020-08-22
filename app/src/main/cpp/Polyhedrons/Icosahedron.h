//
// Created by nathan on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H


#include <cmath>
#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <GLES2/gl2.h>
#include <android/log.h>

#include "Polyhedron.h"
#include "graphics/GLUtils.h"

namespace Polyhedrons {

    class Icosahedron : public Polyhedron {
    private:

        std::string  LogTag;


        static const int POSITION_DATA_SIZE = 3;
        static const int POSITION_OFFSET = 0;
        static const int NORMAL_DATA_SIZE = 0;
        static const int NORMAL_OFFSET = POSITION_DATA_SIZE * BYTES_PER_FLOAT;
        static const int VERTEX_DATA_SIZE = NORMAL_DATA_SIZE+ POSITION_DATA_SIZE;
        static const int NUM_VERTICES = 12;
        static const int STRIDE = VERTEX_DATA_SIZE * BYTES_PER_FLOAT;
        static const int NUM_FACES = 20;


        float* verticesData;
        glm::vec3 vertices[NUM_VERTICES];
        glm::vec3 vertexNormals[NUM_VERTICES];
        GLuint materialProgram;
        GLuint wireframeProgram;
        GLuint*  vbo;
        GLuint*  ibo;
        // Attributes and uniforms handlers for the tetrahedron's material
        GLint positionAttributeHandle;
        GLint normalAttributeHandle;
        GLint lightPositionHandler;


        // Attributes and uniforms for the tetrahedron's wireframe
        GLint wireframeVetexAtributeHandle;
        GLint wireframeColorUniformHandle;

        GLint mMVPMatrixHandle;
        GLint mMVMatrixHandle;


        void buildBuffers();
        bool initShaders();


        public:
            Icosahedron();

            virtual ~Icosahedron();


            virtual bool init();
            virtual void render();
            virtual void destroy();
            virtual void update(); // call this to update the model's state before rendering
            //virtual void postUpdate() = 0; // call this for post rendering update.
            virtual bool initVerices();
            virtual bool initFaceNormals();

            void printGLString(const char *msg, GLenum gle);
            void buildGeometry();
            void buildNormals();
            void sendDataToGPU();

        };

    }

#endif //ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H
