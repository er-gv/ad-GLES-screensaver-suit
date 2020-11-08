//
// Created by Erez on 19/09/20.
//

#include "Torus.h"
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
        {0.007f, 0.635f, 0.49f},
        {1.0f, 1.0f, 1.0f}
};

static const GLfloat brickColor[][3] {
        {0.952f, 0.6901f, 0.188f},
        {0.75390625f, 0.53828125f, 0.69921875f},
        {0.557f, 0.094f, 0.129f},
        {0.635f, 0.305f, 0.007f},
        {0.788f, 0.968f, 0.0392f},
        {1.0f, 1.0f, 1.0f}
};

static const GLfloat brickSize[][2]{
        {0.3f, 0.15f},
        {0.7, 0.65f},
        {0.3f, 0.15f},
        {0.3f, 0.15f},
        {0.3f, 0.15f},
        {0.3f, 0.15f}
};

static const GLfloat brickPct[][2] {
        {0.9f, 0.85},
        {0.9, 0.85},
        {0.9f, 0.85},
        {0.9f, 0.85},
        {0.9f, 0.85},
        {0.9f, 0.85}

};


Torus::Torus(float inner_rad, float outer_rad, int disks, int sides, int idx): HardCodedModel(){
    m_idx = idx;
    setup(inner_rad, outer_rad, disks, sides);
}

Torus::Torus(const glm::mat4& transform, float inner_rad, float outer_rad, int disks, int sides, int idx):
        HardCodedModel(transform), m_idx(idx % 5){
    setup(inner_rad, outer_rad, disks, sides);
}

Torus::~Torus(){
    destroy();
}

void Torus::setup(float rad_in, float rad_out, int disks, int sides){
    inner_radius = rad_in;
    outer_radius = rad_out;
    nRings = (disks<=0?1:disks);
    nSides = sides;
    nVertices = nRings*nSides;

    vertices = nullptr;
    normals = nullptr;

}

bool Torus::init(){
    return initVertices()&&initFaces()&&initBuffers()&&addMaterials();
}

void Torus::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos){

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    material->activate();
    glEnableVertexAttribArray(positionAttributeHandle);
    glEnableVertexAttribArray(normalAttributeHandle);
    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(POSITION_OFFSET));

    glVertexAttribPointer(normalAttributeHandle, NORMAL_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_BYTESIZE, reinterpret_cast<const void *>(NORMAL_OFFSET));


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


    //renderWireframe();
    renderSolid();

    glDisableVertexAttribArray(positionAttributeHandle);
    //glDisableVertexAttribArray(colorAttributeHandle);
    glDisableVertexAttribArray(normalAttributeHandle);
    //glBindTexture(GL_TEXTURE_3D, 0);

    material->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Torus::renderWireframe(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glDrawArrays(GL_POINTS, 0, nVertices);
    int count = nSides;
    int offset = 0;
    for(int k=0; k<nRings; k++) {
        glDrawElements(GL_LINE_LOOP, count, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(offset* sizeof(GL_UNSIGNED_INT)));
        offset+=count;
    }
    count = nRings;
    offset = nSides*nRings;
    for(int k=0; k<nSides; k++) {
        glDrawElements(GL_LINE_LOOP, count, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(offset* sizeof(GL_UNSIGNED_INT)));
        offset+=count;
    }
}

void Torus::renderSolid(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    int count = 4*(nSides);
    int offset = 0;
    int rings_minus_one = nSides-1;
    for(int k=0; k<nRings; k++) {
        glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(offset* sizeof(GL_UNSIGNED_INT)));
        offset+=count;
    }
}

void Torus::destroy(){
    if(vertices)
        delete[] vertices;
    if(normals)
        delete[] normals;
}

void Torus::update(long time){
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    //activeTransform.scale(1.4f);

    activeTransform.rotate(M_PI_2, glm::vec3(1.0, 0.0, 0.0)) ;
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 1.0)) ;
    activeTransform.scale(0.645f);

}

bool Torus::initVertices(){
    vertices = new glm::vec4[nVertices];
    normals  = new glm::vec3[nVertices];
    return vertices && normals;
}

