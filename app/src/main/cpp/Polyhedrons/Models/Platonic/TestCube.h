//
// Created by nathan on 15/05/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H
#define ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H



#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>

#include <cmath>
#include <string>

#include "graphics/Material.h"
#include "Polyhedron.h"


namespace Polyhedrons {
    class TestCube : public Polyhedron {
    private:


        std::string LogTag;


        static const int POSITION_DATA_SIZE = 3;
        static const int NORMAL_DATA_SIZE = 0;
        static const int TEX_DATA_SIZE = 3;
        static const int VERTEX_DATA_SIZE_ELEMS = TEX_DATA_SIZE+ NORMAL_DATA_SIZE + POSITION_DATA_SIZE;
        static const int VERTEX_DATA_SIZE_BYTES = VERTEX_DATA_SIZE_ELEMS * BYTES_PER_FLOAT;

        static const int POS_OFFSET = 0;
        static const int NORMAL_OFFSET = 0*sizeof(GLfloat) ;
        static const int TEX_OFFSET = POSITION_DATA_SIZE*sizeof(GLfloat) ;

        static const int NUM_VERTICES = 8;
        static const int NUM_FACES = 6;
        GLuint* textureBuffer;
        static const int TEXTURE_DIMENTION = 128;
        Material *textureTapestry;

        GLuint m_vbo;
        GLuint m_ibo;
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

        bool initShaders();


    public:
        TestCube();

        virtual ~TestCube();


        virtual bool init();

        virtual void
        render(glm::mat4 &viewMat, glm::mat4 &projectionMat, const glm::vec3 &lightPos);

        virtual void destroy();

        virtual void update(long time); // call this to update the model's state before rendering
        //virtual void postUpdate() = 0; // call this for post rendering update.
        virtual bool initVertices();

        virtual bool initFaces();

        virtual bool initBuffers();

        virtual bool addMaterials();

        bool initTexture();

        void printGLString(const char *msg, GLenum gle);

    };

}
#endif //ANDROIDOPENGLESLESSONSCPP_TESTCUBE_H
