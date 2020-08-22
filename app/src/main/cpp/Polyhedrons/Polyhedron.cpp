//
// Created by nathan on 16/04/20.
//

#include "Polyhedron.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

namespace Polyhedrons {

    Polyhedron::Polyhedron() :Model() {
        vertexDataBuffer = nullptr;
        vertices = nullptr;
        faces = nullptr;
        ibo = vbo = nullptr;
    }

    Polyhedron::Polyhedron(const glm::mat4& transform) :Model(transform) {
        vertexDataBuffer = nullptr;
        vertices = nullptr;
        faces = nullptr;
        ibo = vbo = nullptr;
    }

    Polyhedron::Polyhedron(const Polyhedron& p) : Model(p){
        *this = p;
    };

    Polyhedron& Polyhedron::operator=(const Polyhedron& p){
        destroy();
        nVertices = p.nVertices;
        delete vertices;
        vertices = new glm::vec3[nVertices];
        memcpy(vertices, p.vertices, nVertices*sizeof(glm::vec3));

        for(int i=0; i<nFaces;--i)
            delete faces[i];
        delete faces;
        nFaces = p.nFaces;
        faces = new (Polygon*[nFaces]);
        for(int i=0; i<nFaces;--i)
            faces[i] = p.faces[i];

    }


    Polyhedron::~Polyhedron() {
        destroy();
    }

    bool Polyhedron::init() {
        if (initVertices() && initFaces() && initBuffers()){
            return addMaterials();
        }
        return false;
    }


    void Polyhedron::destroy() {
        if (nullptr !=  vertexDataBuffer) {
            delete  vertexDataBuffer;
            vertexDataBuffer = nullptr;
        }

        if (nullptr != vertices) {
            delete vertices;
            vertices = nullptr;
        }
        if (nullptr != faces){
            for(int i=0; i<nFaces;--i)
                delete faces[i];
            delete faces;
            faces = nullptr;
        }
        if(nullptr != vbo){
            delete vbo;
            ibo = nullptr;
        }
        if(nullptr != ibo){
            delete ibo;
            ibo = nullptr;
        }
    }


/**********************************************************
 *  Inner class Polygon
 *
 ***********************************************************/


    Polyhedron::Polygon::Polygon(const Polyhedron& parent, int indices[], int nVerts) :mParent(parent){

        mIndices = new int[nVerts];
        memcpy(mIndices, indices, nVerts*sizeof(int));

        glm::vec3 p1 = mParent.vertices[mIndices[1]];
        glm::vec3 p2 = mParent.vertices[mIndices[2]];

        p1 -= mParent.vertices[mIndices[0]];
        p2 -= mParent.vertices[mIndices[0]];
        mNormal = computeNormal(mParent.vertices[mIndices[0]], mParent.vertices[mIndices[1]], mParent.vertices[mIndices[2]]);


    }

    Polyhedron::Polygon::~Polygon() {
        delete mIndices;
    }

    glm::vec3& Polyhedron::Polygon::getVertex(int index) {
        return mParent.vertices[mIndices[index]];
    }

    const glm::vec3&  Polyhedron::Polygon::getNormal() {
        return mNormal;
    }

    void Polyhedron::Polygon::setNormal(const glm::vec3& normal) {
        mNormal = normal;
    }

    Polyhedron::Polygon& Polyhedron::Polygon::operator=(const Polyhedron::Polygon& p){

        int n_indices = sizeof(*p.mIndices)/ sizeof(int);
        delete mIndices;
        mIndices = new int[n_indices];
        memcpy(mIndices, p.mIndices, sizeof(*p.mIndices));

        mNormal = p.mNormal;

        return *this;
    }
}