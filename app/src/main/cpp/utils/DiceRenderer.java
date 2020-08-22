import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.SystemClock;

import badlogic.gdx.backends.android.AndroidGL20;

import com.learnopengles.android.cpp.R;

import com.learnopengles.android.common.RawResourceReader;
import com.learnopengles.android.common.TextureHelper;

/**
 * This class implements our custom renderer. Note that the GL10 parameter passed in is unused for OpenGL ES 2.0
 * renderers -- the static class GLES20 is used instead.
 */
public class DiceRenderer implements GLSurfaceView.Renderer 
{	
	/** Used for debug logs. */
	private static final String TAG = "DiceRenderer";
	
	private final Context mActivityContext;
	
	private final AndroidGL20 glEs20;
	/**
	 * Store the model matrix. This matrix is used to move models from object space (where each model can be thought
	 * of being located at the center of the universe) to world space.
	 */
	private float[] mModelMatrix = new float[16];

	/**
	 * Store the view matrix. This can be thought of as our camera. This matrix transforms world space to eye space;
	 * it positions things relative to our eye.
	 */
	private float[] mViewMatrix = new float[16];

	/** Store the projection matrix. This is used to project the scene onto a 2D viewport. */
	private float[] mProjectionMatrix = new float[16];
	
	/** Allocate storage for the final combined matrix. This will be passed into the shader program. */
	private float[] mMVPMatrix = new float[16];
	
	/** 
	 * Stores a copy of the model matrix specifically for the light mPosition.
	 */
	private float[] mLightModelMatrix = new float[16];	
	
	/** This will be used to pass in the transformation matrix. */
	private int mMVPMatrixHandle;
	
	/** This will be used to pass in the modelview matrix. */
	private int mMVMatrixHandle;
	
	/** This will be used to pass in the light mPosition. */
	private int mLightPosHandle;
	
	
	/** Used to hold a light centered on the origin in model space. We need a 4th coordinate so we can get translations to work when
	 *  we multiply this by our transformation matrices. */
	private final float[] mLightPosInModelSpace = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
	
	/** Used to hold the current mPosition of the light in world space (after transformation via model matrix). */
	private final float[] mLightPosInWorldSpace = new float[4];
	
	/** Used to hold the transformed mPosition of the light in eye space (after transformation via modelview matrix) */
	private final float[] mLightPosInEyeSpace = new float[4];
	
	/** This is a handle to our light point program. */

	private AbstractPolyhedron[] mPolyhedrons;
	private final ShaderClass mLightShader;
	private final ShaderClass mPerPixelLightShader;
	private final ShaderClass mMonoChromeShader;
	private final ShaderClass mThresholdBlendingShader;
	private final ShaderClass mWoodShader;
	
	private Bitmap mPerlinTexture;
	private int mTextureDataHandle;

	private ShaderClass mReflectionShader;
	/**
	 * Initialize the model data.
	 */
	
	
	
	public DiceRenderer(final Context activityContext)
	{	
		mActivityContext = activityContext;
		glEs20 = new AndroidGL20();
		mPolyhedrons = new AbstractPolyhedron[5];
		
		mPolyhedrons[0] = new Tetrahedron();
		mPolyhedrons[1] = new Octahedron();
		mPolyhedrons[2] = new Dodecahedron();
		mPolyhedrons[3] = new Cube();
		mPolyhedrons[4] = new Icosahedron();
		mMonoChromeShader = new ShaderClass();
		mPerPixelLightShader = new ShaderClass();
		mThresholdBlendingShader = new ShaderClass();
		mWoodShader = new ShaderClass();
		
		
		
		mLightShader = new ShaderClass();
		for(int i=0; i<mPolyhedrons.length; i++)
			mPolyhedrons[i].init();
		// Define points for a cube.		
		
	}
	  
	protected String getVertexShader(int shaderID)
	{
		return RawResourceReader.readTextFileFromRawResource(mActivityContext, shaderID);
	}
	
	protected String getFragmentShader(int shaderID)
	{
		return RawResourceReader.readTextFileFromRawResource(mActivityContext, shaderID);
	}
	
