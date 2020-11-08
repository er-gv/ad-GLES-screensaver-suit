//
// Created by nathan on 28/08/20.
//

//
// Created by Erez on 19/09/20.
//

#include "Cone.h"
static const GLfloat CUBE_NORMAL_DATA[] = {

        0.0f,  0.0f, +1.0f,
        +1.0f,  0.0f,  0.0f,
        0.0f,  0.0f, -1.0f,
        -1.0f,  0.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f, -1.0f,  0.0f
};

// X, Y, Z
// The normal is used in light calculations and is a vector which points
// orthogonal to the plane of the surface. For a cube model, the normals
// should be orthogonal to the points of each face.

static const GLfloat mortarColor[][3] {

        {0.359375f, 0.26953125f, 0.05078125f},
        {0.050f, 0.368f, 0.647f},
        {0.988f, 0.870f, 0.909f},
        {0.007f, 0.588f, 0.635f},
        {0.007f, 0.635f, 0.49f}
};

static const GLfloat brickColor[][3] {
        {0.952f, 0.6901f, 0.188f},
        {0.75390625f, 0.53828125f, 0.69921875f},

        {0.557f, 0.094f, 0.129f},
        {0.635f, 0.305f, 0.007f},
        {0.788f, 0.968f, 0.0392f}
};

static const GLfloat brickSize[][2]{
        {0.3f, 0.15f},
        {0.7, 0.65f},
        {0.3f, 0.15f},
        {0.3f, 0.15f},
        {0.3f, 0.15f}
};

static const GLfloat brickPct[][2] {
        {0.9f, 0.85},
        {0.9, 0.85},
        {0.9f, 0.85},
        {0.9f, 0.85},
        {0.9f, 0.85}

};



Cone::Cone(float rad, float h, int sides, int idx): HardCodedModel(){
    m_idx = idx;
    setup(rad, h, sides);
}

Cone::Cone(const glm::mat4& transform, float rad, float h, int sides, int idx):
        HardCodedModel(transform), m_idx(idx % 5){
    setup(rad, h, sides);
}

Cone::~Cone(){
    destroy();
}

void Cone::setup(float rad, float h, int sides){
    radius = rad;
    height = h;
    nSides = sides;
    nVertices = 2+nSides;
}

void Cone::destroy(){
    glDeleteBuffers(1, vbo);
    glDeleteBuffers(2, ibo);
}


bool Cone::init(){
    glGenBuffers(1, vbo);
    glGenBuffers(2, ibo);
    if(0==vbo[0]||0==ibo[0]||0==ibo[1])
        return false;
    return initVertices()&&initFaces()&&initBuffers()&&addMaterials();
}

bool Cone::initVertices(){
    unsigned int bufferSize = nVertices*VERTEX_DATA_SIZE;
    GLfloat vertexBuffer[bufferSize];

    //vertex at the middle of bottom circle
    vertexBuffer[0] =0;
    vertexBuffer[1] =0;
    vertexBuffer[2] =0;
    vertexBuffer[3] =1;

    //vertex at the tip of the cone
    unsigned int topIndexOffset = (nVertices-1)*VERTEX_DATA_SIZE;
    vertexBuffer[topIndexOffset+0] =0;
    vertexBuffer[topIndexOffset+1] =height;
    vertexBuffer[topIndexOffset+2] =0;
    vertexBuffer[topIndexOffset+3] =1;

    //all vertices circumfacing the bottom of the cone
    double theta = 0.0;
    double deltaTheta = 2.0*M_PI / nSides;
    unsigned k = VERTEX_DATA_SIZE;
    for(unsigned s=1; s<=nSides; ++s){

        vertexBuffer[k] = radius*sin(theta);
        vertexBuffer[k+1] = 0.0;
        vertexBuffer[k+2] = radius*cos(theta);
        vertexBuffer[k+3] = 1.0;
        k+=VERTEX_DATA_SIZE;
        theta+=deltaTheta;
    }

    //init the faceNormals vector;
    glm::vec3 tip(0.0, height,0.0);
    k = VERTEX_DATA_SIZE;
    unsigned v_idx;
    unsigned u_idx;
    for(unsigned s=1; s<nSides; ++s){
        v_idx = k*s;
        u_idx = (k+1)*s;
        glm::vec3 v(vertexBuffer[v_idx], vertexBuffer[v_idx+1],vertexBuffer[v_idx+2]);
        glm::vec3 u(vertexBuffer[u_idx], vertexBuffer[u_idx+1],vertexBuffer[u_idx+2]);
        faceNormals.push_back(glm::cross(v-tip,u-tip));
    }
    v_idx = k*nSides;
    u_idx = k;
    glm::vec3 v(vertexBuffer[v_idx], vertexBuffer[v_idx+1],vertexBuffer[v_idx+2]);
    glm::vec3 u(vertexBuffer[u_idx], vertexBuffer[u_idx+1],vertexBuffer[u_idx+2]);
    faceNormals.push_back(glm::cross(v-tip,u-tip));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertices*VERTEX_DATA_BYTESIZE, vertexBuffer, GL_STATIC_DRAW);
    return true;
}

