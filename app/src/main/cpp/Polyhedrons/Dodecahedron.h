//
// Created by Erez on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_DODECAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_DODECAHEDRON_H


#include "Polyhedron.h"
#include <cmath>
#include <string>

#include <android/log.h>

#include "graphics/GLincludes.h"
#include "graphics/Material.h"

namespace Polyhedrons {
    class Dodecahedron : public Polyhedrons::Polyhedron {
    private:


        std::string  LogTag;


        static const int POSITION_DATA_SIZE = 3;
        static const int POSITION_OFFSET = 0;
        static const int NORMAL_DATA_SIZE = 0;
        static const int VERTEX_DATA_SIZE = NORMAL_DATA_SIZE+ POSITION_DATA_SIZE;
        static const int NUM_VERTICES = 20;
        static const int NUM_FACES = 12;

        glm::vec3 vertices[NUM_VERTICES]{};
        glm::vec3 vertexNormals[NUM_FACES]{};
        Material* monochrome{};

        GLuint  vbo{};
        GLuint  ibo[2]{};
        // Attributes and uniforms handlers for the tetrahedron's material
        GLint positionAttributeHandle{};
        GLint normalAttributeHandle{};
        GLint lightPositionHandler{};


        // Attributes and uniforms for the tetrahedron's wireframe
        GLint wireframeVetexAtributeHandle{};
        GLint wireframeColorUniformHandle{};

        GLint mMVPMatrixHandle{};
        GLint mMVMatrixHandle{};

        bool initShaders();


    public:
        Dodecahedron();

        virtual ~Dodecahedron();


        virtual bool init();

        virtual void
        render(glm::mat4 &viewMat, glm::mat4 &projectionMat, const glm::vec3 &lightPos);

        virtual void destroy();

        virtual void update(long time); // call this to update the model's state before rendering
        //virtual void postUpdate() = 0; // call this for post rendering update.
        virtual bool initVertices();

        virtual bool initFaces();

        virtual bool initBuffers();

        void printGLString(const char *msg, GLenum gle);

        virtual bool addMaterials();


    };

}
#endif //ANDROIDOPENGLESLESSONSCPP_DODECAHEDRON_H
