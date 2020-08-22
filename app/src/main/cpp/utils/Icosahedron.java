import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

import ShaderClass;
import com.learnopengles.android.Polyhedrons.Vector3D;

import badlogic.gdx.backends.android.AndroidGL20;

public class Icosahedron extends AbstractPolyhedron {

	private FloatBuffer verticesBuffer;
	private ShortBuffer triangleFanIndicesBuffer;
	private ShortBuffer triangleStripIndexBuffer;
	private ShortBuffer wireFrameTopBottomLinesIndexBuffer;
	private ShortBuffer wireFrameMiddleStripLinesIndexBuffer;
	
	private final static String LogTag = "ICOSAHEDRON";
	private final static int POSITION_DATA_SIZE = 3;
	private final static int POSITION_OFFSET = 0;
	private final static int NORMAL_DATA_SIZE = 3;
	private final static int NORMAL_OFFSET = POSITION_DATA_SIZE * BYTES_PER_FLOAT;
	private final static int VERTEX_DATA_SIZE = NORMAL_DATA_SIZE+ POSITION_DATA_SIZE;
	private final static int NUM_VERTICES = 12;
	private static final int STRIDE = VERTEX_DATA_SIZE * BYTES_PER_FLOAT;
	
	public Icosahedron() {
		vertices = new Vector3D[NUM_VERTICES];
		vertexNormals = new Vector3D[NUM_VERTICES];
		for(int i=0; i<12; i++)
			vertexNormals[i] = new Vector3D();
		for(int i=0; i<12; i++)
			vertices[i] = new Vector3D();
		//create arrays for vertex buffer objects and index buffer objects
		vbo = new int[1];
		ibo = new int[3];
	}

	@Override
	public void render(AndroidGL20 glEs20, ShaderClass shader) {
		shader.useShader();
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		 
		// Bind Attributes
		int positionAttribute = shader.getAttribLoc("a_Position");
		int normalAttribute = shader.getAttribLoc("a_Normal");
		GLES20.glEnableVertexAttribArray(positionAttribute);
		GLES20.glEnableVertexAttribArray(normalAttribute);
		
		glEs20.glVertexAttribPointer(positionAttribute, POSITION_DATA_SIZE, GLES20.GL_FLOAT, false, STRIDE, POSITION_OFFSET);
		glEs20.glVertexAttribPointer(normalAttribute,   NORMAL_DATA_SIZE,   GLES20.GL_FLOAT, false, STRIDE, NORMAL_OFFSET);
		
		 
		int colorLoc = shader.getUniformLoc("u_Color");
		
		// Draw
		//set triangles color
		GLES20.glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f) ;
		
