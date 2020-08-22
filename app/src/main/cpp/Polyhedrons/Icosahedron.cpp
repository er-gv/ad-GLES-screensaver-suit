//

// Created by nathan on 16/04/20.
//

#include "Icosahedron.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

static GLushort constexpr triangleFanIndicesBuffer[]= {0,1,2,3,4,5,1,11,10,9,8,7,6,10};
static GLushort constexpr triangleStripIndexBuffer[]= {6,1,10,
                                                       1,6,2,
                                                       7,2,6,
                                                       2,7,3,
                                                       8,3,7,
                                                       3,8,4,
                                                       9,4,8,
                                                       4,9,5,
                                                       10,5,9,
                                                       5,10,1};


static GLushort constexpr wireFrameTopBottomLinesIndexBuffer[]=
        {0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10};

Polyhedrons::Icosahedron::Icosahedron() :
    Polyhedron(), LogTag("ICOSAHEDRON"){

}

Polyhedrons::Icosahedron::~Icosahedron() {
    destroy();
}

void Polyhedrons::Icosahedron::render() {
		glUseProgram(materialProgram);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

		// Bind Attributes
		int positionAttribute = glGetAttribLocation(materialProgram, "a_Position");
		int normalAttribute = glGetAttribLocation(materialProgram, "a_Normal");
		glEnableVertexAttribArray(positionAttribute);
		glEnableVertexAttribArray(normalAttribute);

		glVertexAttribPointer(positionAttribute, POSITION_DATA_SIZE, GL_FLOAT, false, STRIDE, verticesData+POSITION_OFFSET);
		glVertexAttribPointer(normalAttribute,   NORMAL_DATA_SIZE,   GL_FLOAT, false, STRIDE, verticesData+NORMAL_OFFSET);


		int colorLoc = glGetUniformLocation(materialProgram, "u_Color");

		// Draw
		//set triangles color
		glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f) ;

		//draw triangles
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_SHORT, triangleFanIndicesBuffer);
		glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f) ;
		glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_SHORT, triangleFanIndicesBuffer+(7*BYTES_PER_SHORT));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
		for(int i= 0, evenIndex =0; i<5; i++, evenIndex+=12){

			glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f) ;
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,  triangleStripIndexBuffer+evenIndex);
			glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f) ;
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,  triangleStripIndexBuffer+evenIndex+6);
		}

		//set wireframe color and render the wireframe
		glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f) ;

		//draw wireframe around the ten triangles at the middle of the icosahedron.
		for(int i= 0; i<10; i++){
			glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT,  &triangleStripIndexBuffer[3*i]);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
		glDrawElements(GL_LINES, 20, GL_UNSIGNED_SHORT,  wireFrameTopBottomLinesIndexBuffer);

		//GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[3]);
		//glEs20.glDrawElements(GLES20.GL_LINES, 20, GLES20.GwireFrameTopBottomLinesIndexBufferL_UNSIGNED_SHORT, 0);

		//unbind the buffers and disable vertices attribs
		glDisableVertexAttribArray(positionAttribute);
		glDisableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void Polyhedrons::Icosahedron::buildGeometry() {
		float phiAngles  = 26.56505f; /* the phi angle needed for generation */

        float r = 1.0f; /* any radius in which the polyhedron is inscribed */
        /* first vertex */

        float phiRadians = glm::radians(phiAngles); /* 2 sets of four points */
        float deltaTheta = glm::radians(72.0f);
        float theta = 0.0f;
        float product_r_cosPhi = r * cos(phiRadians);
        float product_r_sinPhi = r * sin(phiRadians);

        for(int i=1; i<6; i++){
            vertices[i].x = cos(theta)* product_r_cosPhi;
            vertices[i].y = sin(theta)* product_r_cosPhi;
            vertices[i].z = product_r_sinPhi;
            theta += deltaTheta;
        }
        //
        //product_r_cosPhi = r * android.util. (float) Math.cos(-phiRadians);
        product_r_sinPhi = -product_r_sinPhi;
        theta = glm::radians(36.0);
        for(int i=6; i<11; i++){
            vertices[i].x = cos(theta)* product_r_cosPhi;
            vertices[i].y = sin(theta)* product_r_cosPhi;
            vertices[i].z = product_r_sinPhi;
            theta += deltaTheta;
        }
        //set top and bottom vertices
        vertices[0].x  =  0.0f;
        vertices[0].z  =  r;
        vertices[0].y  =  0.0f;
        vertices[11].x =  0.0f;
        vertices[11].z = -r;
        vertices[11].y =  0.0f;
    }


    void Polyhedrons::Icosahedron::buildNormals() {
    Polyhedron::Polygon faces[NUM_FACES] = {
            /* map vertices to 20 faces */
            Polygon(this,(int[]){0, 1, 2}),
            Polygon(this,(int[]){ 0, 2, 3 }),
            Polygon(this,(int[]){ 0, 3, 4 }),
            Polygon(this,(int[]){ 0, 4, 5 }),
            Polygon(this,(int[]){ 0, 5, 1 }),
            Polygon(this,(int[]){ 11, 6, 7 }),
            Polygon(this,(int[]){ 11, 7, 8 }),
            Polygon(this,(int[]){ 11, 8, 9 }),
            Polygon(this,(int[]){ 11, 9, 10 }),
            Polygon(this,(int[]){ 11, 10, 6 }),
            Polygon((int[]){ 1, 2, 6 }),
            Polygon((int[]){ 2, 3, 7 }),
            Polygon((int[]){ 3, 4, 8 }),
            Polygon((int[]){ 4, 5, 9 }),
            Polygon((int[]){ 5, 1, 10 }),
            Polygon((int[]){ 6, 7, 2 }),
            Polygon((int[]){ 7, 8, 3 }),
            Polygon((int[]){ 8, 9, 4 }),
            Polygon((int[]){ 9, 10, 5 }),
            Polygon((int[]){ 10, 6, 1 })
    };

    glm::vec3 faceNormals[NUM_FACES];
    for(int i=0; i<NUM_FACES; i++){
        glm::vec3 const &v0 = faces[i].getVertex(*this, 0);
        glm::vec3 const &v1 = faces[i].getVertex(*this, 1);
        glm::vec3 const &v2 = faces[i].getVertex(*this, 2);
        faceNormals[i] = glm::cross(v0 -v1, v0 -v2);
    }

    /*vertexNormals[0]  = glm::vec3(0f, 0f,  1.0f);
    vertexNormals[11] = glm::vec3(0f, 0f, -1.0f);
    vertexNormals[1]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[0], faceNormals[4], faceNormals[10], faceNormals[14], faceNormals[19]});
    vertexNormals[2]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[0], faceNormals[1], faceNormals[10], faceNormals[11], faceNormals[15]});
    vertexNormals[3]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[1], faceNormals[2], faceNormals[11], faceNormals[12] , faceNormals[16]});
    vertexNormals[4]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[2], faceNormals[3], faceNormals[12], faceNormals[13], faceNormals[17]});
    vertexNormals[5]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[3], faceNormals[4], faceNormals[13], faceNormals[14], faceNormals[18]});
    vertexNormals[6]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[5], faceNormals[9], faceNormals[10], faceNormals[15], faceNormals[19]});
    vertexNormals[7]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[5], faceNormals[6], faceNormals[11], faceNormals[15], faceNormals[16]});
    vertexNormals[8]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[6], faceNormals[7], faceNormals[12], faceNormals[16], faceNormals[17]});
    vertexNormals[9]  = glm::vec3.avarage(new glm::vec3[]{faceNormals[7], faceNormals[8], faceNormals[12], faceNormals[16], faceNormals[17]});
    vertexNormals[10] = glm::vec3.avarage(new glm::vec3[]{faceNormals[8], faceNormals[9], faceNormals[14], faceNormals[18], faceNormals[19]});

    for(int i=0; i<NUM_FACES; i++)
        vertexNormals[i].normalize();*/
}