	@Override
	public void onSurfaceCreated(GL10 glUnused, EGLConfig config) 
	{
		//init GPU buffers with our models
		for(int i=0; i<mPolyhedrons.length; i++)
			mPolyhedrons[i].sendDataToGPU();
		
		//create and init shader objects
		PerlinNoiseGenerator.seed(SystemClock.uptimeMillis());
		
		String vertexShaderSrc = getVertexShader(R.raw.monochrome_phong_vs);
		String fragmentShaderSrc = getFragmentShader(R.raw.monochrome_phong_fs);
		mMonoChromeShader.createAndLinkProgram(vertexShaderSrc, fragmentShaderSrc, new String[]{"a_Position", "a_Normal"});
		
		
		mPerPixelLightShader.createAndLinkProgram(getVertexShader(R.raw.per_pixel_vertex_shader_no_tex), 
												  getFragmentShader(R.raw.per_pixel_fragment_shader_no_tex),
												  new String[]{"a_Position", "a_Color", "a_Normal"});
		
		mThresholdBlendingShader.createAndLinkProgram(getVertexShader(R.raw.threshold_blending_vs), 
				  									  getFragmentShader(R.raw.threshold_blending_fs), 
				  									  new String[]{"a_Position", "a_Barecent"});
		
		mWoodShader.createAndLinkProgram(getVertexShader(R.raw.wood_vertex_shader), 
										 getFragmentShader(R.raw.wood_fragment_shader), 
										 new String[]{"a_Position",/*, "a_Normal",*/ "a_TexCoordinate"});
		
		


		GLES20.glClearColor(0.4f, 0.4f, 0.8f, 0.0f);
		
		// Use culling to remove back faces.
		GLES20.glEnable(GLES20.GL_CULL_FACE);
		GLES20.glCullFace(GLES20.GL_BACK);
		
		// Enable depth testing
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		
		// The below glEnable() call is a holdover from OpenGL ES 1, and is not needed in OpenGL ES 2.
		// Enable texture mapping
		//GLES20.glEnable(GLES20.GL_TEXTURE_2D);
			
		// Position the eye in front of the origin.
		final float eyeX = 0.0f;
		final float eyeY = 0.0f;
		final float eyeZ = -0.5f;

		// We are looking toward the distance
		final float lookX = 0.0f;
		final float lookY = 0.0f;
		final float lookZ = -15.0f;

		// Set our up vector. This is where our head would be pointing were we holding the camera.
		final float upX = 0.0f;
		final float upY = 1.0f;
		final float upZ = 0.0f;

		// Set the view matrix. This matrix can be said to represent the camera mPosition.
		// NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination of a model and
		// view matrix. In OpenGL 2, we can keep track of these matrices separately if we choose.
		Matrix.setLookAtM(mViewMatrix, 0, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);
		
		mPerlinTexture = PerlinNoiseGenerator.make2Dtexture(256, new float[]{1.0f, 0.5f, 0.25f, 0.125f, 0.0625f});
        mTextureDataHandle = TextureHelper.loadTexture(mPerlinTexture);
	}	
		
	@Override
	public void onSurfaceChanged(GL10 glUnused, int width, int height) 
	{
		// Set the OpenGL viewport to the same size as the surface.
		GLES20.glViewport(0, 0, width, height);

		// Create a new perspective projection matrix. The height will stay the same
		// while the width will vary as per aspect ratio.
		final float ratio = (float) width / height;
		final float left = -ratio;
		final float right = ratio;
		final float bottom = -1.0f;
		final float top = 1.0f;
		final float near = 1.0f;
		final float far = 20.0f;
		
		Matrix.frustumM(mProjectionMatrix, 0, left, right, bottom, top, near, far);
		
		
	}	

	@Override
	public void onDrawFrame(GL10 glUnused) 
	{
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);			        
                
        // Do a complete rotation every 10 seconds.
        long time = SystemClock.uptimeMillis() % 2500L;        
        float angleInDegrees = (360.0f / 2500.0f) * ((int) time);                
        
        // Calculate mPosition of the light. Rotate and then push into the distance.
        Matrix.setIdentityM(mLightModelMatrix, 0);
        Matrix.translateM(mLightModelMatrix, 0, 0.0f, 0.0f, -2.0f);      
               
        Matrix.multiplyMV(mLightPosInWorldSpace, 0, mLightModelMatrix, 0, mLightPosInModelSpace, 0);
        Matrix.multiplyMV(mLightPosInEyeSpace, 0, mViewMatrix, 0, mLightPosInWorldSpace, 0);                        
        
        // get program handlers for matrix uniforms
        mMVPMatrixHandle = mMonoChromeShader.getUniformLoc("u_MVPMatrix");
        
        
        //Octahedron
        this.renderOctahedron(angleInDegrees);
        
        //Cube
        this.renderCube(angleInDegrees);
        
        //  Tetrahedron
        this.renderTetrahedron(angleInDegrees);    
        
         
     
      
        // This multiplies the view matrix by the model matrix, and stores the result in the MVP matrix
        // (which currently contains model * view).
        
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        
        // Pass in the modelview matrix.
        GLES20.glUniformMatrix4fv(mMVMatrixHandle, 1, false, mMVPMatrix, 0);                
        
