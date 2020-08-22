//
// Created by nathan on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H



#include <cmath>
#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

#include <GLES2/gl2.h>
#include <android/log.h>

#include "Polyhedron.h"
#include "graphics/GLUtils.h"
#include "graphics/Material.h"

namespace Polyhedrons {

    class Tetrahedron : public Polyhedron {
        std::string  LogTag;
        static const int POSITION_DATA_SIZE = 4;
        static const int NORMAL_DATA_SIZE = 0;
        static const int TEX_DATA_SIZE = 0;
        static const int VERTEX_DATA_SIZE_IN_ELEMENTS = TEX_DATA_SIZE +NORMAL_DATA_SIZE +POSITION_DATA_SIZE;
        static const int VERTEX_DATA_SIZE_IN_BYTES = BYTES_PER_FLOAT*VERTEX_DATA_SIZE_IN_ELEMENTS;

        /** from Polyhedron
        glm::vec3 *vertices;
        int nVertices;

        glm::vec3 *vertexNormals;
        int nNormals
        */
        GLuint cloudsProgram;
        GLuint monochromeProgram;
        GLuint wireframeProgram;

        Material* monochrome;
        // Attributes and uniforms handlers for the tetrahedron's materials

        GLint monochromePositionHandle;
        GLint monochromeMVPHandle;
        GLint monochromeMVHandle;
        GLint monochromeFaceNormHandle;
        GLint monochromeLightPosHandle;

        GLint wireframePositionHandler;
        GLint wireframeMVPHandler;
        GLint wireframeColorHandler;

        GLint cloudsPositionHandle;
        GLint cloudsSkyColorHandler;
        GLint cloudsCloudColorHandle;
        GLint cloudsFaceNormHandle;
        GLint cloudsLightPosHandle;
        GLint cloudsMVHandle;



            // Attributes and uniforms for the tetrahedron's wireframe
        GLint wireframeVetexAtributeHandle;
        GLint wireframeColorUniformHandle;

        GLint mMVPMatrixHandle;
        GLint mMVMatrixHandle;
        GLfloat normals[3*4];
        bool initBuffers();
        bool initShaders();
        virtual bool initVertices();
        virtual bool initFaces();
    public:
        Tetrahedron();
        Tetrahedron(glm::vec3 & position);

        ~Tetrahedron();

        virtual bool init();
        virtual void render(Camera& camera) ;
        virtual void destroy();
        virtual void update(); // call this to update the model's state before rendering
        virtual bool addMaterials();
        //virtual void postUpdate() = 0; // call this for post rendering update.

        void printGLString(const char *msg, GLenum gle);
        bool buildNormals();



    };
}

#endif //ANDROIDOPENGLESLESSONSCPP_TETRAHEDRON_H