		//draw triangles
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		glEs20.glDrawElements(GLES20.GL_TRIANGLE_FAN, 7, GLES20.GL_UNSIGNED_SHORT, 0);
		GLES20.glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f) ;
		glEs20.glDrawElements(GLES20.GL_TRIANGLE_FAN, 7, GLES20.GL_UNSIGNED_SHORT, 7*BYTES_PER_SHORT);
		
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
		for(int i= 0, evenIndex =0; i<5; i++, evenIndex+=12){
			
			GLES20.glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f) ;
			glEs20.glDrawElements(GLES20.GL_TRIANGLES, 3, GLES20.GL_UNSIGNED_SHORT, evenIndex);
			GLES20.glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f) ;
			glEs20.glDrawElements(GLES20.GL_TRIANGLES, 3, GLES20.GL_UNSIGNED_SHORT, evenIndex+6);
		}
		
		//set wireframe color and render the wireframe 
		GLES20.glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f) ;

		//draw wireframe around the ten triangles at the middle of the icosahedron.
		for(int i= 0; i<10; i++){
			
			glEs20.glDrawElements(GLES20.GL_LINE_LOOP, 3, GLES20.GL_UNSIGNED_SHORT, 6*i);
		}
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
		glEs20.glDrawElements(GLES20.GL_LINES, 20, GLES20.GL_UNSIGNED_SHORT, 0);
		
		//GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[3]);
		//glEs20.glDrawElements(GLES20.GL_LINES, 20, GLES20.GL_UNSIGNED_SHORT, 0);

		//unbind the buffers and disable vertices attribs
		GLES20.glDisableVertexAttribArray(positionAttribute);
		GLES20.glDisableVertexAttribArray(normalAttribute);
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	@Override
	protected void buildVertices() {
		float phiaAngles  = 26.56505f; /* the phi angle needed for generation */

		float r = 1.0f; /* any radius in which the polyhedron is inscribed */
		/* first vertex */
		
		float phiRadians = (float) (Math.PI*phiaAngles/180.0f); /* 2 sets of four points */
		float deltaTheta = (float) (Math.PI * (72.0f/ 180.0f));
		float theta = 0.0f;
		float product_r_cosPhi = r * (float)Math.cos(phiRadians);
		float product_r_sinPhi = r * (float)Math.sin(phiRadians);
		
		for(int i=1; i<6; i++){
			vertices[i].x = (float) Math.cos(theta)* product_r_cosPhi;
		    vertices[i].y = (float) Math.sin(theta)* product_r_cosPhi;
		    vertices[i].z = product_r_sinPhi;
		    theta += deltaTheta;
		}
//		
		//product_r_cosPhi = r * android.util. (float) Math.cos(-phiRadians);
		product_r_sinPhi = -product_r_sinPhi;
		theta = (float) (Math.PI*36.0/180.0f);
		for(int i=6; i<11; i++){
			vertices[i].x = (float) Math.cos(theta)* product_r_cosPhi;
		    vertices[i].y = (float) Math.sin(theta)* product_r_cosPhi;
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

	@Override
	protected void buildNormals() {
		Polygon[] faces = new Polygon[20];
		/* map vertices to 4 faces */
		faces[0]  = new Polygon(new int[]{0,1,2});//
		faces[1]  = new Polygon(new int[]{0,2,3});//
		faces[2]  = new Polygon(new int[]{0,3,4});//
		faces[3]  = new Polygon(new int[]{0,4,5});//
		faces[4]  = new Polygon(new int[]{0,5,1});//
		faces[5]  = new Polygon(new int[]{11,6,7});//
		faces[6]  = new Polygon(new int[]{11,7,8});//
		faces[7]  = new Polygon(new int[]{11,8,9});//
		faces[8]  = new Polygon(new int[]{11,9,10});//
		faces[9]  = new Polygon(new int[]{11,10,6});//
		faces[10] = new Polygon(new int[]{1,2,6});//
		faces[11] = new Polygon(new int[]{2,3,7});//
		faces[12] = new Polygon(new int[]{3,4,8});//
		faces[13] = new Polygon(new int[]{4,5,9});//
		faces[14] = new Polygon(new int[]{5,1,10});//
		faces[15] = new Polygon(new int[]{6,7,2});//
		faces[16] = new Polygon(new int[]{7,8,3});//
		faces[17] = new Polygon(new int[]{8,9,4});//
		faces[18] = new Polygon(new int[]{9,10,5});//
		faces[19] = new Polygon(new int[]{10,6,1});//
		
		Vector3D[] faceNormals = new Vector3D[faces.length];
		for(int i=0; i<faces.length; i++){
			faceNormals[i] = new Vector3D(
				Vector3D.crossProduct(
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(1)),
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(2))
				)
			);
		}
		
		vertexNormals[0]  = new Vector3D(0f, 0f,  1.0f);
		vertexNormals[11] = new Vector3D(0f, 0f, -1.0f);
		vertexNormals[1]  = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[4], faceNormals[10], faceNormals[14], faceNormals[19]});
		vertexNormals[2]  = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[10], faceNormals[11], faceNormals[15]});
		vertexNormals[3]  = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[2], faceNormals[11], faceNormals[12] , faceNormals[16]});
		vertexNormals[4]  = Vector3D.avarage(new Vector3D[]{faceNormals[2], faceNormals[3], faceNormals[12], faceNormals[13], faceNormals[17]});
		vertexNormals[5]  = Vector3D.avarage(new Vector3D[]{faceNormals[3], faceNormals[4], faceNormals[13], faceNormals[14], faceNormals[18]});
		vertexNormals[6]  = Vector3D.avarage(new Vector3D[]{faceNormals[5], faceNormals[9], faceNormals[10], faceNormals[15], faceNormals[19]});
		vertexNormals[7]  = Vector3D.avarage(new Vector3D[]{faceNormals[5], faceNormals[6], faceNormals[11], faceNormals[15], faceNormals[16]});
		vertexNormals[8]  = Vector3D.avarage(new Vector3D[]{faceNormals[6], faceNormals[7], faceNormals[12], faceNormals[16], faceNormals[17]});
		vertexNormals[9]  = Vector3D.avarage(new Vector3D[]{faceNormals[7], faceNormals[8], faceNormals[12], faceNormals[16], faceNormals[17]});
		vertexNormals[10] = Vector3D.avarage(new Vector3D[]{faceNormals[8], faceNormals[9], faceNormals[14], faceNormals[18], faceNormals[19]});
		
		for(int i=0; i<vertexNormals.length; i++)
			vertexNormals[i].normalize();


	}

	@Override
	protected void buildBuffers() {
		int dataSize = this.vertices.length*VERTEX_DATA_SIZE;
		float[] veteticesData = new float[dataSize];
	
		for(int i=0, j=0; i<dataSize ; i+=VERTEX_DATA_SIZE, j++){
			veteticesData[i] = this.vertices[j].x;
			veteticesData[i+1] = this.vertices[j].y;
			veteticesData[i+2] = this.vertices[j].z;
			veteticesData[i+3] = this.vertexNormals[j].x;
			veteticesData[i+4] = this.vertexNormals[j].y;
			veteticesData[i+5] = this.vertexNormals[j].z;
		}
		
		verticesBuffer = ByteBuffer.allocateDirect(dataSize * BYTES_PER_FLOAT).
						  order(ByteOrder.nativeOrder()).asFloatBuffer();
		verticesBuffer.put(veteticesData).position(0);
		
		
		triangleFanIndicesBuffer = ByteBuffer.allocateDirect(14*BYTES_PER_SHORT).
								 order(ByteOrder.nativeOrder()).asShortBuffer();
		triangleFanIndicesBuffer.put(new short[]{0,1,2,3,4,5,1,11,10,9,8,7,6,10}).position(0);
		
		triangleStripIndexBuffer = ByteBuffer.allocateDirect(30*BYTES_PER_SHORT).
				 order(ByteOrder.nativeOrder()).asShortBuffer();
		triangleStripIndexBuffer.put(
				new short[]{6,1,10,
						     1,6,2,
						     7,2,6,
						     2,7,3,
						     8,3,7,
						     3,8,4,
						     9,4,8,
						     4,9,5,
						     10,5,9,
						     5,10,1}).position(0);
	
		wireFrameTopBottomLinesIndexBuffer = ByteBuffer.allocateDirect(20*BYTES_PER_SHORT).
				 order(ByteOrder.nativeOrder()).asShortBuffer();
		wireFrameTopBottomLinesIndexBuffer.put(new short[]{0,1,0,2,0,3,0,4,0,5,11,6,11,7,11,8,11,9,11,10}).position(0);
		
		
	}

	@Override
	public void sendDataToGPU() {
		GLES20.glGenBuffers(1, vbo, 0);
		GLES20.glGenBuffers(3, ibo, 0);
		 
		if (vbo[0] > 0 && ibo[0] > 0) {
		    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		    GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, verticesBuffer.capacity()
		                            * BYTES_PER_FLOAT, verticesBuffer, GLES20.GL_STATIC_DRAW);
		 
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		    GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, triangleFanIndicesBuffer.capacity()
		                            * BYTES_PER_SHORT, triangleFanIndicesBuffer, GLES20.GL_STATIC_DRAW);
		    
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
		    GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, triangleStripIndexBuffer.capacity()
		                            * BYTES_PER_SHORT, triangleStripIndexBuffer, GLES20.GL_STATIC_DRAW);
		    
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
		    GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, wireFrameTopBottomLinesIndexBuffer.capacity()
		                            * BYTES_PER_SHORT, wireFrameTopBottomLinesIndexBuffer, GLES20.GL_STATIC_DRAW);
		 
		    
		    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		    GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	}

}