bool Cone::initFaces(){

    unsigned int nBodyIndices = 3*nSides;
    unsigned int nLidIndices = 2+nSides;
    GLuint bodyIndices[nBodyIndices];
    GLuint lidIndices[nLidIndices];

    lidIndices[0] = 0;
    for(int i=1; i<=nSides; ++i){
        lidIndices[i]=i;
    }
    lidIndices[nLidIndices-1]=1;

    int k;
    for(int i=1; i<nSides; ++i){
        k = 3*i;
        bodyIndices[k] = i;
        bodyIndices[k+1] = nSides+1;
        bodyIndices[k+2] = i+1;
    }
    k= nSides*3;
    bodyIndices[k] = nSides;
    bodyIndices[k+1] = nSides+1;
    bodyIndices[k+2] = 1;


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nBodyIndices*sizeof(GLuint), bodyIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nLidIndices*sizeof(GLuint), lidIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

bool Cone::initBuffers(){
    return true;
}

bool Cone::addMaterials(){
    const char *vertexSrc = "shaders/bricks/gouraund/vertex.glsl";
    const char *fragmentSrc ="shaders/bricks/gouraund/fragment.glsl";
    bool result = false;
    material = Material::makeMaterial(vertexSrc, fragmentSrc);
    if(material) {

        positionAttributeHandle = material->getAttrib("MCvertex");

        mNormalHandle = material->getUniform("u_MCnormal");
        mBrickColorHandle = material->getUniform( "u_BrickColor");
        mMortarColorHandle = material->getUniform( "u_MortarColor");
        mBrickSizeHandle = material->getUniform( "u_BrickSize");
        mBrickPctHandle = material->getUniform( "u_BrickPct");
        mLightPosHandle = material->getUniform("u_LightPosition");
        mMVPMatrixHandle = material->getUniform("u_MVPMatrix");
        mMVMatrixHandle = material->getUniform("u_MVMatrix");


        result = true;
    }
    return result;

}

Cone& Cone::operator=(const Cone& m){
    return *this;
}


void Cone::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos){

    material->activate();
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glEnableVertexAttribArray(positionAttributeHandle);
    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(POSITION_OFFSET));

    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;


    //draw triangles
    glUniformMatrix4fv(mMVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(mMVMatrixHandle, 1, false, glm::value_ptr(viewT));
    //glUniformMatrix4fv(mNormalMatrixHandle, 1, false, glm::value_ptr(viewMat));

    glUniform3fv(mLightPosHandle, 1, glm::value_ptr(lightPos));
    /*glUniform1f(sunSurface->getUniform("u_diffuseCoaff"), 0.1);
    glUniform1f(sunSurface->getUniform("u_specularCoaff"), 0.9);
    glUniform1f(sunSurface->getUniform("u_shininess"), 5.0);
    glUniform1f(sunSurface->getUniform("u_ambiantCoaff"), 0.2);*/


    glUniform3fv(mBrickColorHandle, 1, brickColor[m_idx]);
    glUniform3fv(mMortarColorHandle, 1, mortarColor[m_idx]);
    glUniform2fv(mBrickSizeHandle, 1, brickSize[m_idx]);
    glUniform2fv( mBrickPctHandle, 1, brickPct[m_idx]);

    renderBody(viewT);
    if(!isOpen) {
        renderLids(viewT);
    }

    glDisableVertexAttribArray(positionAttributeHandle);
    //glDisableVertexAttribArray(colorAttributeHandle);
    //glDisableVertexAttribArray(normalAttributeHandle);
    //glBindTexture(GL_TEXTURE_3D, 0);

    material->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void Cone::update(long time){
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    //activeTransform.scale(1.4f);
    activeTransform.rotate(glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0)) ;
    //activeTransform.rotate(glm::radians(125.0), glm::vec3(0.0, 0.0, 0.0)) ;
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(-1.0, 1.0, 0.0)) ;


}


void Cone::renderBody(glm::mat4& viewMat) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    for(int k=0; k<nSides; k++) {
        glUniform3fv(mNormalHandle, 1, glm::value_ptr(glm::vec3(viewMat*glm::vec4(faceNormals[k], 0.0))));
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>((3 * k) * sizeof(GLuint)));
    }
}

void Cone::renderLids(glm::mat4& viewMat) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glUniform3fv(mNormalHandle, 1, glm::value_ptr(glm::vec3(viewMat*glm::vec4(0.0, -1.0, 0.0,0.0))));
    glDrawElements(GL_TRIANGLE_FAN, nSides + 2, GL_UNSIGNED_INT, 0);
}

