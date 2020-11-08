//
// Created by Erez on 28/08/20.
//

#include "Cylinder.h"

Cylinder::~Cylinder(){
    destroy();
}

Cylinder::Cylinder(){

    m_center =glm::vec3(0);
    m_radius =1.0;
    m_height =2.0;
    m_nSides =4;
    m_nRings = 10;
    nVertices = m_nSides*m_nRings+2;
}

Cylinder::Cylinder(const glm::vec3& center, float radius, float height, unsigned int nSides, unsigned int nRings)
:m_center(center), m_radius(radius), m_height(height), m_nSides(nSides), m_nRings(nRings){
    nVertices = m_nSides*m_nRings+2;
}

bool Cylinder::init(){
    return (initVertices() && initFaces() &&  initBuffers() && initShaders());
}
bool Cylinder::initVertices(){
    vertices = new glm::vec3[nVertices];
    normals = new glm::vec3[nVertices];

    vertices[0] = m_center;
    normals[0] = glm::vec3(0,-1,0);

    vertices[nVertices-1] = m_center+glm::vec3(0, m_height, 0);
    normals[nVertices-1] = glm::vec3(0,+1,0);
    double theta = M_2_PI / m_nSides;
    double  step = m_height/m_nRings;
    //double angle =0, double h=0
    /*for(int r=0; counter=0; r<= nVertices-1; r+=step){
        for(int s=0; s<= m_nSides; ++s, angle+=theta){
            vertices[1+r*m_nSides+s] = glm::vec3(m_radius*cos(angle), h, m_radius*sin(angle));
        }
    }*/
    return true;

}

bool Cylinder::initFaces(){
    return true;
}

bool Cylinder::initBuffers(){


    glGenBuffers(1, &vbo);
    glGenBuffers(3, ibo);

    GLuint* topFan = new GLuint[1+m_nSides];
    GLuint* bottomFan = new GLuint[1+m_nSides];
    GLuint* circumface = new GLuint[nVertices];
    topFan[0] = 0;
    bottomFan[0] = m_nSides*m_nRings +1;
    GLuint counter = 1;
    for(int k=1; k<= m_nSides; ++k){
        topFan[k] = counter;
        bottomFan[k] = counter+m_nSides*(m_nRings -1);
    }

    for(int r=0, counter=0; r<= m_nRings; ++r, ++counter){
        for(int s=0; s<= m_nSides; ++s){
            circumface[counter] = r*m_nSides+s;
        }
    }

    GLfloat* vertexDataBuffer = new GLfloat[(nVertices)*VERTEX_DATA_SIZE];
    for(int k=0; k<VERTEX_DATA_SIZE*nVertices; k=k+VERTEX_DATA_SIZE){
        vertexDataBuffer[k] = vertices[k].x;
        vertexDataBuffer[k+1] = vertices[k].y;
        vertexDataBuffer[k+2] = vertices[k].z;
        vertexDataBuffer[k+3] = normals[k].x;
        vertexDataBuffer[k+4] = normals[k].y;
        vertexDataBuffer[k+5] = normals[k].z;
    }
    if (vbo>0 && ibo[0]>0 && ibo[1]>0 && ibo[2]>0 ) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_DATA_SIZE*nVertices*sizeof(GLfloat),  vertexDataBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (1+m_nSides)*sizeof(GLuint ),
                     topFan, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (1+m_nSides)*sizeof(GLuint ),
                     bottomFan, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_nSides*m_nRings)*sizeof(GLuint ),
                     bottomFan, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    return true;
}

bool Cylinder::addMaterials(){return true;}

void Cylinder::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos){}
void Cylinder::destroy(){}
void Cylinder::update(long time){} // call this to update the model's state before rendering
//virtual void postUpdate() = 0; // call this for post rendering update.
bool Cylinder::initShaders(){return true;}