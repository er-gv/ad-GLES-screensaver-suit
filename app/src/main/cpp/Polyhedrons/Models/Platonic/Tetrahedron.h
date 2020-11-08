//
// Created by Erez on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H



#include <cmath>
#include <string>

#include "graphics/GLincludes.h"

#include <android/log.h>

#include "Polyhedrons/Models/Platonic/Polyhedron.h"
#include "graphics/GLUtils.h"
#include "graphics/Material.h"

namespace Polyhedrons {

    class Tetrahedron : public Polyhedron {
        std::string  LogTag;
        static const int POSITION_DATA_SIZE = 3;
        static const int NORMAL_DATA_SIZE = 0;
        static const int TEX_DATA_SIZE =2;
        static const int VERTEX_DATA_SIZE_IN_ELEMENTS = TEX_DATA_SIZE +NORMAL_DATA_SIZE +POSITION_DATA_SIZE;
        static const int VERTEX_DATA_SIZE_IN_BYTES = BYTES_PER_FLOAT*VERTEX_DATA_SIZE_IN_ELEMENTS;


        /** from Polyhedron
        glm::vec3 *vertices;
        int nVertices;

        glm::vec3 *vertexNormals;
        int nNormals
        */
        glm::vec2 texcoors[4];

        Material* granite;
        // Attributes and uniforms handlers for the tetrahedron's materials

        GLuint positionAttributeHandle;
        GLuint texcoordAttributeHandle;

        GLuint MVPMatrixHandle;
        GLuint MVMatrixHandle;
        GLuint NormalMatrixHandle;

        GLuint ambiantHandler;
        GLuint diffuseHandler;
        GLuint specularHandler;
        GLuint shininessHandle;

        GLuint noiseScaleHandler;
        GLuint normalHandler;
        GLuint colorHandler;
        GLuint lightPositionHandler;
        GLuint textureSamplerHandler;

        GLuint textureDataHandler;

        bool initBuffers();
        bool initShaders();
        virtual bool initVertices();
        virtual bool initFaces();
    public:
        Tetrahedron();
        Tetrahedron(glm::vec3 & position);

        ~Tetrahedron();

        virtual bool init();
        virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) ;
        virtual void destroy();
        virtual void update(long time); // call this to update the model's state before rendering
        virtual bool addMaterials();
        //virtual void postUpdate() = 0; // call this for post rendering update.

        void printGLString(const char *msg, GLenum gle);
        bool buildNormals();



    };
}

#endif //ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H
