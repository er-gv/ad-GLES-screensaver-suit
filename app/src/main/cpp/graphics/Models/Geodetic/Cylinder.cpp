//
// Created by Erez on 19/09/20.
//

#include "Cylinder.h"
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



Cylinder::Cylinder(float rad, float h, int sides, int idx): HardCodedModel(), m_idx(idx){
    setup(rad, h, sides);
}

Cylinder::Cylinder(const glm::mat4& transform, float rad, float h, int sides, int idx):
        HardCodedModel(transform), m_idx(idx)
{
    setup(rad, h, sides);
}

Cylinder::~Cylinder(){
    destroy();
}

void Cylinder::setup(float rad, float h, int sides){
    radius = rad;
    height = h;
    nSides = sides;
    nBodyVertices = 2*nSides;
    nLidVertices = 2*(nSides+1);
}


bool Cylinder::init(){
    glGenBuffers(2, vbo);
    glGenBuffers(2, ibo);
    if(0== vbo[0] || 0== vbo[1] || 0 == ibo[0]|| 0 == ibo[1]){
        return false;
    }
    return initVertices()&&initFaces()&&initBuffers()&&addMaterials();
}

void Cylinder::destroy(){
    glDeleteBuffers(2, vbo);
    glDeleteBuffers(2, ibo);
}

void Cylinder::update(long time){
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    //activeTransform.scale(1.4f);
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 1.0)) ;
}

bool Cylinder::initVertices(){
    return true;
}

bool Cylinder::initFaces(){
    return initBodyFaces() && initLidFaces();
}

/**
 * setup GL_ELEMENT_BUFFER for a GL_TRIANGLE_STRIP representing the Cylinder circumface.
 */
bool Cylinder::initBodyFaces(){
    int nBodyIndices = 2*(nSides+1);
    GLuint indices[nBodyIndices];
    for(int k=0; k<nSides; k+=2){
        indices[k]=k;
        indices[k+1]=k+1;
    }
    indices[2*nSides-2]=0;
    indices[2*nSides-1]=1;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nBodyIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return true;
}

/**
 * setup GL_ELEMENT_BUFFER for a GL_TRIANGLE_STRIP representing the Cylinder top and bottom circles.
 */
bool Cylinder::initLidFaces(){

    unsigned int indices_per_lid = nSides+2;
    //unsigned int first_bottom_idx = nSides+1;
    unsigned int nLidIndices = 2*indices_per_lid;
    unsigned int bottomLidOffset = indices_per_lid;

    GLuint indicesBuffer[nLidIndices];

    indicesBuffer[0]=0;
    indicesBuffer[bottomLidOffset]=nSides+1;

    for(unsigned int k=1; k<=nSides; k++){
        //computing the indices around the center of top and bottom lids.
        indicesBuffer[k]=k;
        indicesBuffer[bottomLidOffset+k]=nLidIndices-k;
    }
    indicesBuffer[nSides+1]=1;
    indicesBuffer[2*nSides+1]=2*nSides+1;


    //flush it to the GPU.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nLidIndices*sizeof(GLuint), indicesBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return true;
}

bool Cylinder::initBuffers(){
    return initBodyBuffers() && initLidBuffers();
}

/**
 * setup GL_ARRAY_BUFFER for the vertices of the Cylinder body.
 */
