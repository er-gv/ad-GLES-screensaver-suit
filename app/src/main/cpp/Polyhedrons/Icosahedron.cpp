//

// Created by nathan on 16/04/20.
//

#include <vector>
#include <tuple>

#include "Icosahedron.h"

static constexpr GLfloat faceColors[] ={
        0.941176f, 0.9019607f, 0.549019f,
        1.0, 1.0, 0.0, 0.7, 1.0, 0.3,
        0.894501f, 0.444444f, 0.839215f
};
Polyhedrons::Icosahedron::Icosahedron() :
    Polyhedron(), LogTag("ICOSAHEDRON"){
    monochrome = nullptr;
}

Polyhedrons::Icosahedron::~Icosahedron() {
    destroy();
}

void Polyhedrons::Icosahedron::update(long time) {

    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    activeTransform.scale(3.4f);
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 0.0)) ;
}

void Polyhedrons::Icosahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {
    monochrome->activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLuint positionAttr = monochrome->getAttrib("a_Position");
    glVertexAttribPointer(positionAttr, 3, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          0, 0);
    glEnableVertexAttribArray(positionAttr);


    //draw triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;
    glUniformMatrix4fv(monochrome->getUniform("u_MVPMatrix"), 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(monochrome->getUniform("u_mvMat"), 1, false, glm::value_ptr(viewT));
    glUniform3fv(monochrome->getUniform("u_LightPos"), 1, glm::value_ptr(lightPos));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glUniform4f(monochrome->getUniform("u_Color"), faceColors[0], faceColors[1], faceColors[2], 1.0f);
    for(int i=0; i<5; ++i) {
        glUniform3fv(monochrome->getUniform("u_FaceNormal"), 1, glm::value_ptr(vertexNormals[i]));
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(3 * i * sizeof(GLuint)));
    }

    for(int i=0; i<10; ++i) {
        glUniform3fv(monochrome->getUniform("u_FaceNormal"), 1, glm::value_ptr(vertexNormals[5+i]));
        glUniform4f(monochrome->getUniform("u_Color"), faceColors[3*(i%2)+3], faceColors[3*(i%2)+4], faceColors[3*(i%2)+5], 1.0f);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>((15+(3 * i) )* sizeof(GLuint)));
    }

    glUniform4f(monochrome->getUniform("u_Color"), faceColors[9], faceColors[10], faceColors[11], 1.0f);
    for(int i=0; i<5; ++i) {
        glUniform3fv(monochrome->getUniform("u_FaceNormal"), 1, glm::value_ptr(vertexNormals[15+i]));
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>((45+(3 * i))* sizeof(GLuint)));
    }

    glDisableVertexAttribArray(positionAttr);
    monochrome->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

	bool Polyhedrons::Icosahedron::initVertices()  {
		float phiAngles  = 26.56505f; /* the phi angle needed for generation */
        float phiRadians = glm::radians(phiAngles); /* 2 sets of four points */

        float r = 1.0f; /* any radius in which the polyhedron is inscribed */
        /* first vertex */
        float deltaTheta = 72.0f; //360/5 = 72 angles.
        float theta = 54.0f; //half angle
        float product_r_cosPhi = r * cos(phiRadians);
        float product_r_sinPhi = r * sin(phiRadians);

        for(int i=1; i<6; i++){
            vertices[i].x = cos(glm::radians(theta))*product_r_cosPhi;
            vertices[i].z = sin(glm::radians(theta))*product_r_cosPhi;
            vertices[i].y = product_r_sinPhi;
            theta += deltaTheta;
        }
        //
        //product_r_cosPhi = r * com.ergv.glScreenSavers.util. (float) Math.cos(-phiRadians);
        product_r_sinPhi = -product_r_sinPhi;
        theta = 1.5*54.0f;
        for(int i=6; i<11; i++){
            vertices[i].x = cos(glm::radians(theta))*product_r_cosPhi;
            vertices[i].z = sin(glm::radians(theta))*product_r_cosPhi;
            vertices[i].y = product_r_sinPhi;
            theta += deltaTheta;
        }
        //set top and bottom vertices
        vertices[0].x  =  0.0f;
        vertices[0].y  =  r;
        vertices[0].z  =  0.0f;
        vertices[11].x =  0.0f;
        vertices[11].y = -r;
        vertices[11].z =  0.0f;
        return true;
    }


