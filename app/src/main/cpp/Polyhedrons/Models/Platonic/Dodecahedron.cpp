//
// Created by Erez on 16/04/20.
//

#include "Dodecahedron.h"
#include "graphics/PerlinNoiseGenerator.h"
#include <vector>

Polyhedrons::Dodecahedron::Dodecahedron():Polyhedrons::Polyhedron(), LogTag("DODECAHEDRON"){
    sunSurface = nullptr;
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
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.8, 0.5, -1.0)) ;
}

void Polyhedrons::Dodecahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE, 0);

    glVertexAttribPointer(texcoordAttributeHandle, TEX_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE, reinterpret_cast<const void *>(TEX_OFFSET));
    glEnableVertexAttribArray(positionAttributeHandle);
    glEnableVertexAttribArray(texcoordAttributeHandle);

    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;

    glLineWidth(7.5);
    wireFrame->activate();
    glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
    for(int i=0; i<NUM_FACES; ++i) {
        glDrawElements(GL_LINE_LOOP, 5, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(5 * i * sizeof(GLuint)));
    }
    wireFrame->deactivate();
    glLineWidth(1.0);

    sunSurface->activate();
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to this unit
    glBindTexture(GL_TEXTURE_3D, textureDataHandler);
    // Tell the texture uniform sampler to use the texture
    // in the shader by binding to texture unit 0.
    glUniform1i(textureSamplerHandler, 0);


    //draw triangles
    glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(MVMatrixHandle, 1, false, glm::value_ptr(viewT));
    glUniformMatrix4fv(NormalMatrixHandle, 1, false, glm::value_ptr(viewMat));

    glUniform3fv(lightPositionHandler, 1, glm::value_ptr(lightPos));
    /*glUniform1f(sunSurface->getUniform("u_diffuseCoaff"), 0.1);
    glUniform1f(sunSurface->getUniform("u_specularCoaff"), 0.9);
    glUniform1f(sunSurface->getUniform("u_shininess"), 5.0);
    glUniform1f(sunSurface->getUniform("u_ambiantCoaff"), 0.2);*/


    GLuint normHandle = sunSurface->getUniform("u_FaceNormal");
    //GLuint colorHandle = sunSurface->getUniform("u_Color");
    for(int i=0; i<NUM_FACES; ++i) {
        glUniform3fv(normHandle, 1, glm::value_ptr(vertexNormals[i]));
        //glUniform4f(colorHandle, faceColors[3*(i%4)], faceColors[3*(i%4)+1], faceColors[3*(i%4)+2], 1.0f);
        glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(5 * i * sizeof(GLuint)));
    }
    sunSurface->deactivate();


    glDisableVertexAttribArray(positionAttributeHandle);
    glDisableVertexAttribArray(texcoordAttributeHandle);
    glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(0);
    //sunSurface->deactivate();
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
            11,18,1,3,19,
            15,3,1,14,10,
            0,12,4,6,16,

            9,16,6,18,11,
            10,14,4,12,8,
            4,14,1,18,6,

            5,13,2,17,7,
            7,17,9,11,19,
            17,2,0,16,9,
            8,12,0,2,13,
            7,19,3,15,5,
            5,15,10,8,13
            };


    //GLuint wireFrameTopBottomLinesIndexBuffer[]=
     //       {0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10};

    for(int i=0; i<NUM_VERTICES ; ++i){
        int j = i*6;
        vertexBuffer[j] = vertices[i].x;
        vertexBuffer[j+1] = vertices[i].y;
        vertexBuffer[j+2] = vertices[i].z;
        vertexBuffer[j+3] = vertices[i].x;
        vertexBuffer[j+4] = vertices[i].y;
        vertexBuffer[j+5] = vertices[i].z;
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
    if(sunSurface)
        delete sunSurface;
    if(wireFrame)
        delete  wireFrame;
    textureDataHandler = 0;

}

bool Polyhedrons::Dodecahedron::initShaders() {

    if(addMaterials()){
        positionAttributeHandle = sunSurface->getAttrib("a_Position");
        texcoordAttributeHandle = sunSurface->getAttrib("a_TexCoord");

        MVPMatrixHandle = sunSurface->getUniform("u_MVPMatrix");
        MVMatrixHandle = sunSurface->getUniform("u_MVMatrix");
        NormalMatrixHandle = sunSurface->getUniform("u_NormalMatrix");

        lightPositionHandler = sunSurface->getUniform("u_LightPos");
        textureSamplerHandler = sunSurface->getUniform("u_Texture");

        textureDataHandler = PerlinNoiseGenerator::get3DTexture();
        return (textureDataHandler>0);
    }
    return false;
}

bool Polyhedrons::Dodecahedron::addMaterials(){

    const char vertex_shader[] = "shaders/vertex/vertex_turbulence_texture.glsl";
    const char fragment_shader[] = "shaders/fragment/fragment_sun_texture.glsl";
    sunSurface = Material::makeMaterial(vertex_shader, fragment_shader);
    wireFrame = Material::makeMaterial("shaders/wireframe/vertex.glsl", "shaders/wireframe/fragment.glsl");
    return (sunSurface && wireFrame);
};

bool Polyhedrons::Dodecahedron::init() {
    return (initVertices() && initFaces() &&  initBuffers() && initShaders());
}




