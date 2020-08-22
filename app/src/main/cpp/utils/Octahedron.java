import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

import ShaderClass;
import com.learnopengles.android.Polyhedrons.Vector3D;

import badlogic.gdx.backends.android.AndroidGL20;

public class Octahedron extends AbstractPolyhedron {

	private FloatBuffer verticesBuffer;
	private ShortBuffer trianglesIndexBuffer;
	private ShortBuffer wireFrameLinesIndexBuffer;

	public Octahedron() {
		this.vertexNormals = new Vector3D[6];
		this.vertices = new Vector3D[6];
		for(int i=0; i<6; i++)
			vertexNormals[i] = new Vector3D();
		for(int i=0; i<6; i++)
			vertices[i] = new Vector3D();
		//create arrays for vertex buffer objects and index buffer objects
		vbo = new int[1];
		ibo = new int[2];
	}

	@Override
	public void render(AndroidGL20 glEs20, ShaderClass shader) {
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		 
		
		// Bind Attributes
		int positionAttributeHandle = shader.getAttribLoc("a_Position");
		int barecentricAttributeHandle = shader.getAttribLoc("a_Barecent");
		
		GLES20.glEnableVertexAttribArray(positionAttributeHandle);
		GLES20.glEnableVertexAttribArray(barecentricAttributeHandle);

		
		glEs20.glVertexAttribPointer(positionAttributeHandle,    3, GLES20.GL_FLOAT, false, 24, 0);
		glEs20.glVertexAttribPointer(barecentricAttributeHandle, 3, GLES20.GL_FLOAT, false, 24, 12);
		
		
		int defaultColorLoc = shader.getUniformLoc("u_OutOfThresholdColor");
		int thresholdLoc = shader.getUniformLoc("u_Threshold");
		glEs20.glUniform3f(defaultColorLoc, 1.00f, 0.0f, 0.0f);
		glEs20.glUniform1f(thresholdLoc, 0.45f);
		// Draw
		//set triangles color
		
		
		//draw triangles
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		
		
		glEs20.glDrawElements(GLES20.GL_TRIANGLE_FAN, 6, GLES20.GL_UNSIGNED_SHORT, 0);
		glEs20.glDrawElements(GLES20.GL_TRIANGLE_FAN, 6, GLES20.GL_UNSIGNED_SHORT, 12);
		
		//draw wireframe
		
		//unbind the buffers and the vertices attribs
		glEs20.glDisableVertexAttribArray(positionAttributeHandle);
		glEs20.glDisableVertexAttribArray(barecentricAttributeHandle);
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
		
	}

	@Override
	protected void buildVertices() {
		float phiaAngles  = 0.0f; /* the phi angle needed for generation */

		float r = 1.0f; /* any radius in which the polyhedron is inscribed */
		/* first vertex */
		float phiRadians = (float) (Math.PI*phiaAngles/180.0); 
		float deltaTheta = (float) (Math.PI/2.0);
		float theta = 0.0f;
		float rCosPhi = r *  (float)Math.cos(phiRadians);
		float rSinPhi = r *  (float)Math.sin(phiRadians);
		for(int i=1; i<5; i++){
			vertices[i].x =  (float)Math.cos(theta)*rCosPhi;
		    vertices[i].z =  (float)Math.sin(theta)*rCosPhi;
		    vertices[i].y = rSinPhi;
		    theta += deltaTheta;
		}
		
		vertices[0].x = 0.0f;
	    vertices[0].z = 0.0f;
	    vertices[0].y = r;
	    vertices[5].x = 0.0f;
	    vertices[5].z = 0.0f;
	    vertices[5].y = -r;
	}