bool Torus::initFaces(){

    //building vertices from bottom up.
    double angleStep = 2.0*M_PI / nSides;

    int k = 0;
    double ringsAngle = 0.0;
    double ringsAngleStep = 2.0*M_PI/nRings;
    double radi = 0.5*(outer_radius-inner_radius);
    for(int i=0; i< nRings; ++i){
        double c = cos(ringsAngle);
        double s = sin(ringsAngle);
        glm::vec4 ref((inner_radius+radi)*c, 0, (inner_radius+radi)*s, 0);
        double angle = 0.0;

        for(int j=0; j<nSides; ++j){
            glm::mat4 rotation(
                    glm::vec4(c,0,s,0),
                    glm::vec4(0,1,0,0),
                    glm::vec4(-s,0, c,0),
                    glm::vec4(0,0,0,1));
            glm::vec4 pt(radi*cos(angle), radi*sin(angle),0, 1);
            vertices[k] = glm::vec4(ref+rotation*pt);

            normals[k] = glm::vec3(rotation*pt);
            angle+=angleStep;
            k++;
        }
        ringsAngle+=ringsAngleStep;

    }
    return true;
}

bool Torus::initBuffers(){
    glGenBuffers(1, &vbo[0]);
    glGenBuffers(2, ibo);
    if(0== vbo[0] || 0 == ibo[0] || 0==ibo[1]){
        int f = (0==vbo[0]?0:1);
        __android_log_print(ANDROID_LOG_ERROR, "TORUS", "glGenBuffers failed, ibo[%d].", f);
        return false;
    }

    GLfloat vertexBuffer[nVertices*VERTEX_DATA_SIZE];
    for(unsigned int i=0; i<nVertices; ++i){
        unsigned k = i*VERTEX_DATA_SIZE;
        vertexBuffer[k+0] = vertices[i].x;
        vertexBuffer[k+1] = vertices[i].y;
        vertexBuffer[k+2] = vertices[i].z;
        vertexBuffer[k+3] = vertices[i].w;
        vertexBuffer[k+4] = normals[i].x;
        vertexBuffer[k+5] = normals[i].y;
        vertexBuffer[k+6] = normals[i].z;
    }


    int nWireIndices = 2*nSides*nRings;
    auto* wireframeIndices = new GLuint[nWireIndices];

    int nSolidIndices = 4*nSides*nRings;
    auto* solidIndices = new GLuint[nSolidIndices];
    if(!solidIndices || !wireframeIndices){
        if(!*wireframeIndices)
        __android_log_print(ANDROID_LOG_ERROR, "TORUS", "glGenBuffers failed, wireframeIndices is null.");
        if(!*solidIndices)
        __android_log_print(ANDROID_LOG_ERROR, "TORUS", "glGenBuffers failed, solidIndices is null.");
        return false;
    }
    //buildup indices for wireframe rendering.
    int k=0;
    for(int h=0; h< nRings; ++h){
        for(int c=0; c<nSides; ++c){
            wireframeIndices[k++] = nSides*h+c;
        }
    }
    for(int h=0; h< nSides; ++h){
        for(int c=0; c<nRings; ++c){
            wireframeIndices[k++] = nSides*c+h;
        }
    }


    //buildup indices for solid rendering.
    k=0;
    for(int r=0; r<nRings; ++r){
        for(int s=0; s< nSides; ++s) {
            solidIndices[k++] = r*nSides + s;
            solidIndices[k++] = r*nSides + (s+1)%nSides;
            solidIndices[k++] = ((r+1)%nRings)*nSides + s;
            solidIndices[k++] = ((r+1)%nRings)*nSides + (s+1)%nSides;
        }

    }



    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertices*VERTEX_DATA_BYTESIZE, vertexBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nWireIndices*sizeof(GLuint), wireframeIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nSolidIndices*sizeof(GLuint), solidIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] wireframeIndices;
    delete[] solidIndices;
    __android_log_print(ANDROID_LOG_INFO, "TORUS", "done initBuffers");
    return true;
}

bool Torus::addMaterials(){
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

