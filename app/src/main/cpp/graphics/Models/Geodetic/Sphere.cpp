//
// Created by Erez on 19/09/20.
//

#include "Sphere.h"
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



Sphere::Sphere(float rad, int disks, int sides, int idx):
        HardCodedModel(), m_idx(idx){
    setup(rad, disks, sides);
}

Sphere::Sphere(const glm::mat4& transform, float rad, int disks, int sides, int idx):
        HardCodedModel(transform), m_idx(idx){
    setup(rad, disks, sides);
}

Sphere::~Sphere(){
    destroy();
}

void Sphere::setup(float rad, int disks, int sides){
    radius = rad;
    nRings = (disks<=0?1:disks);
    nSides = sides;
    nVertices = 1+nRings*nSides;

    vertices = nullptr;
    normals = nullptr;

}

bool Sphere::init(){
    return initVertices()&&initFaces()&&initBuffers()&&addMaterials();
}

void Sphere::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos){

    //renderBody(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);
    //if(!isOpen())
    //    renderLids(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
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

    int offset = 0;
    int count = 2*(nSides+1);
    int rings_minus_one = nRings-1;

    //render the upper hemisphere
    for(int k=0; k<rings_minus_one; k++) {
        glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(offset * sizeof(GL_UNSIGNED_INT)));
        offset+=count;
    }
    glDrawElements(GL_TRIANGLE_FAN,nSides+2, GL_UNSIGNED_INT,
                   reinterpret_cast<const void *>((nRings-1)*count* sizeof(GL_UNSIGNED_INT)));

    //render the lower hemisphere
    offset=0;
    activeTransform.scale(-1.0);
    glUniformMatrix4fv(mMVPMatrixHandle, 1, false, glm::value_ptr(projectionMat*viewMat*activeTransform.get()));
    for(int k=0; k<rings_minus_one; k++) {
        glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(offset * sizeof(GL_UNSIGNED_INT)));
        offset+=count;
    }


    glDrawElements(GL_TRIANGLE_FAN,nSides+2, GL_UNSIGNED_INT,
                   reinterpret_cast<const void *>((nRings-1)*count* sizeof(GL_UNSIGNED_INT)));
    glDisableVertexAttribArray(positionAttributeHandle);
    //glDisableVertexAttribArray(colorAttributeHandle);
    glDisableVertexAttribArray(normalAttributeHandle);
    //glBindTexture(GL_TEXTURE_3D, 0);

    material->deactivate();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sphere::destroy(){
    if(vertices)
        delete[] vertices;
    if(normals)
        delete[] normals;
}

void Sphere::update(long time){
    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    //activeTransform.scale(1.4f);

    activeTransform.rotate(glm::radians(-45.0), glm::vec3(1.0, 0.0, 0.0)) ;
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, 1.0)) ;


}

bool Sphere::initVertices(){
    vertices = new glm::vec4[nVertices];
    normals  = new glm::vec3[nVertices];
    return vertices && normals;
}

bool Sphere::initFaces(){

    //building vertices from bottom up.
    double angleStep = 2.0*M_PI / nSides;
    double heightStep = radius/double(nRings);
    double level = 0;
    int k = 0;

    for(int i=0; i< nRings; ++i){
        double angle = 0.0;
        double radi = sqrt(radius*radius-level*level);
        for(int c=0; c<nSides; ++c){
            vertices[k] = glm::vec4(
                    radi*sin(angle), //x
                    level, //y
                    radi*cos(angle), //z
                    1.0);

            normals[k] = glm::vec3(vertices[i*nSides+c]);
            normals[k].y = 0;
            angle+=angleStep;
            k++;
        }
        level +=heightStep;

    }
    //tip of the Sphere
    vertices[nVertices-1] = glm::vec4(0.0, radius, 0.0, 1.0);
    normals[nVertices-1] = glm::vec3(0.0, 1.0, 0.0);
    return true;
}

bool Sphere::initBuffers(){
    glGenBuffers(1, &vbo[0]);
    glGenBuffers(1, ibo);
    if(0== vbo || 0 == ibo[0]){
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


    int rings_minus_one = nRings-1;
    int nIndices = (2+nSides)+ 2*rings_minus_one*(nSides+1);
    GLuint* indices = new GLuint[nIndices];
    if(!indices)
        return false;

    int k=0;
    for(int h=0; h< rings_minus_one; ++h){
        for(int c=0; c<nSides; ++c){
            indices[k+0] = nSides*(h+1)+c;
            indices[k+1] = nSides*(h+0)+c;
            k+=2;
        }
        indices[k++] = nSides*(h+1);
        indices[k++] = nSides*(h+0);
    }

    indices[k++] = nVertices-1;
    for(int c=0; c<nSides; ++c){
        indices[k++] = (rings_minus_one*nSides)+c;
    }
    indices[k] = rings_minus_one*nSides;


    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertices*VERTEX_DATA_BYTESIZE, vertexBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete[] indices;
    return true;
}

bool Sphere::addMaterials(){
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

Sphere& Sphere::operator=(const Sphere& m){
    return *this;
}


