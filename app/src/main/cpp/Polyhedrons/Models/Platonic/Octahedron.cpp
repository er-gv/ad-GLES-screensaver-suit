//
// Created by Erez on 16/04/20.
//

#include "Octahedron.h"
//
// Created by Erez on 16/04/20.
//

#include <android/log.h>
#include <graphics/PerlinNoiseGenerator.h>
#include "Octahedron.h"
#include "graphics/Material.h"



namespace Polyhedrons {
    Octahedron::Octahedron() : Polyhedron(), LogTag("Octahedron") {
        nVertices = 6;
        nFaces = 8;
        faces = nullptr;
    }

    Octahedron::Octahedron(glm::vec3 &position) : Polyhedron(), LogTag("Octahedron") {
        nVertices = 6;
        nFaces = 8;
        faces =  nullptr;
        translate(position);
    }

    Octahedron::~Octahedron() {
        destroy();
    }

    bool Octahedron::init() {
        //init the vertices, faces and normals for this polyhedron.
        //then put everything into gpu buffers

        //initVertices()
        //initFaces(vertices)
        //initBuffers(vertices, faces, )
        //addMaterial(material name)
        return (initVertices() && initFaces() &&  initBuffers() && initShaders());
    }

    bool Octahedron::initVertices() {
        //set tex coordintes
        texcoors[0] = glm::vec2(0.5, 0.0);
        texcoors[1] = glm::vec2(0.0, 0.5);
        texcoors[2] = glm::vec2(1.0, 0.5);
        texcoors[3] = glm::vec2(0.0, 0.5);
        texcoors[4] = glm::vec2(1.0, 0.5);
        texcoors[5] = glm::vec2(0.5, 1.0);

        vertices = new glm::vec3[nVertices];

        vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f); //top of the pyramid
        vertices[5] = glm::vec3(0.0f, -1.0f, 0.0f); //bottom of the pyramid
        double theta = 0.0;
        for(int i=1; i<5; i++){

            double cosine =cos(glm::radians(theta));
            double sine =sin(glm::radians(theta));
            vertices[i] = glm::vec3(cosine, 0.0, sine);
            theta+=90.0;
        }

