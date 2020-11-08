//
// Created by Erez on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_OCTAHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_OCTAHEDRON_H



#include <cmath>
#include <string>

#include "graphics/GLincludes.h"

#include <android/log.h>

#include "Polyhedron.h"
#include "graphics/GLUtils.h"
#include "graphics/Material.h"


namespace Polyhedrons {

    class Octahedron : public Polyhedron {
        std::string  LogTag;
        static const int POSITION_DATA_SIZE = 4;
        static const int NORMAL_DATA_SIZE = 0;
        static const int TEX_DATA_SIZE = 0;
        static const int VERTEX_DATA_SIZE_IN_ELEMENTS = TEX_DATA_SIZE +NORMAL_DATA_SIZE +POSITION_DATA_SIZE;
        static const int VERTEX_DATA_SIZE_IN_BYTES = BYTES_PER_FLOAT*VERTEX_DATA_SIZE_IN_ELEMENTS;

        static constexpr GLfloat faceColors[]={
                                1.0, 0.0, 0.0,
                                1.0, 1.0, 0.0,
                                1.0, 1.0, 1.0,
                                0.0, 0.0, 1.0};
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
        // Attributes and uniforms handlers for the Octahedron's materials

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



            // Attributes and uniforms for the Octahedron's wireframe
        GLint wireframeVetexAtributeHandle;
        GLint wireframeColorUniformHandle;

        GLint mMVPMatrixHandle;
        GLint mMVMatrixHandle;
        bool initBuffers();
        bool initShaders();
        virtual bool initVertices();
        virtual bool initFaces();
    public:
        Octahedron();
        Octahedron(glm::vec3 & position);

        ~Octahedron();

        virtual bool init();
        virtual void render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) ;
        virtual void destroy();
        virtual void update(long time); // call this to update the model's state before rendering
        virtual bool addMaterials();
        //virtual void postUpdate() = 0; // call this for post rendering update.

        void printGLString(const char *msg, GLenum gle);
        bool buildNormals();



    };
};


#endif //ANDROIDOPENGLESLESSONSCPP_OCTAHEDRON_H
