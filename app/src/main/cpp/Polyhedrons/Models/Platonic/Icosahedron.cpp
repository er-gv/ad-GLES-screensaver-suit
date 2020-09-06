//

// Created by nathan on 16/04/20.
//

#include <vector>
#include <tuple>
#include <graphics/PerlinNoiseGenerator.h>

#include "Icosahedron.h"

static constexpr GLfloat faceColors[] ={
        0.941176f, 0.9019607f, 0.549019f,
        0.6, 0.2, 0.2, 0.7, 1.0, 0.3,
        0.894501f, 0.444444f, 0.839215f
};
Polyhedrons::Icosahedron::Icosahedron() :
    Polyhedron(), LogTag("ICOSAHEDRON"){
    paintSpatter = nullptr;
}

Polyhedrons::Icosahedron::~Icosahedron() {
    destroy();
}

void Polyhedrons::Icosahedron::update(long time) {

    float angleInDegrees = (360.0f / 10000.0f) * ((int) time);

    activeTransform.setTransform(initialTransform.get());
    activeTransform.scale(3.4f);
    activeTransform.rotate(glm::radians(angleInDegrees), glm::vec3(0.0, 1.0, -1.0)) ;

}

void Polyhedrons::Icosahedron::render(glm::mat4& viewMat, glm::mat4& projectionMat, const glm::vec3& lightPos) {

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE_BYTES, reinterpret_cast<const void *>(POSITION_OFFSET));
    glEnableVertexAttribArray( positionAttributeHandle);

    glVertexAttribPointer(texcoordAttributeHandle, TEX_DATA_SIZE, GL_FLOAT,
                          static_cast<GLboolean>(false),
                          VERTEX_DATA_SIZE_BYTES, reinterpret_cast<const void *>(TEX_OFFSET));
    glEnableVertexAttribArray(texcoordAttributeHandle);


    //draw triangles
    glm::mat4 viewT = viewMat*activeTransform.get();
    glm::mat4 projT = projectionMat*viewT;
    glm::mat4 normalsT = glm::inverse(glm::transpose(activeTransform.get()));

    glLineWidth(7.5);
    wireFrame->activate();
    glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
    glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    glDrawElements(GL_LINE_LOOP, 10, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINE_LOOP, 5, GL_UNSIGNED_INT, reinterpret_cast<const void *>(10* sizeof(GLuint)));
    glDrawElements(GL_LINE_LOOP, 5, GL_UNSIGNED_INT, reinterpret_cast<const void *>(15 * sizeof(GLuint)));

    wireFrame->deactivate();
    glLineWidth(1.0);

    paintSpatter->activate();
    glUniformMatrix4fv(MVPMatrixHandle, 1, false, glm::value_ptr(projT));
    glUniformMatrix4fv(MVMatrixHandle, 1, false, glm::value_ptr(viewT));
    //glUniformMatrix4fv(NormalMatrixHandle, 1, false, glm::value_ptr(normalsT));
    glUniform3fv(lightPositionHandler, 1, glm::value_ptr(lightPos));
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to this unit
    glBindTexture(GL_TEXTURE_3D, textureDataHandler);
    // Tell the texture uniform sampler to use the texture
    // in the shader by binding to texture unit 0.
    glUniform1i(textureSamplerHandler, 0);
    /*
     * glUniform1f(paintSpatter->getUniform("u_diffuseCoaff"), 0.0);
     * glUniform1f(paintSpatter->getUniform("u_specularCoaff"), 0.0);
     * glUniform1f(paintSpatter->getUniform("u_shininess"), 7.0);
     * glUniform1f(paintSpatter->getUniform("u_ambiantCoaff"), 0.2);
     */



    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
    //glUniform4f(paintSpatter->getUniform("u_Color"), faceColors[0], faceColors[1], faceColors[2], 1.0f);
    /*for(int i=0; i<2; ++i) {
        //glUniform3fv(paintSpatter->getUniform("u_FaceNormal"), 1, glm::value_ptr(vertexNormals[i]));
        glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(7 * i * sizeof(GLuint)));
    }*/

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
    glUniform3f(paintColorHandler,0.941176f, 0.9019607f, 0.549019f);
    glUniform3f(materialColorHandler,  0.6, 0.2, 0.2);
    glUniform1f(thresholdHandler, 0.075);
    for(int i=0; i<NUM_FACES; ++i) {
        //glUniform3fv(paintSpatter->getUniform("u_FaceNormal"), 1, glm::value_ptr(vertexNormals[i]));
        //glUniform3f(materialColorHandler, faceColors[3 * (i % 2) + 3], faceColors[3 * (i % 2) + 4], faceColors[3 * (i % 2) + 5]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                       reinterpret_cast<const void *>(((3 * i) )* sizeof(GLuint)));
    }


    glDisableVertexAttribArray(positionAttributeHandle);
    glDisableVertexAttribArray(texcoordAttributeHandle);
    glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(0);
    paintSpatter->deactivate();
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
    facesIdx.push_back(std::make_tuple( 1, 2,0));
    facesIdx.push_back(std::make_tuple( 2, 3,0 ));
    facesIdx.push_back(std::make_tuple( 3, 4,0 ));
    facesIdx.push_back(std::make_tuple( 4, 5,0));
    facesIdx.push_back(std::make_tuple( 5, 1,0 ));
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

    GLuint triangleStripIndexBuffer[]= {
            0,2,1,            0,3,2,            0,4,3,
            0,5,4,            0,1,5,

            10,1,6, 2,6,1,  6,2,7, 3,7,2, 7,3,8,
            4,8,3,  8,4,9,  5,9,4, 9,5,10, 1,10,5,

            11, 9,10,            11, 8, 9,            11, 7, 8,
            11, 6, 7,            11, 10, 6};


    GLuint wireFrameTopBottomLinesIndexBuffer[]=
            {0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10};
    GLuint triangleLineLoopsBuffer[]= {10,5,9,4,8,3,7,2,6,1,
                                       1,2,3,4,5,
                                       9,8,7,6,10};

    for(int i=0; i<NUM_VERTICES ; ++i){
        int j = i*VERTEX_DATA_SIZE;
        vertexBuffer[j] = vertices[i].x;
        vertexBuffer[j+1] = vertices[i].y;
        vertexBuffer[j+2] = vertices[i].z;
        vertexBuffer[j+3] = vertices[i].x;
        vertexBuffer[j+4] = vertices[i].y;
        vertexBuffer[j+5] = vertices[i].z;
    }
    glGenBuffers(1, &vbo);
    glGenBuffers(3, ibo);

    if (vbo > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleLineLoopsBuffer),
                     triangleLineLoopsBuffer, GL_STATIC_DRAW);

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
    if(paintSpatter)
        delete paintSpatter;
    if(wireFrame)
        delete  wireFrame;
}

