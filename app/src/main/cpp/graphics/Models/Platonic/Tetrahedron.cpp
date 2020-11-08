//
// Created by nathan on 16/04/20.
//

#include <android/log.h>
#include <graphics/PerlinNoiseGenerator.h>
#include "Tetrahedron.h"
#include "graphics/Material.h"



namespace Polyhedrons {
    Tetrahedron::Tetrahedron() : Polyhedron(), LogTag("TETRAHEDRON") {
        nVertices = 4;
        nFaces = 4;
        faces = nullptr;
    }

    Tetrahedron::Tetrahedron(glm::vec3 &position) : Polyhedron(), LogTag("TETRAHEDRON") {
        nVertices = 4;
        nFaces = 4;
        faces =  nullptr;
        translate(position);
    }

    Tetrahedron::~Tetrahedron() {
        destroy();
    }

    bool Tetrahedron::init() {
        //init the vertices, faces and normals for this polyhedron.
        //then put everything into gpu buffers

        //initVertices()
        //initFaces(vertices)
        //initBuffers(vertices, faces, )
        //addMaterial(material name)
        return (initVertices() && initFaces() &&  initBuffers() && initShaders());
    }

    bool Tetrahedron::initVertices() {
        //vertexNormals = new glm::vec3[nVertices];
        vertices = new glm::vec3[nVertices];

        vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f); //top of the pyramid
        texcoors[0] = glm::vec2(0.5, 0.5);
        double theta = glm::radians(60.0);
        double delta = glm::radians(120.0);
        for(int i=1; i<4; i++){
            double cosine = cos(theta);
            double sine = sin(theta);
            vertices[i] = glm::vec3(cosine, 0.0, sine);
            texcoors[i] = glm::vec2(0.5+0.5*cosine, 0.5+0.5*sine);
            theta+=delta;
        }

        {
            char buffer[100];
            for (int i = 0; i < 4; i++) {
                sprintf(buffer, "vertices[%d] = (%f, %f, %f).", i, vertices[i].x, vertices[i].y,
                        vertices[i].z);
                __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s", buffer);
            }
        }
        return true;
    }
    void Tetrahedron::destroy() {
        //TODO delete unnecessary memory
        glDeleteBuffers(1, vbo);
        glDeleteBuffers(1, ibo);
    }


    bool Tetrahedron::initFaces() {
        char buffer[100];
        normals = new glm::vec3[nFaces];
        if(nullptr == normals)
            return false;

        normals[0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[2]-vertices[0]));
        normals[1] = glm::normalize(glm::cross(vertices[2]-vertices[0], vertices[3]-vertices[0]));
        normals[2] = glm::normalize(glm::cross(vertices[3]-vertices[0], vertices[1]-vertices[0]));
        normals[3] = glm::normalize(glm::cross(vertices[1]-vertices[2], vertices[3]-vertices[2]));

        return true;
    }
    bool Tetrahedron::initShaders() {
        if(!addMaterials())
            return false;

        positionAttributeHandle = granite->getAttrib("a_Position");
        texcoordAttributeHandle = granite->getAttrib("a_TexCoord");

        MVMatrixHandle = granite->getUniform("u_MVMatrix");
        MVPMatrixHandle = granite->getUniform("u_MVPMatrix");
        NormalMatrixHandle = granite->getUniform("u_NormalMat");

        textureSamplerHandler = granite->getUniform("u_Texture");

        lightPositionHandler = granite->getUniform("u_LightPos");
        normalHandler = granite->getUniform("u_FaceNormal");
        colorHandler = granite->getUniform("u_FaceColor");


        noiseScaleHandler = granite->getUniform("u_noiseScale");
        diffuseHandler = granite->getUniform("u_diffuseCoaff");
        ambiantHandler = granite->getUniform("u_ambiantCoaff");
        specularHandler = granite->getUniform("u_specularCoaff");
        shininessHandle = granite->getUniform("u_shininess");

        return true;
    };



    bool Tetrahedron::initBuffers() {
        vbo = new GLuint[1];
        ibo = new GLuint[1];
        glGenBuffers(1, vbo);
        glGenBuffers(1, ibo);

        GLuint constexpr trianglesIndexBuffer[] ={1,2,0, 2,3,0, 3,1,0, 3,2,1};

        vertexDataBuffer = new GLfloat[nVertices*5];
        char buffer[100];
        for(int i=0; i< nVertices; i++){
            sprintf(buffer, "nVert[%d] = (%f, %f, %f).", i, vertices[i].x, vertices[i].y, vertices[i].z);
            __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s",
                                buffer);
            int j = 5*i;
            vertexDataBuffer[j] = vertices[i].x;
            vertexDataBuffer[j+1] = vertices[i].y;
            vertexDataBuffer[j+2] = vertices[i].z;
            vertexDataBuffer[j+3] = texcoors[i].x;
            vertexDataBuffer[j+4] = texcoors[i].y;
        }
        if (vbo[0]>0 && ibo[0]>0 ) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, 5*nVertices*sizeof(GLfloat),  vertexDataBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesIndexBuffer),
                         trianglesIndexBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //TODO at this point we can delete vertexDataBuffer.
        return true;
    }

       //TODO transform/projection/pre-render state update
    void Tetrahedron::update(long time) {

        float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

        activeTransform.setTransform(initialTransform.get());
        activeTransform.scale(4.4f);
        activeTransform.translate(glm::vec3(0.0, 1.45,-0.0));
        activeTransform.rotate(glm::radians(90.0), glm::vec3(0.0, 0.0, 1.0)) ;
        activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.4, 1.0, -0.8)) ;
        //activeTransform.rotate(glm::radians(40.0), glm::vec3(1.0, 0.0, 0.0)) ;

    }

    //TODO bugfix
    void Tetrahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {
        static constexpr GLfloat faceColors[]={
                1.0, 0.77647, 0.6, 1.0,
                1.0, 0.0, 1.0, 1.0,
                0.0, 1.0, 0.0, 1.0,
                1.0, 1.0, 1.0, 1.0};
        granite->activate();
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
        for(int i=0; i<4; ++i) {
            glUniform4fv(colorHandler, 1, &faceColors[i]);
            glUniform3fv(normalHandler, 1, glm::value_ptr(normals[i]));
            glUniform1f(noiseScaleHandler,pow(3.0, i-1));
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                           reinterpret_cast<const void *>(3 * i * sizeof(GLuint)));

        }

        glDisableVertexAttribArray(positionAttributeHandle);
        glDisableVertexAttribArray(texcoordAttributeHandle);
        granite->deactivate();
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Tetrahedron::printGLString(const char *msg, GLenum gle) {
        __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), msg, gle);
    }

    bool Tetrahedron::addMaterials() {
        const char vertex_shader[] = "shaders/turbulence/vertex_texture_2D.glsl";
        const char fragment_shader[] = "shaders/grains/fragment.glsl";
        granite = Material::makeMaterial(vertex_shader, fragment_shader);
        if(nullptr != granite )
        {
            textureDataHandler = PerlinNoiseGenerator::get2DTexture(32);
            return textureDataHandler>0;
        }
    }


}