        // This multiplies the modelview matrix by the projection matrix, and stores the result in the MVP matrix
        // (which now contains model * view * projection).
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
        this.mMonoChromeShader.useShader();
        // Pass in the combined matrix.
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        
        // Pass in the light mPosition in eye space.
        //GLES20.glUniform3f(mLightPosHandle, mLightPosInEyeSpace[0], mLightPosInEyeSpace[1], mLightPosInEyeSpace[2]);
        
        //render the tetrahedron
       // this.mPolyhedronShader.useShader();
        mPolyhedrons[0].render(glEs20, this.mMonoChromeShader);
        
        //Dodecohedron
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, +4.0f, +7.0f, -14.0f);
        Matrix.scaleM(mModelMatrix, 0, 3.0f, 3.0f, 3.0f);
        Matrix.rotateM(mModelMatrix, 0, angleInDegrees, -1.0f, 1.0f, -1.0f);
        
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        // Pass in the modelview matrix.
        GLES20.glUniformMatrix4fv(mMVMatrixHandle, 1, false, mMVPMatrix, 0);                
        
        // This multiplies the modelview matrix by the projection matrix, and stores the result in the MVP matrix
        // (which now contains model * view * projection).
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);

        // Pass in the combined matrix.
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_DST_COLOR);		
        mPolyhedrons[2].render(glEs20, this.mMonoChromeShader);
        GLES20.glDisable(GLES20.GL_BLEND);

                

        ///////////////////////////////////////////////////////////////////////////////////////////           
        //Icosahedron
        //
        
       
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, 4.5f, -6.5f, -14.0f);
        Matrix.scaleM(mModelMatrix, 0, 3.0f, 3.0f, 3.0f);
        Matrix.rotateM(mModelMatrix, 0, angleInDegrees, -1.0f, -1.0f, -1.0f);
        
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        // Pass in the modelview matrix.
        this.mMonoChromeShader.useShader();
        GLES20.glUniformMatrix4fv(mMVMatrixHandle, 1, false, mMVPMatrix, 0);                
        
        // This multiplies the modelview matrix by the projection matrix, and stores the result in the MVP matrix
        // (which now contains model * view * projection).
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);

        // Pass in the combined matrix.
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        mPolyhedrons[4].render(glEs20, this.mMonoChromeShader);
        
        ///////////////////////////////////////////////////////////////////////////////////////////           
        //Light point
        //
        //this.mLightShader.useShader();
        //apply model view transformation on the icosahedron and then render it.       
       // drawLight();
        //GLES20.glUseProgram(0);
        
        
	}				
	
	private void renderTetrahedron(float angleInDegrees) {
		
		        
		//save aside the composition of scale and rotation while building the model matrix.
		//we use that combination for normal transform.
        float [] scaleAndRotationMat = new float[16];
        Matrix.setIdentityM(scaleAndRotationMat, 0);
        Matrix.scaleM(scaleAndRotationMat, 0, 1.0f, 1.0f, 1.0f);
        Matrix.rotateM(scaleAndRotationMat, 0, angleInDegrees, -1.0f, 1.0f, 0.0f);
        
        //enable shader and get matrices handlers.
        this.mWoodShader.useShader();
        int MVPMatrixHandle = this.mWoodShader.getUniformLoc("u_MVPMatrix");
        int MVMatrixHandle  = this.mWoodShader.getUniformLoc("u_MVMatrix");
        int NormalsMatrixHandle  = this.mWoodShader.getUniformLoc("u_MNormalsMatrix");
        
        // calculate the modelview matrix and pass it to the shader. 
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, 1.0f, 0.3f, -14.0f);
        Matrix.scaleM(mModelMatrix, 0, 5.0f, 5.0f, 5.0f);
        Matrix.rotateM(mModelMatrix, 0, angleInDegrees, -1.0f, -1.0f, -1.0f);
        
      //calculate mvp matrix and sent it to the shader.
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        GLES20.glUniformMatrix4fv(MVMatrixHandle, 1, false, mMVPMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
        GLES20.glUniformMatrix4fv(MVPMatrixHandle, 1, false, mMVPMatrix, 0);                
        
        //find the inverse transpose of the scale-rotate composition and pass it as normals transform matrix.
        float [] inverseMat = new float[16];
        float [] inverseTransposeMat = new float[16];

        Matrix.invertM(inverseMat, 0, scaleAndRotationMat, 0);
        Matrix.transposeM(inverseTransposeMat, 0, inverseMat, 0);
        //cut the top left 3X3 portion of the inverse transpose matrix. Need to do this sine the above methods work on 4X4 matrices
        //while the impotent data remains at the 3X3 top left portion.
        float [] normalMat = new float[]{inverseTransposeMat[0], inverseTransposeMat[1], inverseTransposeMat[2], 
        								 inverseTransposeMat[4], inverseTransposeMat[5], inverseTransposeMat[6],
        								 inverseTransposeMat[8], inverseTransposeMat[9], inverseTransposeMat[10]								 
        };
        // Pass in the normal matrix.
        GLES20.glUniformMatrix4fv(NormalsMatrixHandle, 1, false, normalMat, 0);                

        
      
      
        
        // Pass in the light mPosition in eye space.
        mLightPosHandle = this.mWoodShader.getUniformLoc("u_LightPos");
        GLES20.glUniform3f(mLightPosHandle, mLightPosInEyeSpace[0], mLightPosInEyeSpace[1], mLightPosInEyeSpace[2]);
     // Pass in the light scale.
        int mLightScaleHandle = this.mWoodShader.getUniformLoc("u_LuminumScale");
        GLES20.glUniform1f(mLightScaleHandle, 1.0f);
        
        
        // Set the active texture unit to texture unit 0.
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        
        // Bind the texture to this unit.
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureDataHandle);
        
        // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
        int textureUniformHandle = this.mWoodShader.getUniformLoc("u_Texture");
        GLES20.glUniform1i(textureUniformHandle, 0);        
        //render the tetrahedron
        mPolyhedrons[0].render(glEs20, this.mWoodShader);
      
		
	}

	private void renderOctahedron(float angleInDegrees){
		
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);		
		Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, -4.0f, +5.0f, -14.0f);
        Matrix.scaleM(mModelMatrix, 0, 3.0f, 3.0f, 3.0f);
        Matrix.rotateM(mModelMatrix, 0, angleInDegrees, 0.0f, 1.0f, 0.0f);
        
        //apply the shader we want to use here
        this.mThresholdBlendingShader.useShader();
        
        //get matrix handlers for this shader 
        int MVPMatrixHandle = this.mThresholdBlendingShader.getUniformLoc("u_MVPMatrix");
        
       
        //calculate mvp matrix and sent it to the shader.
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
        
       // Pass in the combined matrix.
        GLES20.glUniformMatrix4fv(MVPMatrixHandle, 1, false, mMVPMatrix, 0);
        mPolyhedrons[1].render(glEs20, mThresholdBlendingShader);
        GLES20.glDisable(GLES20.GL_BLEND);
	}

	private void renderCube(float angleInDegrees){
		
		Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, -4.0f, -7.0f, -14.0f);
        Matrix.scaleM(mModelMatrix, 0, 3.0f, 3.0f, 3.0f);
        Matrix.rotateM(mModelMatrix, 0, angleInDegrees, -1.0f, -1.0f, 1.0f);
        
        //apply the shader we want to use here
        this.mPerPixelLightShader.useShader();
        
        //get matrix handlers for this shader 
        int MVPMatrixHandle = this.mPerPixelLightShader.getUniformLoc("u_MVPMatrix");
        int MVMatrixHandle  = this.mPerPixelLightShader.getUniformLoc("u_MVMatrix");
       
        //calculate modelview matrix and sent it to the shader.
        float[] MVmatrix = new float[16];
        Matrix.multiplyMM(MVmatrix, 0, mViewMatrix, 0, mModelMatrix, 0);   
        GLES20.glUniformMatrix4fv(MVMatrixHandle, 1, false, MVmatrix, 0);
  
        
        //calculate MVP matrix and sent it to the shader.
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, MVmatrix, 0);
        GLES20.glUniformMatrix4fv(MVPMatrixHandle, 1, false, mMVPMatrix, 0);
        
       // Pass in the combined matrix.
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        mPolyhedrons[3].render(glEs20, this.mPerPixelLightShader);

	}
		
	/**
	 * Draws a point representing the mPosition of the light.
	 */
	private void drawLight()
	{
		final int pointMVPMatrixHandle = mLightShader.getUniformLoc("u_MVPMatrix");
        final int pointPositionHandle  = mLightShader.getAttribLoc("a_Position");
        
		// Pass in the mPosition.
		GLES20.glVertexAttrib3f(pointPositionHandle, mLightPosInModelSpace[0], mLightPosInModelSpace[1], mLightPosInModelSpace[2]);

		// Since we are not using a buffer object, disable vertex arrays for this attribute.
        GLES20.glDisableVertexAttribArray(pointPositionHandle);  
		
		// Pass in the transformation matrix.
		Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mLightModelMatrix, 0);
		Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
		GLES20.glUniformMatrix4fv(pointMVPMatrixHandle, 1, false, mMVPMatrix, 0);
		
		// Draw the point.
		GLES20.glDrawArrays(GLES20.GL_POINTS, 0, 1);
	}
}