void Polyhedrons::Icosahedron::buildBuffers() {
    int dataSize = NUM_FACES*VERTEX_DATA_SIZE;
    verticesData = new float[dataSize];

    for(int i=0, j=0; i<dataSize ; i+=VERTEX_DATA_SIZE, j++){
        verticesData[i] = vertices[j].x;
        verticesData[i+1] = vertices[j].y;
        verticesData[i+2] = vertices[j].z;
        /*veteticesData[i+3] = this.vertexNormals[j].x;
        veteticesData[i+4] = this.vertexNormals[j].y;
        veteticesData[i+5] = this.vertexNormals[j].z;*/
    }

}

void Polyhedrons::Icosahedron::sendDataToGPU() {
    glGenBuffers(1, vbo);
    glGenBuffers(3, ibo);

    if (vbo[0] > 0 && ibo[0] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, STRIDE*NUM_VERTICES, verticesData, GL_STATIC_DRAW);

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
    }

}

    void Polyhedrons::Icosahedron::destroy() {
        if(nullptr != vbo)
            delete vbo;
        if(nullptr != ibo)
            delete ibo;
        if(nullptr != verticesData)
            delete verticesData;
    }

    bool Polyhedrons::Icosahedron::initShaders() {

    }

    bool Polyhedrons::Icosahedron::init() {
        buildGeometry();
        buildNormals();
        buildBuffers();
        initShaders();
    }