bool Cylinder::initBodyBuffers() {

    double circleStep = float((2.0f*M_PI*radius)/nSides);
    double around=0.0;
    unsigned int bufferSize = nBodyVertices*VERTEX_DATA_SIZE;
    GLfloat bodyVertices[bufferSize];
    unsigned int k=0;
    unsigned int two_vertices_size = VERTEX_DATA_SIZE*2;
    for(int s=0; s<nSides; ++s, k+=two_vertices_size){

        double sn = radius*sin(around);
        double cn = radius*cos(around);
        bodyVertices[k+0] = sn;
        bodyVertices[k+1] = height;
        bodyVertices[k+2] = cn;
        bodyVertices[k+3] = 1.0;
        bodyVertices[k+4] = sn;
        bodyVertices[k+5] = 0;
        bodyVertices[k+6] = cn;

        bodyVertices[k+7] = sn;
        bodyVertices[k+8] = 0;
        bodyVertices[k+9] = cn;
        bodyVertices[k+10] = 1.0;
        bodyVertices[k+11] = sn;
        bodyVertices[k+12] = 0;
        bodyVertices[k+13] = cn;
        around+=circleStep;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, nBodyVertices*VERTEX_DATA_BYTESIZE, bodyVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}

/**
 * setup GL_ARRAY_BUFFER for the vertices of the Cylinder top and bottom circles.
 */
bool Cylinder::initLidBuffers(){
    //nLidVertices = 2*(nSides+1);
    //setup a GLuint buffer to fill with vertex data and then copy it to the GPU.
    unsigned int bufferSize = nLidVertices*VERTEX_DATA_SIZE/*-NORMA_DATA_SIZE*/;
    GLfloat lidVertices[bufferSize];

    unsigned int indices_per_lid = (nSides+1)*VERTEX_DATA_SIZE; // each vertex has VERTEX_DATAS_SIZE glfloats


    double circleStep = float((2.0f*M_PI*radius)/nSides);
    double around=0.0;
    unsigned int k=0;

    //center of top lid
    lidVertices[k+0] = 0;
    lidVertices[k+1] = height;
    lidVertices[k+2] = 0.0;
    lidVertices[k+3] = 1.0;
    lidVertices[k+4] = 0;
    lidVertices[k+5] = 1.0;
    lidVertices[k+6] = 0;

    //center of bottom lid
    lidVertices[indices_per_lid+k] = 0;
    lidVertices[indices_per_lid+k+1] = 0;
    lidVertices[indices_per_lid+k+2] = 0;
    lidVertices[indices_per_lid+k+3] = 1.0;
    lidVertices[indices_per_lid+k+4] = 0;
    lidVertices[indices_per_lid+k+5] = -1;
    lidVertices[indices_per_lid+k+6] = 0;

    k=VERTEX_DATA_SIZE;
    for(int s=0/*-NORMA_DATA_SIZE*/; s<nSides; ++s){

        double sn = radius*sin(around);
        double cn = radius*cos(around);
        //add a vertex at the top lid circumfance.
        lidVertices[k+0] = sn;
        lidVertices[k+1] = height;
        lidVertices[k+2] = cn;
        lidVertices[k+3] = 1.0;
        lidVertices[k+4] = 0;
        lidVertices[k+5] = 1.0;
        lidVertices[k+6] = 0;

        //add a vertex at the bottom lid circumfance.
        unsigned int m = indices_per_lid+k;
        lidVertices[m] = sn;
        lidVertices[m+1] = 0;
        lidVertices[m+2] = cn;
        lidVertices[m+3] = 1.0;
        lidVertices[m+4] = 0;
        lidVertices[m+5] = -1;
        lidVertices[m+6] = 0;

        around+=circleStep;
        k+=VERTEX_DATA_SIZE;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize*sizeof(GLfloat), lidVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}

bool Cylinder::addMaterials(){
    const char *vertexSrc = "shaders/bricks/phong/vertex.glsl";
    const char *fragmentSrc ="shaders/bricks/phong/fragment.glsl";
    bool result = false;
    material = Material::makeMaterial(vertexSrc, fragmentSrc);
    if(material) {

        positionAttributeHandle = material->getAttrib("MCposition");
        normalAttributeHandle = material->getAttrib("MCnormal");

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

Cylinder& Cylinder::operator=(const Cylinder& m){
    return *this;
}

void Cylinder::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos){

    material->activate();

    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;

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

    renderBody();
    renderLids();

    material->deactivate();
}

void Cylinder::renderBody() {

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glEnableVertexAttribArray(positionAttributeHandle);
    glEnableVertexAttribArray(normalAttributeHandle);
    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(POSITION_OFFSET));

    glVertexAttribPointer(normalAttributeHandle, NORMAL_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(NORMAL_OFFSET));
    glDrawElements(GL_TRIANGLE_STRIP, 2*(nSides+1), GL_UNSIGNED_INT,0);
    glDisableVertexAttribArray(positionAttributeHandle);
    glDisableVertexAttribArray(normalAttributeHandle);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cylinder::renderLids() {

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);

    glEnableVertexAttribArray(positionAttributeHandle);
    glEnableVertexAttribArray(normalAttributeHandle);

    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(POSITION_OFFSET));

    glVertexAttribPointer(normalAttributeHandle, NORMAL_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(NORMAL_OFFSET));


    //glDrawElements(GL_TRIANGLE_FAN, nSides+2, GL_UNSIGNED_INT,0);
    glDrawElements(GL_TRIANGLE_FAN, nSides+2, GL_UNSIGNED_INT,
                   reinterpret_cast<const void *>(sizeof(GLuint) * (nSides + 2)));

    glDisableVertexAttribArray(positionAttributeHandle);
    glDisableVertexAttribArray(normalAttributeHandle);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