bool Polyhedrons::Icosahedron::initShaders() {
    if(!addMaterials())
        return false;

    positionAttributeHandle = paintSpatter->getAttrib("a_Position");
    texcoordAttributeHandle = paintSpatter->getAttrib("a_TexCoord");


    MVPMatrixHandle = paintSpatter->getUniform("u_MVPMatrix");
    MVMatrixHandle = paintSpatter->getUniform("u_MVMatrix");
    //NormalMatrixHandle = paintSpatter->getUniform("u_NormalMatrix");
    lightPositionHandler = paintSpatter->getUniform("u_LightPos");
    textureSamplerHandler = paintSpatter->getUniform("u_Texture");
    normalHandle = paintSpatter->getUniform("u_Normal");
    paintColorHandler = paintSpatter->getUniform("u_PaintColor");
    materialColorHandler = paintSpatter->getUniform("u_MaterialColor");
    thresholdHandler = paintSpatter->getUniform("u_Threshold");
    textureDataHandler = PerlinNoiseGenerator::get3DTexture();
    return (textureDataHandler >0);
}
    
bool Polyhedrons::Icosahedron::addMaterials(){
    const char vertex_shader[] = "shaders/vertex/vertex_turbulence_texture.glsl";
    const char fragment_shader[] = "shaders/fragment/fragment_paint_spatter.glsl"; //paint_spatter
    paintSpatter = Material::makeMaterial(vertex_shader, fragment_shader);
    wireFrame = Material::makeMaterial("shaders/wireframe/vertex.glsl", "shaders/wireframe/fragment.glsl");
    return (paintSpatter && wireFrame);
};

bool Polyhedrons::Icosahedron::init() {
    return (initVertices() && initFaces() &&  initBuffers() && initShaders());
}



