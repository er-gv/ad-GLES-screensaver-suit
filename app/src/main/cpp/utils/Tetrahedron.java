import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import badlogic.gdx.backends.android.AndroidGL20;

import android.opengl.GLES20;
import android.util.Log;

import ShaderClass;
import com.learnopengles.android.Polyhedrons.Vector3D;


public class Tetrahedron extends AbstractPolyhedron {

	private static final String LogTag = "TETRAHEDRON";
	private FloatBuffer verticesBuffer;
	private ShortBuffer trianglesIndexBuffer;
	
	private ShortBuffer wireFrameLinesIndexBuffer;
	private static final int POSITION_DATA_SIZE = 3;
	private static final int NORMAL_DATA_SIZE = 3;
	private static final int TEX_DATA_SIZE = 2;
	private static final int VERTEX_DATA_SIZE = BYTES_PER_FLOAT*(TEX_DATA_SIZE +NORMAL_DATA_SIZE +POSITION_DATA_SIZE);
	
	public Tetrahedron(){
		//create new Vector3D arrays to hold vertex mPosition and normal data.
		this.vertexNormals = new Vector3D[4];
		this.vertices = new Vector3D[4];
		for(int i=0; i<4; i++)
			vertexNormals[i] = new Vector3D();
		for(int i=0; i<4; i++)
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
		int normalAttributeHandle = shader.getAttribLoc("a_Normal");
		int texAttributeHandle = shader.getAttribLoc("a_TexCoordinate");
		
		GLES20.glEnableVertexAttribArray(positionAttributeHandle);
		GLES20.glEnableVertexAttribArray(normalAttributeHandle);
		GLES20.glEnableVertexAttribArray(texAttributeHandle);
		
		glEs20.glVertexAttribPointer(positionAttributeHandle, POSITION_DATA_SIZE,  GLES20.GL_FLOAT, false, VERTEX_DATA_SIZE, 0);
		glEs20.glVertexAttribPointer(normalAttributeHandle,   NORMAL_DATA_SIZE,    GLES20.GL_FLOAT, false, VERTEX_DATA_SIZE, 12);
		glEs20.glVertexAttribPointer(texAttributeHandle,      TEX_DATA_SIZE,       GLES20.GL_FLOAT, false, VERTEX_DATA_SIZE, 24);
		
		//pass in cloud and sky color.
		int skyColorLoc = shader.getUniformLoc("u_skyColor");
		int cloudColorLoc = shader.getUniformLoc("u_cloudColor");
		glEs20.glUniform3f(cloudColorLoc, 0.8f, 0.8f, 0.8f);
		glEs20.glUniform3f(skyColorLoc, 0.0f, 0.0f, 0.65f);
		
		
		// Set the active texture unit to texture unit 0.
        //GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        
        
		//draw triangles
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
		
		glEs20.glDrawElements(GLES20.GL_TRIANGLES, 12, GLES20.GL_UNSIGNED_SHORT, 0);
		
		//draw wireframe
		//GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
		//glEs20.glDrawElements(GLES20.GL_LINES, 12, GLES20.GL_UNSIGNED_SHORT, 0);
		
		//unbind the buffers and the vertices attribs
		GLES20.glDisableVertexAttribArray(normalAttributeHandle);
		GLES20.glDisableVertexAttribArray(positionAttributeHandle);
		GLES20.glDisableVertexAttribArray(texAttributeHandle);

		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	@Override
	protected void buildBuffers() {
		
		
		
		float[] verticesData = new float[]{
			
			////////////////////////////////////////////////////////////
			//vertex 0
			//
			vertices[0].x, vertices[0].y, vertices[0].z, //spatial data 0
			vertexNormals[0].x, vertexNormals[0].y, vertexNormals[0].z, //normal data 0
			0.5f, 0.5f, //tex data 0

			////////////////////////////////////////////////////////////
			//vertex 1
			//
			vertices[1].x, vertices[1].y, vertices[1].z, //spatial data 1
			vertexNormals[1].x, vertexNormals[1].y, vertexNormals[1].z, //normal data 1
			0.0f, 0.0f, //tex data 1

			////////////////////////////////////////////////////////////
			//vertex 2
			//
			vertices[2].x, vertices[2].y, vertices[2].z, //spatial data 2
			vertexNormals[2].x, vertexNormals[2].y, vertexNormals[2].z, //normal data 2
			1.0f, 0.0f, //tex data 2
			
			////////////////////////////////////////////////////////////
			//vertex 3
			//
			vertices[3].x, vertices[3].y, vertices[3].z, //spatial data 3
			vertexNormals[3].x, vertexNormals[3].y, vertexNormals[3].z, //normal data 3
			0.5f, 1.0f //tex data 3
			
		};
		verticesBuffer = ByteBuffer.allocateDirect(4 * VERTEX_DATA_SIZE).
				  order(ByteOrder.nativeOrder()).asFloatBuffer();
		verticesBuffer.put(verticesData).position(0);	
		
		trianglesIndexBuffer = ByteBuffer.allocateDirect(12*BYTES_PER_SHORT).
								 order(ByteOrder.nativeOrder()).asShortBuffer();
		trianglesIndexBuffer.put(new short[]{0,1,2, 3,1,0, 0,2,3, 3,2,1}).position(0);
										 
		wireFrameLinesIndexBuffer = ByteBuffer.allocateDirect(12*BYTES_PER_SHORT).
				 order(ByteOrder.nativeOrder()).asShortBuffer();
		wireFrameLinesIndexBuffer.put(new short[]{0,1,0,2,0,3,1,2,1,3,2,3}).position(0);
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

	@Override
	protected void buildVertices() {
		float phiaAngles  = -19.471220333f; /* the phi angle needed for generation */

		float r = 1.0f; /* any radius in which the polyhedron is inscribed */
		/* first vertex */
		float phiRadians = (float) (Math.PI*phiaAngles/180.0f); 
		float deltaTheta = (float) (120.0f * Math.PI / 180.0f);
		vertices[0].x = 0.0f;
		vertices[0].z = 0.0f;
		vertices[0].y = r;
		float theta = 0.0f;
		float rCosPhi = r* (float)Math.cos(phiRadians);
		float rMultsinPhi = r* (float)Math.sin(phiRadians);
		for(int i=1; i<4; i++){
			vertices[i].x =  (float)Math.cos(theta*2)*rCosPhi;
		    vertices[i].z =  (float)Math.sin(theta*2)*rCosPhi;
		    vertices[i].y = rMultsinPhi;
		    theta += deltaTheta;
		}
		for(int i =0; i<4; i++){
			String data = "vertices["+i+"] = ("+vertices[i].x+" , "+vertices[i].y+" , "+vertices[i].z+" , "+").";
			Log.i(LogTag, data);
		}
	}

	@Override
	protected void buildNormals() {
		Polygon[] faces = new Polygon[4];
		/* map vertices to 4 faces */
		faces[0] = new Polygon(new int[]{0,1,2});
		faces[1] = new Polygon(new int[]{0,2,3});
		faces[2] = new Polygon(new int[]{0,3,1});
		faces[3] = new Polygon(new int[]{1,2,3});
		
		Vector3D[] faceNormals = new Vector3D[faces.length];
		for(int i=0; i<faces.length; i++){
			faceNormals[i] = new Vector3D(
				Vector3D.crossProduct(
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(1)),
					Vector3D.subtract(faces[i].getVertex(0), faces[i].getVertex(2))
				)
			);
		}
		
		this.vertexNormals[0] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[2]}); 
		this.vertexNormals[1] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[2], faceNormals[3]});
		this.vertexNormals[2] = Vector3D.avarage(new Vector3D[]{faceNormals[0], faceNormals[1], faceNormals[3]});
		this.vertexNormals[3] = Vector3D.avarage(new Vector3D[]{faceNormals[1], faceNormals[2], faceNormals[3]});
		
		for(int i=0; i<this.vertexNormals.length; i++){
			this.vertexNormals[i].normalize();
		}
		for(int i=0; i<this.vertexNormals.length; i++){
			String data = "vertexNormals["+i+"] = ("+vertexNormals[i].x+" , "+vertexNormals[i].y+" , "+vertexNormals[i].z+" , "+").";
			Log.i(LogTag, data);
		}
	}
}
