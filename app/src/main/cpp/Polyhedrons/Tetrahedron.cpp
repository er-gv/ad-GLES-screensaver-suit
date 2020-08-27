//
// Created by nathan on 16/04/20.
//

#include <android/log.h>
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
        int theta = 60;
        for(int i=1; i<4; i++){
            double  rads = glm::radians(float(theta));
            vertices[i] = glm::vec3(cos(rads), 0.0, sin(rads));
            theta+=120;
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
            const char vertex_shader[] = "shaders/vertex/monochrome_face_vertex.glsl";
            const char fragment_shader[] = "shaders/fragment/monochrome_face_fragment.glsl";
            monochrome = Material::makeMaterial(vertex_shader, fragment_shader);

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
        normals = new glm::vec3[nFaces];
        if(nullptr == normals)
            return false;
        /*faces = new Polyhedron::Polygon*[nFaces];

         faces[0] = new Polygon(*this, (int[]) {1, 2, 0}, 3);
        faces[1] = new Polygon(*this, (int[]) {2, 3, 0}, 3);
        faces[2] = new Polygon(*this, (int[]) {3, 1, 0}, 3);
        faces[3] = new Polygon(*this, (int[]) {3, 1, 2}, 3);

        size_t sizeof_float = sizeof(float);
        size_t offset = 3*sizeof_float;
        glm::vec3 v1 = vertices[1]-vertices[0];
        glm::vec3 v2 = vertices[2]-vertices[0];*/
        normals[0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[2]-vertices[0]));
        normals[1] = glm::normalize(glm::cross(vertices[2]-vertices[0], vertices[3]-vertices[0]));
        normals[2] = glm::normalize(glm::cross(vertices[3]-vertices[0], vertices[1]-vertices[0]));
        normals[3] = glm::normalize(glm::cross(vertices[1]-vertices[2], vertices[3]-vertices[2]));
        /*for( int k=0; k<nFaces; k++){
            const glm::vec3& normal = faces[k]->getNormal();
            int j = 3*k;
            glm::vec3 n = glm::normalize(glm::vec3 const&a, glm::vec3 const& b , glm::vec3 const& c){
                return glm::normalize(glm::cross(c-a, b-a));
            }

        }*/
        return true;
    }

    bool Tetrahedron::initBuffers() {
        vbo = new GLuint[1];
        ibo = new GLuint[1];
        glGenBuffers(1, vbo);
        glGenBuffers(1, ibo);

        GLuint constexpr trianglesIndexBuffer[] ={1,2,0, 2,3,0, 3,1,0, 3,2,1};

        vertexDataBuffer = new GLfloat[4*3];
        char buffer[100];
        for(int i=0; i< nVertices; i++){
            sprintf(buffer, "nVert[%d] = (%f, %f, %f).", i, vertices[i].x, vertices[i].y, vertices[i].z);
            __android_log_print(ANDROID_LOG_INFO, Tetrahedron::LogTag.c_str(), "%s",
                                buffer);

            vertexDataBuffer[3*i] = vertices[i].x;
            vertexDataBuffer[3*i+1] = vertices[i].y;
            vertexDataBuffer[3*i+2] = vertices[i].z;
        }
        if (vbo[0]>0 && ibo[0]>0 ) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat),  vertexDataBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesIndexBuffer),
                         trianglesIndexBuffer, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //TODO at this point we can delete vertexDataBuffer.
        return true;
    }

    void Tetrahedron::destroy() {
        //TODO delete unnecessary memory
        glDeleteBuffers(1, vbo);
        glDeleteBuffers(1, ibo);
    }

    //TODO transform/projection/pre-render state update
    void Tetrahedron::update(long time) {

        float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

        activeTransform.setTransform(initialTransform.get());
        activeTransform.scale(2.4f);
        activeTransform.translate(glm::vec3(-2.0, 0.0, 0.0));
        activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 0.0)) ;
        //activeTransform.rotate(glm::radians(40.0), glm::vec3(1.0, 0.0, 0.0)) ;

    }

    //TODO bugfix
    void Tetrahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {

        monochrome->activate();
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        GLuint positionAttr = monochrome->getAttrib("a_Position");
        glVertexAttribPointer(positionAttr, 3, GL_FLOAT,
                              static_cast<GLboolean>(false),
                              0, 0);
        glEnableVertexAttribArray(positionAttr);


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
        glm::mat4 viewT = viewMat*activeTransform.get();
        glm::mat4 projT = projectionMat*viewT;
        glUniformMatrix4fv(monochrome->getUniform("u_MVPMatrix"), 1, false, glm::value_ptr(projT));
        glUniformMatrix4fv(monochrome->getUniform("u_mvMat"), 1, false, glm::value_ptr(viewT));

        glUniform3fv(monochrome->getUniform("u_LightPos"), 1, glm::value_ptr(lightPos));


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        for(int i=0; i<4; ++i) {
            glUniform3fv(monochrome->getUniform("u_FaceNormal"), 1,glm::value_ptr(normals[i]));
            glUniform4f(monochrome->getUniform("u_Color"), faceColors[3*i], faceColors[3*i+1], faceColors[3*i+2], 1.0f);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                           reinterpret_cast<const void *>(3 * i * sizeof(GLuint)));
        }

        glDisableVertexAttribArray(positionAttr);
        monochrome->deactivate();
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