	@Override
	protected void buildNormals() {
		Polygon[] faces = new Polygon[8];
		/* map vertices to 4 faces */
			
		faces[0] = new Polygon(new int[]{0,1,2});
		faces[1] = new Polygon(new int[]{0,2,3});
		faces[2] = new Polygon(new int[]{0,3,4});
		faces[3] = new Polygon(new int[]{0,4,1});
		faces[4] = new Polygon(new int[]{5,1,2});
		faces[5] = new Polygon(new int[]{5,2,3});
		faces[6] = new Polygon(new int[]{5,3,4});
		faces[7] = new Polygon(new int[]{5,4,1});
		
		Vector3D[] faceNormals = new Vector3D[faces.length];
		for(int i=0; i<faces.length; i++){
			faceNormals[i] = new Vector3D(
				Vector3D.crossProduct(
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(1)),
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(2))
				)
			);
		}
		
		this.vertexNormals[0] = new Vector3D(0.0f, 0.0f, 1.0f); 
		this.vertexNormals[1] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[4], faceNormals[7]});
		this.vertexNormals[2] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[4], faceNormals[5]});
		this.vertexNormals[3] = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[2], faceNormals[5], faceNormals[6]});
		this.vertexNormals[4] = Vector3D.avarage(new Vector3D[]{faceNormals[2], faceNormals[3], faceNormals[6], faceNormals[7]});
		this.vertexNormals[5] = new Vector3D(0.0f, 0.0f, -1.0f);
		
		
			
		for(int i=0; i<this.vertexNormals.length; i++){
			this.vertexNormals[i].normalize();
		}

	}

	@Override
	protected void buildBuffers() {
		//int dataSize = this.vertices.length*6;
		verticesBuffer = ByteBuffer.allocateDirect(36 * BYTES_PER_FLOAT).
						  order(ByteOrder.nativeOrder()).asFloatBuffer();
		
		float[] verticesData = new float[]{
			
				//vertex 0
					vertices[0].x, vertices[0].y, vertices[0].z, /* spatial data */
					0.0f, 1.0f, 0.0f,   /* barecentric weight */ 
			    //vertex 1
					vertices[1].x, vertices[1].y, vertices[1].z, /* spatial data */
					1.0f, 0.0f, 0.0f,   /* barecentric weight */ 
			    //vertex 2
					vertices[2].x, vertices[2].y, vertices[2].z, /* spatial data */
					0.0f, 0.0f, 1.0f,   /* barecentric weight */ 
				//vertex 3
					vertices[3].x, vertices[3].y, vertices[3].z, /* spatial data */
					1.0f, 0.0f, 0.0f,   /* barecentric weight */ 
				//vertex 4
					vertices[4].x, vertices[4].y, vertices[4].z, /* spatial data */
					0.0f, 0.0f, 1.0f,   /* barecentric weight */ 
				//vertex 5
					vertices[5].x, vertices[5].y, vertices[5].z, /* spatial data */
					0.0f, 1.0f, 0.0f    /* barecentric weight */ 
				
				
			
		};
		
		verticesBuffer.put(verticesData).position(0);	
		
		trianglesIndexBuffer = ByteBuffer.allocateDirect(12*BYTES_PER_SHORT).
								 order(ByteOrder.nativeOrder()).asShortBuffer();
		trianglesIndexBuffer.put(new short[]{0,4,3,2,1,4, 5,1,2,3,4,1}).position(0);
		
		
		wireFrameLinesIndexBuffer = ByteBuffer.allocateDirect(20*BYTES_PER_SHORT).
				 order(ByteOrder.nativeOrder()).asShortBuffer();
		wireFrameLinesIndexBuffer.put(new short[]{0,1,0,2,0,3,0,4, 5,1,5,2,5,3,5,4, 1,2,3,4}).position(0);
	}

	@Override
	public void sendDataToGPU() {
		GLES20.glGenBuffers(1, vbo, 0);
		GLES20.glGenBuffers(2, ibo, 0);
		 
		if (vbo[0] > 0 && ibo[0] > 0) {
		    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		    GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, verticesBuffer.capacity()
		                            * BYTES_PER_FLOAT, verticesBuffer, GLES20.GL_STATIC_DRAW);
		 
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		    GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, trianglesIndexBuffer.capacity()
		                            * BYTES_PER_SHORT, trianglesIndexBuffer, GLES20.GL_STATIC_DRAW);
		    
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
		    GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, wireFrameLinesIndexBuffer.capacity()
		                            * BYTES_PER_SHORT, wireFrameLinesIndexBuffer, GLES20.GL_STATIC_DRAW);
		 
		    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
		} 
		
	}

}
