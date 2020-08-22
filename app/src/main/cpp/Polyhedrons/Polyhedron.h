//
// Created by nathan on 16/04/20.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_POLYHEDRON_H
#define ANDROIDOPENGLESLESSONSCPP_POLYHEDRON_H

#include "graphics/Matrix.h"
#include "graphics/GLUtils.h"
#include "graphics/Model.h"
#include "graphics/Camera.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Polyhedrons {

    class Polyhedron: public Model {

    protected: class Polygon {

        private:
            int *mIndices;
            glm::vec3 mNormal;
            const Polyhedron& mParent;

            inline glm::vec3 computeNormal(glm::vec3 const&a, glm::vec3 const& b , glm::vec3 const& c){
                return glm::normalize(glm::cross(c-a, b-a));
            }
        public:
            Polygon(const Polyhedron& parent, int vertices[], int nVerts);

            ~Polygon();
            Polyhedron::Polygon& operator=(const Polyhedron::Polygon& p);
            glm::vec3& getVertex(int index);

            const glm::vec3 &getNormal();

            void setNormal(const glm::vec3 &normal);
        };

    protected:
        static const int BYTES_PER_FLOAT = sizeof(GLfloat);
        static const int BYTES_PER_SHORT = sizeof(GLshort);
        static const int BYTES_PER_INT = sizeof(GLint);
        static const int BYTES_PER_UINT = sizeof(GLuint);

        GLfloat* vertexDataBuffer;
        glm::vec3 *vertices;
        int nVertices;

        //glm::vec3 *vertexNormals;

        Polyhedron::Polygon** faces;
        int nFaces;
        GLuint* vbo; int nVbos;
        GLuint* ibo; int nIbos;


    public:
        Polyhedron();
        Polyhedron(const glm::mat4& transform);
        Polyhedron(const Polyhedron& p);
        virtual ~Polyhedron();

        Polyhedron& operator=(const Polyhedron& p);

        bool init();
        virtual bool initVertices()=0;
        virtual bool initFaces()=0;
        virtual bool initBuffers()=0;
        virtual bool addMaterials()=0;
        virtual void render(Camera& camera) = 0;
        virtual void destroy();
        virtual void update() = 0; // call this to update the model's state before rendering
        //virtual void postUpdate() = 0; // call this for post rendering update.
        virtual bool initShaders()=0;




    };
}


#endif //ANDROIDOPENGLESLESSONSCPP_POLYHEDRON_H
