/*package Polyhedrons;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

import ShaderClass;
import com.learnopengles.android.Polyhedrons.Vector3D;

import badlogic.gdx.backends.android.AndroidGL20;

public class Dodecahedron extends AbstractPolyhedron {

	private FloatBuffer verticesBuffer;
	private ShortBuffer trianglesIndexBuffer;

	public Dodecahedron() {
		this.vertexNormals = new Vector3D[20];
		this.vertices = new Vector3D[20];
		for(int i=0; i<20; i++)
			vertexNormals[i] = new Vector3D();
		for(int i=0; i<20; i++)
			vertices[i] = new Vector3D();
		//create arrays for vertex buffer objects and index buffer objects
		vbo = new int[1];
		ibo = new int[1];
	}

	@Override
	public void render(AndroidGL20 glEs20, ShaderClass shader) {
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		 
		
		// Bind Attributes
		int positionAttributeHandle = shader.getAttribLoc("a_Position");
		//int normalAttributeHandle = shader.getAttribLoc("a_Normal");
		//GLES20.glEnableVertexAttribArray(normalAttributeHandle);
		GLES20.glEnableVertexAttribArray(positionAttributeHandle);
		
		//verticesBuffer.mPosition(POSITION_OFFSET);
		glEs20.glVertexAttribPointer(positionAttributeHandle, 3, GLES20.GL_FLOAT, false, 0, 0);
		
		//verticesBuffer.mPosition(this.NORMAL_OFFSET);
		//glEs20.glVertexAttribPointer(normalAttributeHandle, this.NORMAL_DATA_SIZE, GLES20.GL_FLOAT, false, stride, this.NORMAL_OFFSET);
		
		int colorLoc = shader.getUniformLoc("u_Color");
		
		// Draw
		
		//set triangles color
		float[] colorsSet = new float[]{
			1.0f, 0.0f, 0.0f,	
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.2f,
			0.3f, 0.0f, 1.0f
			
		};
		
		//draw triangles
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		for(int i =0, faceIndex=0; i< 12; i++, faceIndex+=10){
			int colorIndex = 3*(i % 4);
			glEs20.glUniform3f(colorLoc, colorsSet[colorIndex], colorsSet[colorIndex+1], colorsSet[colorIndex+2]);
			//glEs20.glUniform3f(colorLoc, colorsSet[colorIndex], colorsSet[colorIndex+1], colorsSet[colorIndex+2]);
			glEs20.glDrawElements(GLES20.GL_TRIANGLE_FAN, 5, GLES20.GL_UNSIGNED_SHORT, faceIndex);
		}
		
		
		//unbind the buffers and the vertices attribs
		glEs20.glDisableVertexAttribArray(positionAttributeHandle);
		//glEs20.glDisableVertexAttribArray(normalAttributeHandle);
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);


	}

	@Override
	protected void buildVertices() {
		float phiaAngles  = 52.62263590f; /* the phi angle needed for generation *

		float r = 1.0f; /* any radius in which the polyhedron is inscribed */
		/* first vertex *
		float phiRadians = (float) (Math.PI*phiaAngles/180.0); 
		float deltaTheta = (float) (Math.PI*72.0/180.0);
		float rCosPhi = r*  (float) Math.cos(phiRadians);
		float rSinPhi = r*  (float) Math.sin(phiRadians);
		float theta = 0.0f;
		for(int i=0; i<5; i++){
			vertices[i].x =  (float) Math.cos(theta)*rCosPhi;
		    vertices[i].z =  (float) Math.sin(theta)*rCosPhi;
		    vertices[i].y = rSinPhi;
		    theta += deltaTheta;
		}
		theta = 0.0f;
		phiRadians = -phiRadians;
		rCosPhi = r*  (float) Math.cos(phiRadians);
		rSinPhi = r*  (float) Math.sin(phiRadians);
		for(int i=15; i<20; i++){
			vertices[i].x =  (float) Math.cos(theta)*rCosPhi;
		    vertices[i].z =  (float) Math.sin(theta)*rCosPhi;
		    vertices[i].y = rSinPhi;
		    theta += deltaTheta;
		}
		
		phiaAngles  = 10.81231754f;
		phiRadians =  (float) (Math.PI*phiaAngles/180.0);
		deltaTheta = (float) (deltaTheta/2.0);
		rCosPhi = r*  (float) Math.cos(phiRadians);
		rSinPhi = r*  (float) Math.sin(phiRadians);
		for(int i=5; i<10; i++){
			vertices[i].x =  (float) Math.cos(theta)*rCosPhi;
		    vertices[i].z =  (float) Math.sin(theta)*rCosPhi;
		    vertices[i].y = rSinPhi;
		    theta += deltaTheta;
		}
		theta = 0.0f;
		phiRadians = -phiRadians;
		rCosPhi = r* (float) Math.cos(phiRadians);
		rSinPhi = r*  (float) Math.sin(phiRadians);
		for(int i=10; i<15; i++){
			vertices[i].x =  (float) Math.cos(theta)*rCosPhi;
		    vertices[i].z =  (float) Math.sin(theta)*rCosPhi;
		    vertices[i].y = rSinPhi;
		    theta += deltaTheta;
		}
	}

	@Override
	protected void buildNormals() {
		Polygon[] faces = new Polygon[12];
		/ map vertices to 12 faces *
			
		faces[0] =  new Polygon(new int[]{0,1,2,3,4});
		faces[1] =  new Polygon(new int[]{0,1,6,10,5});
		faces[2] =  new Polygon(new int[]{1,2,7,11,6});
		faces[3] =  new Polygon(new int[]{2,3,8,12,7});
		faces[4] =  new Polygon(new int[]{3,4,9,13,8});
		faces[5] =  new Polygon(new int[]{4,0,5,14,9});
		faces[6] =  new Polygon(new int[]{15,16,11,6,10});
		faces[7] =  new Polygon(new int[]{16,17,12,7,11});
		faces[8] =  new Polygon(new int[]{17,18,13,8,12});
		faces[9] =  new Polygon(new int[]{18,19,14,9,13});
		faces[10] = new Polygon(new int[]{19,15,10,5,14});
		faces[11] = new Polygon(new int[]{15,16,17,18,19});		  
		  
		Vector3D[] faceNormals = new Vector3D[faces.length];
		for(int i=0; i<faces.length; i++){
			faceNormals[i] = new Vector3D(
				Vector3D.crossProduct(
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(1)),
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(2))
				)
			);
		}
		
		/
		
		 * *
		this.vertexNormals[0] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[5]}); 
		this.vertexNormals[1] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[2]});
		this.vertexNormals[2] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[2], faceNormals[3]});
		this.vertexNormals[3] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[3], faceNormals[4]});
		this.vertexNormals[4] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[4], faceNormals[5]});
		this.vertexNormals[5] = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[5], faceNormals[10]});
		this.vertexNormals[6] = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[2], faceNormals[6]});
		this.vertexNormals[7] = Vector3D.avarage(new Vector3D[]{faceNormals[2], faceNormals[3], faceNormals[7]});
		this.vertexNormals[8] = Vector3D.avarage(new Vector3D[]{faceNormals[3], faceNormals[4], faceNormals[8]});
		this.vertexNormals[9] = Vector3D.avarage(new Vector3D[]{faceNormals[4], faceNormals[5], faceNormals[9]});
		this.vertexNormals[10] = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[6], faceNormals[10]});
		this.vertexNormals[11] = Vector3D.avarage(new Vector3D[]{faceNormals[2], faceNormals[6], faceNormals[7]});
		this.vertexNormals[12] = Vector3D.avarage(new Vector3D[]{faceNormals[3], faceNormals[7], faceNormals[8]});
		this.vertexNormals[13] = Vector3D.avarage(new Vector3D[]{faceNormals[4], faceNormals[8], faceNormals[9]});
		this.vertexNormals[14] = Vector3D.avarage(new Vector3D[]{faceNormals[5], faceNormals[9], faceNormals[10]});
		this.vertexNormals[15] = Vector3D.avarage(new Vector3D[]{faceNormals[6], faceNormals[10], faceNormals[11]});
		this.vertexNormals[16] = Vector3D.avarage(new Vector3D[]{faceNormals[6], faceNormals[7], faceNormals[11]});
		this.vertexNormals[17] = Vector3D.avarage(new Vector3D[]{faceNormals[7], faceNormals[8], faceNormals[11]});
		this.vertexNormals[18] = Vector3D.avarage(new Vector3D[]{faceNormals[8], faceNormals[9], faceNormals[11]});
		this.vertexNormals[19] = Vector3D.avarage(new Vector3D[]{faceNormals[9], faceNormals[10], faceNormals[11]});
		
			
		for(int i=0; i<this.vertexNormals.length; i++){
			this.vertexNormals[i].normalize();
		}

	}

	@Override
	protected void buildBuffers() {
		//int dataSize = this.vertices.length*6;
		verticesBuffer = ByteBuffer.allocateDirect(60 * BYTES_PER_FLOAT).
						  order(ByteOrder.nativeOrder()).asFloatBuffer();
				
				float[] verticesData = new float[]{
					
					 /* spatial data*
						0.607f,  0.000f,  0.795f, 
						0.188f,  0.577f,  0.795f, 
					   -0.491f,  0.357f,  0.795f, 
					   -0.491f, -0.357f,  0.795f, 
						0.188f, -0.577f,  0.795f, 
						0.982f,  0.000f,  0.188f, 
						0.304f,  0.934f,  0.188f, 
					   -0.795f,  0.577f,  0.188f, 
					   -0.795f, -0.577f,  0.188f, 
						0.304f, -0.934f,  0.188f, 
						0.795f,  0.577f, -0.188f, 
					   -0.304f,  0.934f, -0.188f, 
					   -0.982f,  0.000f, -0.188f, 
					   -0.304f, -0.934f, -0.188f, 
						0.795f, -0.577f, -0.188f, 
						0.491f,  0.357f, -0.795f, 
					   -0.188f,  0.577f, -0.795f, 
					   -0.607f,  0.000f, -0.795f, 
					   -0.188f, -0.577f, -0.795f, 
						0.491f, -0.357f, -0.795f 
					
						 
					
				};
				verticesBuffer.put(verticesData).mPosition(0);
				
				trianglesIndexBuffer = ByteBuffer.allocateDirect(60*BYTES_PER_SHORT).
										 order(ByteOrder.nativeOrder()).asShortBuffer();
				trianglesIndexBuffer.put(
						new short[]{0,1,2,3,4,
									5,10,6,1,0,
									6,11,7,2,1,
									7,12,8,3,2,
									8,13,9,4,3,
									9,14,5,0,4,
									
									15,16,11,6,10,
									16,17,12,7,11,
									17,18,13,8,12,
									18,19,14,9,13,
									19,15,10,5,14, 
									19,18,17,16,15
						}
				).mPosition(0);
				
	}
			
	public void sendDataToGPU() {
		GLES20.glGenBuffers(1, vbo, 0);
		GLES20.glGenBuffers(1, ibo, 0);
				 
		if (vbo[0] > 0 && ibo[0] > 0) {
			GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
			GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, verticesBuffer.capacity()
				                            * BYTES_PER_FLOAT, verticesBuffer, GLES20.GL_STATIC_DRAW);
				 
			GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
			GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, trianglesIndexBuffer.capacity()
				                            * BYTES_PER_SHORT, trianglesIndexBuffer, GLES20.GL_STATIC_DRAW);
				    
			GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
			GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
		} 
				
	}

}
*/