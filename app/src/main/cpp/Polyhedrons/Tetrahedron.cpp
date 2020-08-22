//
// Created by nathan on 16/04/20.
//
#include "graphics/Material.h"
#include "Tetrahedron.h"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

        float phiAngles = -19.471220333f; /* the phi angle needed for generation */

        float r = 1.0f; /* any radius in which the polyhedron is inscribed */
        /* first vertex */
        float phiRadians = glm::radians(phiAngles);
        float deltaTheta = glm::radians(120.0f);
        vertices[0] = glm::vec3(0.0f, 0.0f, r);
        float theta = 0.0f;
        float rCosPhi = r * cos(phiRadians);
        float rMultsinPhi = r * sin(phiRadians);
        for (int i = 1; i < 4; i++) {
            vertices[i] = glm::vec3(cos(theta * 2) * rCosPhi, rMultsinPhi,
                                    sin(theta * 2) * rCosPhi);
            theta += deltaTheta;
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

    bool Tetrahedron::initShaders() {

        {
            const char vertex_shader[] = "shaders/vertex/point_vertex_shader.glsl";
            const char fragment_shader[] = "shaders/fragment/monochrome_face_fragment.glsl";
            monochrome = Material::makeMaterial(
                    vertex_shader, fragment_shader,
                    (const char*[]){"a_Position"}, 1,
                    (const char*[]){"u_MVPMatrix", "u_mvMat", "u_LightPos", "u_FaceNormal"}, 4);

            /*monochromePositionHandle = glGetAttribLocation(monochromeProgram, "a_Position");
            monochromeMVPHandle = glGetUniformLocation(monochromeProgram, "u_MVPMatrix");
            monochromeMVHandle = glGetUniformLocation(monochromeProgram, "u_mvMat");
            monochromeLightPosHandle = glGetUniformLocation(monochromeProgram, "u_LightPos");
            monochromeFaceNormHandle = glGetUniformLocation(monochromeProgram, "u_FaceNormal")
            monochromeProgram = GLUtils::createProgram(&vertex_shader, &fragment_shader);*/

        }
        /*
        {
            const char *vertex_shader = GLUtils::openTextFile(
                    "shaders/vertex/point_vertex_shader.glsl");
            const char *fragment_shader = GLUtils::openTextFile(
                    "shaders/fragment/point_fragment_shader.glsl");
            wireframeProgram = GLUtils::createProgram(&vertex_shader, &fragment_shader);
        }
        {
            const char *vertex_shader = GLUtils::openTextFile(
                    "shaders/vertex/point_vertex_shader.glsl");
            const char *fragment_shader = GLUtils::openTextFile(
                    "shaders/fragment/noise_fs.glsl");
            cloudsProgram = GLUtils::createProgram(&vertex_shader, &fragment_shader);
        }
        if (wireframeProgram && monochromeProgram && cloudsProgram) {

            // Bind Attributes and uniforms for the tetrahedron's material
            monochromePositionHandle = glGetAttribLocation(monochromeProgram, "a_Position");
            monochromeMVPHandle = glGetUniformLocation(monochromeProgram, "u_MVPMatrix");
            monochromeMVHandle = glGetUniformLocation(monochromeProgram, "u_mvMat");
            monochromeLightPosHandle = glGetUniformLocation(monochromeProgram, "u_LightPos");
            monochromeFaceNormHandle = glGetUniformLocation(monochromeProgram, "u_FaceNormal");

            wireframePositionHandler = glGetAttribLocation(wireframeProgram, "a_Position");
            wireframeMVPHandler = glGetUniformLocation(wireframeProgram, "u_MVPMatrix");
            wireframeColorHandler = glGetUniformLocation(wireframeProgram, "u_Color");

            cloudsPositionHandle = glGetAttribLocation(cloudsProgram, "a_Position");
            cloudsSkyColorHandler = glGetUniformLocation(cloudsProgram, "u_skyColor");
            cloudsCloudColorHandle = glGetUniformLocation(cloudsProgram, "u_cloudColor");
            cloudsMVHandle = glGetUniformLocation(cloudsProgram, "u_mvMat");
            cloudsLightPosHandle = glGetUniformLocation(cloudsProgram, "u_LightPos");
            cloudsFaceNormHandle = glGetUniformLocation(cloudsProgram, "u_FaceNormal");
            return true;
        }*/
        return true;
    }

    bool Tetrahedron::initFaces() {
        char buffer[100];
        faces = new Polyhedron::Polygon*[nFaces];
        faces[0] = new Polygon(*this, (int[]) {0, 1, 2}, 3);
        faces[1] = new Polygon(*this, (int[]) {0, 2, 3}, 3);
        faces[2] = new Polygon(*this, (int[]) {0, 3, 1}, 3);
        faces[3] = new Polygon(*this, (int[]) {1, 2, 3}, 3);

        size_t sizeof_float = sizeof(float);
        size_t offset = 3*sizeof_float;
        for( int k=0; k<nFaces; k++){
            const glm::vec3& normal = faces[k]->getNormal();
            memcpy(normals+offset*k, glm::value_ptr(normal), offset);
            sprintf(buffer, "normal[%d] = (%f, %f, %f).", k,
                    normal.x, normal.y, normal.z);
                    __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s",
                                        buffer);

        }
        return true;
    }

    bool Tetrahedron::initBuffers() {
        vbo = new GLuint[1];
        ibo = new GLuint[2];
        glGenBuffers(1, vbo);
        glGenBuffers(2, ibo);
        short constexpr trianglesFanIndexBuffer[] ={0,1,2, 3,1};
        short constexpr trianglesIndexBuffer[] ={0,1,2, 3,1,0, 0,2,3, 3,2,1};
        short constexpr wireFrameLinesIndexBuffer[] = {0,1,0,2,0,3,1,2,1,3,2,3};

        vertexDataBuffer = new GLfloat[nVertices*VERTEX_DATA_SIZE_IN_ELEMENTS];
        char buffer[100];
        for(int i=0; i< nVertices; i++){
            sprintf(buffer, "nVert[%d] = (%f, %f, %f).", vertices[i].x, vertices[i].y, vertices[i].z);
            __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s",
                                buffer);

            float val[3];
            memcpy(val, glm::value_ptr(vertices[i]), sizeof(val));
            sprintf(buffer, "glm::vec3 copied to arr[3]: = (%f, %f, %f).", val[0], val[1,1], val[2]);
            __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s",
                                buffer);
            memcpy(& vertexDataBuffer[i*VERTEX_DATA_SIZE_IN_ELEMENTS], glm::value_ptr(vertices[i]), sizeof(glm::vec3));
        }
        if (vbo[0]>0 && ibo[0]>0 && ibo[1]>0) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataBuffer),  vertexDataBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesIndexBuffer),
                         trianglesIndexBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireFrameLinesIndexBuffer),
                         wireFrameLinesIndexBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //TODO at this point we can delete vertexDataBuffer.
        return true;
    }

    void Tetrahedron::destroy() {
        //TODO delete unnecessary memory
        glDeleteBuffers(1, vbo);
        glDeleteBuffers(2, ibo);
    }

    //TODO transform/projection/pre-render state update
    void Tetrahedron::update() {
        long time = GLUtils::currentTimeMillis() % 10000L;
        float angleInDegrees = (360.0f / 10000.0f) * ((int) time);
        rotate(angleInDegrees, vertices[0]);
    }

    //TODO bugfix
    void Tetrahedron::render(Camera& camera) {


        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

        GLuint positionAttr = monochrome->getAttrib("a_Position");
        glEnableVertexAttribArray(positionAttr);
        glVertexAttribPointer(positionAttr, POSITION_DATA_SIZE, GL_FLOAT,
                              static_cast<GLboolean>(false),
                              VERTEX_DATA_SIZE_IN_BYTES, 0);


         /*glVertexAttribPointer(texAttributeHandle, TEX_DATA_SIZE, GL_FLOAT, false, VERTEX_DATA_SIZE,
                               24);**/

        //pass in cloud and sky color.
        //int skyColorLoc = glGetUniformLocation(7, "u_skyColor");
        //int cloudColorLoc = glGetUniformLocation(7, "u_cloudColor");
        //glUniform3f(cloudColorLoc, 0.8f, 0.8f, 0.8f);
        //glUniform3f(skyColorLoc, 0.0f, 0.0f, 0.65f);



        // Set the active texture unit to texture unit 0.
        //GLES20.glActiveTexture(GLES20.GL_TEXTURE0);


        //draw triangles
        glBindBuffer(GL_ARRAY_BUFFER, ibo[0]);
        glm::mat4 projecrion = camera.projection();
        glm::mat4 view = camera.lookAt();
        glUniformMatrix4fv(monochrome->getUniform("u_MVPMatrix"), 1, false, glm::value_ptr(activeTransform*view*projecrion));
        glUniformMatrix4fv(monochrome->getUniform("u_mvMat"), 1, false, glm::value_ptr(activeTransform*view));

        glUniform3f(monochrome->getUniform("u_Color"), 0.0f, 0.2f, 0.65f);
        glUniform3f(monochrome->getUniform("u_LightPos"), 0.0f, 0.0f, 0.65f);
        glUniform1fv(monochrome->getUniform("u_FaceNormal"), 12, normals);



        glUniform3f(monochrome->getUniform("u_Color"), 1.0f, 1.2f, 1.6f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glDrawElements(GL_LINES, 12, GL_UNSIGNED_SHORT, 0);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0)
        //
        glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_SHORT, 0)
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

        //draw wireframe
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glDrawElements(GL_LINES, 12, GL_UNSIGNED_SHORT, 0);

        //unbind the buffers and the vertices attribs
        //glDisableVertexAttribArray(normalAttributeHandle);
        //glDisableVertexAttribArray(positionAttributeHandle);
        //glDisableVertexAttribArray(texAttributeHandle);
*/
        glDisableVertexAttribArray(positionAttr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Tetrahedron::printGLString(const char *msg, GLenum gle) {
        __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), msg, gle);
    }

    bool Tetrahedron::addMaterials() {
        return true;
    }


}