        return true;
    }

    bool Octahedron::initShaders() {

        const char vertex_shader[] = "shaders/vertex/vertex_turbulence_2DTexture.glsl";
        const char fragment_shader[] = "shaders/fragment/fragment_wood_2DTexture.glsl";
        wood = Material::makeMaterial(vertex_shader, fragment_shader);
        if(!wood || !addMaterials())
            return false;

        positionAttributeHandle = wood->getAttrib("a_Position");
        texcoordAttributeHandle = wood->getAttrib("a_TexCoord");

        MVMatrixHandle = wood->getUniform("u_MVMatrix");
        MVPMatrixHandle = wood->getUniform("u_MVPMatrix");
        NormalMatrixHandle = wood->getUniform("u_NormalMat");

        textureSamplerHandler = wood->getUniform("u_Texture");

        lightPositionHandler = wood->getUniform("u_LightPos");
        normalHandler = wood->getUniform("u_FaceNormal");
        lightWoodColorHandler = wood->getUniform("u_LightWood");
        darkWoodColorHandler = wood->getUniform("u_DarkWood");


        sliceMatHandler = wood->getUniform("u_SliceMat");
        diffuseHandler = wood->getUniform("u_diffuseCoaff");
        ambiantHandler = wood->getUniform("u_ambiantCoaff");
        specularHandler = wood->getUniform("u_specularCoaff");
        shininessHandle = wood->getUniform("u_shininess");
        textureDataHandler = PerlinNoiseGenerator::get2DTexture();
        return true;
    }

    bool Octahedron::initFaces() {
        char buffer[100];
        normals = new glm::vec3[nFaces];
        if(nullptr == normals)
            return false;

        normals[0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[2]-vertices[0]));
        normals[1] = glm::normalize(glm::cross(vertices[2]-vertices[0], vertices[3]-vertices[0]));
        normals[2] = glm::normalize(glm::cross(vertices[3]-vertices[0], vertices[4]-vertices[0]));
        normals[3] = glm::normalize(glm::cross(vertices[4]-vertices[0], vertices[1]-vertices[0]));
        normals[4] = glm::normalize(glm::cross(vertices[1]-vertices[5], vertices[2]-vertices[5]));
        normals[5] = glm::normalize(glm::cross(vertices[2]-vertices[5], vertices[3]-vertices[5]));
        normals[6] = glm::normalize(glm::cross(vertices[3]-vertices[5], vertices[4]-vertices[5]));
        normals[7] = glm::normalize(glm::cross(vertices[4]-vertices[5], vertices[1]-vertices[5]));

        return true;
    }

    bool Octahedron::initBuffers() {
        vbo = new GLuint[1];
        ibo = new GLuint[1];
        glGenBuffers(1, vbo);
        glGenBuffers(1, ibo);

        GLuint constexpr trianglesIndexBuffer[] ={1,2,0, 2,3,0, 3,4,0, 4,1,0,
                                                  5,2,1, 5,3,2, 5,4,3, 5,1,4};

        vertexDataBuffer = new GLfloat[nVertices*VERTEX_DATA_SIZE_IN_ELEMENTS];
        char buffer[100];
        for(int i=0; i< nVertices; i++){

            int j = VERTEX_DATA_SIZE_IN_ELEMENTS*i;
            vertexDataBuffer[j] = vertices[i].x;
            vertexDataBuffer[j+1] = vertices[i].y;
            vertexDataBuffer[j+2] = vertices[i].z;
            vertexDataBuffer[j+3] = texcoors[i].x;
            vertexDataBuffer[j+4] = texcoors[i].y;
        }
        if (vbo[0]>0 && ibo[0]>0 ) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, nVertices*VERTEX_DATA_SIZE_IN_BYTES,  vertexDataBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesIndexBuffer),
                         trianglesIndexBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //TODO at this point we can delete vertexDataBuffer.
        return true;
    }

    void Octahedron::destroy() {
        //TODO delete unnecessary memory
        glDeleteBuffers(1, vbo);
        glDeleteBuffers(1, ibo);
    }

    //TODO transform/projection/pre-render state update
    void Octahedron::update(long time) {

        float angleInDegrees = (360.0f / 10000.0f) * ((int) time);
        activeTransform.setTransform(initialTransform.get());
        activeTransform.scale(3.1f);
        activeTransform.translate(glm::vec3(-0.3, -0.6, 0.0));
        activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(3.14, 1.0, 0.4)) ;
        //activeTransform.rotate(glm::radians(40.0), glm::vec3(1.0, 0.0, 0.0)) ;

    }

    //TODO bugfix
    void Octahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {
    static glm::vec3 woodColors[] ={
            glm::vec3(1.0, 0.75, 0.2),
            glm::vec3(0.8, 0.5, 0.1),
            glm::vec3(0.3647058, 0.1607843, 0.0235294),
            glm::vec3(0.71764705, 0.43529411, 0.12549019)

    };
    static int colorIndices[] ={
                0,1, 3,2, 0,1, 3,2,
                3,2, 0,1, 3,2, 0,1
        };
        wood->activate();
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(positionAttributeHandle, 3, GL_FLOAT,
                              static_cast<GLboolean>(false),
                              VERTEX_DATA_SIZE_IN_BYTES, 0);
        glEnableVertexAttribArray(positionAttributeHandle);

        glVertexAttribPointer(texcoordAttributeHandle, 2, GL_FLOAT,
                              static_cast<GLboolean>(false),
                              VERTEX_DATA_SIZE_IN_BYTES,
                              reinterpret_cast<const void *>(3 * BYTES_PER_FLOAT));
        glEnableVertexAttribArray(texcoordAttributeHandle);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureDataHandler);
        glUniform1i(textureSamplerHandler, 0);


        glBindBuffer(GL_ARRAY_BUFFER, ibo[0]);
        glm::mat4 viewT = viewMat*activeTransform.get();
        glm::mat4 projT = projectionMat*viewT;


        glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
        glUniformMatrix4fv(MVMatrixHandle, 1, false, glm::value_ptr(viewT));
        glUniformMatrix4fv(NormalMatrixHandle, 1, false, glm::value_ptr(viewT));


        glUniform3fv(lightPositionHandler, 1, glm::value_ptr(lightPos));
        glUniform1f(ambiantHandler, 0.2);
        glUniform1f(diffuseHandler, 1.0);
        glUniform1f(specularHandler, 0.8);
        glUniform1f(shininessHandle, 5.0);

        //draw triangles
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glUniformMatrix4fv(sliceMatHandler, 1, false, glm::value_ptr(getSliceingMat()));

        for(int i=0; i<nFaces; ++i) {
            glUniform3fv(lightWoodColorHandler, 1, glm::value_ptr(woodColors[colorIndices[2*i]]));
            glUniform3fv(darkWoodColorHandler, 1, glm::value_ptr(woodColors[colorIndices[2*i+1]]));
            glUniform3fv(normalHandler, 1, glm::value_ptr(normals[i]));
        //    glUniform1f(noiseScaleHandler,pow(3.0, i-1));
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                           reinterpret_cast<const void *>(3 * i * sizeof(GLuint)));

        }


        glDisableVertexAttribArray(positionAttributeHandle);
        glDisableVertexAttribArray(texcoordAttributeHandle);
        wood->deactivate();
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Octahedron::printGLString(const char *msg, GLenum gle) {
        __android_log_print(ANDROID_LOG_INFO, Octahedron::LogTag.c_str(), msg, gle);
    }

    bool Octahedron::addMaterials() {
        return (0< (textureDataHandler = PerlinNoiseGenerator::get2DTexture()));
    }


}