bool Polyhedrons::Icosahedron::initFaces()  {
    std::vector<std::tuple<int, int, int>> facesIdx;
    facesIdx.push_back(std::make_tuple(0, 1, 2));
    facesIdx.push_back(std::make_tuple( 0, 2, 3 ));
    facesIdx.push_back(std::make_tuple( 0, 3, 4 ));
    facesIdx.push_back(std::make_tuple( 0, 4, 5 ));
    facesIdx.push_back(std::make_tuple( 0, 5, 1 ));
    facesIdx.push_back(std::make_tuple( 11, 6, 7 ));
    facesIdx.push_back(std::make_tuple( 11, 7, 8 ));
    facesIdx.push_back(std::make_tuple( 11, 8, 9 ));
    facesIdx.push_back(std::make_tuple( 11, 9, 10 ));
    facesIdx.push_back(std::make_tuple( 11, 10, 6 ));
    facesIdx.push_back(std::make_tuple( 1, 2, 6 ));
    facesIdx.push_back(std::make_tuple( 2, 3, 7 ));
    facesIdx.push_back(std::make_tuple( 3, 4, 8 ));
    facesIdx.push_back(std::make_tuple( 4, 5, 9 ));
    facesIdx.push_back(std::make_tuple( 5, 1, 10 ));
    facesIdx.push_back(std::make_tuple( 6, 7, 2 ));
    facesIdx.push_back(std::make_tuple( 7, 8, 3 ));
    facesIdx.push_back(std::make_tuple( 8, 9, 4 ));
    facesIdx.push_back(std::make_tuple( 9, 10, 5 ));
    facesIdx.push_back(std::make_tuple( 10, 6, 1 ));

    for(int k=0; k<NUM_FACES; ++k){
        int a = std::get<0>(facesIdx[k]);
        int b = std::get<1>(facesIdx[k]);
        int c = std::get<2>(facesIdx[k]);
        vertexNormals[k] = glm::normalize(glm::cross(vertices[c]-vertices[a],
                                               vertices[b]-vertices[a]));
    }
    return true;
}

bool Polyhedrons::Icosahedron::initBuffers() {
    int dataSize = NUM_VERTICES*VERTEX_DATA_SIZE;
    GLfloat* vertexBuffer = new float[dataSize];
    bool result = false;
    GLuint triangleFanIndicesBuffer[]= {0,1,2,3,4,5,1,11,10,9,8,7,6,10};
    GLuint triangleStripIndexBuffer[]= {
            0,1,2,            0,2,3,            0,3,4,
            0,4,5,            0,5,1,

            6,1,10, 1,6,2,  7,2,6, 2,7,3, 8,3,7,
            3,8,4,  9,4,8,  4,9,5, 10,5,9, 5,10,1,

            11, 10,9,            11, 9, 8,            11, 8, 7,
            11, 7, 6,            11, 6, 10};


    GLuint wireFrameTopBottomLinesIndexBuffer[]=
            {0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10};

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
    glGenBuffers(3, ibo);

    if (vbo > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleFanIndicesBuffer),
                     triangleFanIndicesBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleStripIndexBuffer),
                     triangleStripIndexBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireFrameTopBottomLinesIndexBuffer),
                     wireFrameTopBottomLinesIndexBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        result = true;
        delete[] vertexBuffer;
    }
    return result;
}


void Polyhedrons::Icosahedron::destroy() {
    if(monochrome)
        delete monochrome;
}

bool Polyhedrons::Icosahedron::initShaders() {
    const char vertex_shader[] = "shaders/vertex/monochrome_face_vertex.glsl";
    const char fragment_shader[] = "shaders/fragment/monochrome_face_fragment.glsl";
    monochrome = Material::makeMaterial(vertex_shader, fragment_shader);
    return (nullptr != monochrome);
}
    
bool Polyhedrons::Icosahedron::addMaterials(){};

bool Polyhedrons::Icosahedron::init() {
    return (initVertices() && initFaces() &&  initBuffers() && initShaders());
}



