//
// Created by nathan on 16/04/20.
//

#include "Dodecahedron.h"
#include <vector>
#include <tuple>

static constexpr GLfloat faceColors[] ={
        0.941176f, 0.9019607f, 0.549019f,
        1.0, 1.0, 0.0, 0.7, 1.0, 0.3,
        1.0f, 0.184313f, 0.678431f
};
Polyhedrons::Dodecahedron::Dodecahedron():Polyhedrons::Polyhedron(), LogTag("DODECAHEDRON"){
    monochrome = nullptr;
    vbo =0;
    ibo[0] =0;
}

Polyhedrons::Dodecahedron::~Dodecahedron(){
    destroy();
}


void Polyhedrons::Dodecahedron::update(long time) {

    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    activeTransform.scale(1.4f);
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.8, 1.0, -1.0)) ;
}

void Polyhedrons::Dodecahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {
    monochrome->activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLuint positionAttr = monochrome->getAttrib("a_Position");
    glVertexAttribPointer(positionAttr, 3, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          0, 0);
    glEnableVertexAttribArray(positionAttr);


    //draw triangles
    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;
    glUniformMatrix4fv(monochrome->getUniform("u_MVPMatrix"), 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(monochrome->getUniform("u_mvMat"), 1, false, glm::value_ptr(viewT));
    glUniform3fv(monochrome->getUniform("u_LightPos"), 1, glm::value_ptr(lightPos));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    GLuint normHandle = monochrome->getUniform("u_FaceNormal");
    GLuint colorHandle = monochrome->getUniform("u_Color");
    for(int i=0; i<12; ++i) {
        glUniform3fv(normHandle, 1, glm::value_ptr(vertexNormals[i]));
        glUniform4f(colorHandle, faceColors[3*(i%4)], faceColors[3*(i%4)+1], faceColors[3*(i%4)+2], 1.0f);
        glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(5 * i * sizeof(GLuint)));
    }

    glDisableVertexAttribArray(positionAttr);
    monochrome->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool Polyhedrons::Dodecahedron::initVertices()  {
    double phiRadians  = glm::radians(360.0/5.0); //1.618; /* the phi angle needed for generation. The golden ratio */
    double thetaRadians  = 0.5*thetaRadians;

    double reciproPhi = 1.0 / sqrt(phiRadians);
    double delta = phiRadians;

    phiRadians = 1.618;
    reciproPhi = 1.0 / phiRadians;
    //green
    vertices[0]=glm::vec3(0, -phiRadians, -reciproPhi);
    vertices[1]=glm::vec3(0, +phiRadians, -reciproPhi);
    vertices[2]=glm::vec3(0, -phiRadians, +reciproPhi);
    vertices[3]=glm::vec3(0, +phiRadians, +reciproPhi);
//blue
    vertices[4]=glm::vec3(-reciproPhi, 0, -phiRadians);
    vertices[5]=glm::vec3(-reciproPhi, 0, +phiRadians);
    vertices[6]=glm::vec3(+reciproPhi, 0, -phiRadians);
    vertices[7]=glm::vec3(+reciproPhi, 0, +phiRadians);
    //red
    vertices[8]=glm::vec3(-phiRadians, -reciproPhi,  0);
    vertices[9]=glm::vec3(+phiRadians, -reciproPhi, 0);
    vertices[10]=glm::vec3(-phiRadians, +reciproPhi, 0);
    vertices[11]=glm::vec3(+phiRadians, +reciproPhi, 0);

    //orange
    vertices[12]=glm::vec3(-1, -1, -1);
    vertices[13]=glm::vec3(-1, -1, +1);
    vertices[14]=glm::vec3(-1, +1, -1);
    vertices[15]=glm::vec3(-1, +1, +1);
    vertices[16]=glm::vec3(+1, -1, -1);
    vertices[17]=glm::vec3(+1, -1, +1);
    vertices[18]=glm::vec3(+1, +1, -1);
    vertices[19]=glm::vec3(+1, +1, +1);

    return true;
}


bool Polyhedrons::Dodecahedron::initFaces()  {

    vertexNormals[0] = glm::normalize(glm::cross(vertices[17]-vertices[7],
                                                     vertices[2]-vertices[7]));
    vertexNormals[1] = glm::normalize(glm::cross(vertices[11]-vertices[19],
                                                 vertices[9]-vertices[19]));
    vertexNormals[2] = glm::normalize(glm::cross(vertices[16]-vertices[9],
                                                 vertices[0]-vertices[9]));
    vertexNormals[3] = glm::normalize(glm::cross(vertices[2]-vertices[13],
                                                 vertices[0]-vertices[13]));
    vertexNormals[4] = glm::normalize(glm::cross(vertices[19]-vertices[7],
                                                 vertices[3]-vertices[7]));
    vertexNormals[5] = glm::normalize(glm::cross(vertices[15]-vertices[5],
                                                 vertices[5]-vertices[5]));
    vertexNormals[6] = glm::normalize(glm::cross(vertices[3]-vertices[19],
                                                 vertices[1]-vertices[19]));
    vertexNormals[7] = glm::normalize(glm::cross(vertices[14]-vertices[10],
                                                 vertices[1]-vertices[10]));
    vertexNormals[8] = glm::normalize(glm::cross(vertices[6]-vertices[16],
                                                 vertices[4]-vertices[16]));
    vertexNormals[9] = glm::normalize(glm::cross(vertices[18]-vertices[11],
                                                 vertices[6]-vertices[11]));
    vertexNormals[10] = glm::normalize(glm::cross(vertices[12]-vertices[8],
                                                 vertices[4]-vertices[8]));
    vertexNormals[11] = glm::normalize(glm::cross(vertices[18]-vertices[6],
                                                 vertices[1]-vertices[6]));

    return true;
}

bool Polyhedrons::Dodecahedron::initBuffers() {
    int dataSize = NUM_VERTICES*VERTEX_DATA_SIZE;
    GLfloat* vertexBuffer = new float[dataSize];
    bool result = false;
    //GLuint triangleFanIndicesBuffer[]= {0,1,2,3,4,5,1,11,10,9,8,7,6,10};
    GLuint triangleStripIndexBuffer[]= {
            7,17,2,13,5,
            19,11,9,17,7,
            9,16,0,2,17,
            13,2,0,12,8,
            5,15,3,19,7,
            13,8,10,15,5,
            19,3,1,18,11,
            10,14,1,3,15,
            16,6,4,12,0,
            11,18,6,16,9,
            8,12,4,14,10,
            6,18,1,14,4
            };


    //GLuint wireFrameTopBottomLinesIndexBuffer[]=
     //       {0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10};

    for(int i=0; i<NUM_VERTICES ; ++i){
        int j = i*VERTEX_DATA_SIZE;
        vertexBuffer[j] = vertices[i].x;
        vertexBuffer[j+1] = vertices[i].y;
        vertexBuffer[j+2] = vertices[i].z;
        /*vertexBuffer[i+3] = this.vertexNormals[j].x;
        vertexBuffer[i+4] = this.vertexNormals[j].y;
        vertexBuffer[i+5] = this.vertexNormals[j].z;*/
    }
    glGenBuffers(1, &vbo);
    glGenBuffers(1, ibo);

    if (vbo > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleFanIndicesBuffer),
                     triangleFanIndicesBuffer, GL_STATIC_DRAW);
*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleStripIndexBuffer),
                     triangleStripIndexBuffer, GL_STATIC_DRAW);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireFrameTopBottomLinesIndexBuffer),
                     wireFrameTopBottomLinesIndexBuffer, GL_STATIC_DRAW);
*/
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        result = true;
        delete[] vertexBuffer;
    }
    return result;
}


void Polyhedrons::Dodecahedron::destroy() {
    if(monochrome)
        delete monochrome;
}

bool Polyhedrons::Dodecahedron::initShaders() {
    const char vertex_shader[] = "shaders/vertex/monochrome_face_vertex.glsl";
    const char fragment_shader[] = "shaders/fragment/monochrome_face_fragment.glsl";
    monochrome = Material::makeMaterial(vertex_shader, fragment_shader);
    return (nullptr != monochrome);
}

bool Polyhedrons::Dodecahedron::addMaterials(){};

bool Polyhedrons::Dodecahedron::init() {
    return (initVertices() && initFaces() &&  initBuffers() && initShaders());
}




