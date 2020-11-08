//
// Created by Erez on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H


#include <cmath>
#include <string>

#include <android/log.h>

#include "Polyhedron.h"
#include "graphics/GLUtils.h"
#include "graphics/Material.h"

namespace Polyhedrons {

    class Icosahedron : public Polyhedron {
    private:

        std::string  LogTag;


        static const int POSITION_DATA_SIZE = 3;
        static const int POSITION_OFFSET = 0;

        static const int TEX_DATA_SIZE = 3;
        static const int TEX_OFFSET = POSITION_DATA_SIZE*BYTES_PER_FLOAT;

        static const int NORMAL_DATA_SIZE = 0;

        static const int VERTEX_DATA_SIZE = TEX_DATA_SIZE + NORMAL_DATA_SIZE + POSITION_DATA_SIZE;
        static const int VERTEX_DATA_SIZE_BYTES = VERTEX_DATA_SIZE * BYTES_PER_FLOAT;
        static const int NUM_VERTICES = 12;
        static const int NUM_FACES = 20;

        glm::vec3 vertices[NUM_VERTICES];
        glm::vec3 vertexNormals[NUM_FACES];
        Material* paintSpatter;
        Material* wireFrame;

        GLuint  vbo;
        GLuint  ibo[3];
        // Attributes and uniforms handlers for the tetrahedron's material
        GLuint positionAttributeHandle;
        GLuint texcoordAttributeHandle;
        //GLuint normalAttributeHandle;

        GLuint MVPMatrixHandle;
        GLuint MVMatrixHandle;
        GLuint NormalMatrixHandle;
        GLuint lightPositionHandler;
        GLuint textureSamplerHandler;
        GLuint textureDataHandler;
        GLuint normalHandle;

        GLuint paintColorHandler;
        GLuint materialColorHandler;
        GLuint thresholdHandler;

        bool initShaders();

        public:
            Icosahedron();

            virtual ~Icosahedron();


            virtual bool init();

            virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);
            virtual void destroy();
            virtual void update(long time); // call this to update the model's state before rendering
            //virtual void postUpdate() = 0; // call this for post rendering update.
            virtual bool initVertices();
            virtual bool initFaces();
            virtual bool initBuffers();
            //void printGLString(const char *msg, GLenum gle);

        virtual bool addMaterials();


        };

    }

#endif //ANDROIDOPENGLESLESSONSCPP_ICOSAHEDRON_H
