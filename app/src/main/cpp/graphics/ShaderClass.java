import java.util.ArrayList;

import com.learnopengles.android.common.ShaderHelper;

import android.opengl.GLES20;
import android.util.Log;

public class ShaderClass {

	private int mProgramHandle;
	private boolean mIsInitilized;
	private ArrayList<String> mAttribs;
	
	public ShaderClass() {
		mIsInitilized = false;
	}
	
	public boolean isContainsAttrib(String attribName){
		
		return mAttribs.contains(attribName);
	}
	/**
	 * Customer can use this in order to check if the shaderClass object is ready to use
	 * @return a boolean value indicating that the shader object was initialized and is ready for use.
	 */
	public boolean isInitilized(){
		return this.mIsInitilized;
	}
	/**
	 * Helper function to compile and link a program.
	 * 
	 * @param vertexShaderHandle An OpenGL handle to an already-compiled vertex shader.
	 * @param fragmentShaderHandle An OpenGL handle to an already-compiled fragment shader.
	 * @param attributes Attributes that need to be bound to the program.
	 * @return An OpenGL handle to the program.
	 */
	
	public void createAndLinkProgram(final String vertexShaderSrc, final String fragmentShaderSrc, final String[] attributes) 
	{
		//first compile vertex and fragment shaders
		final int vertexShaderHandle = ShaderHelper.compileShader(GLES20.GL_VERTEX_SHADER, vertexShaderSrc);		
		final int fragmentShaderHandle = ShaderHelper.compileShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderSrc);
		if(vertexShaderHandle == 0 || fragmentShaderHandle == 0){
			mProgramHandle = -1;
		}
		//if everything went OK we continue by compiling ad linking the program, adding attributes data.
		mProgramHandle = GLES20.glCreateProgram();
		
		if (mProgramHandle != 0) 
		{
			// Bind the vertex shader to the program.
			GLES20.glAttachShader(mProgramHandle, vertexShaderHandle);			

			// Bind the fragment shader to the program.
			GLES20.glAttachShader(mProgramHandle, fragmentShaderHandle);
			
			// Bind attributes
			if (attributes != null)
			{
				final int size = attributes.length;
				this.mAttribs = new ArrayList<String>();
				for (int i = 0; i < size; i++)
				{
					GLES20.glBindAttribLocation(mProgramHandle, i, attributes[i]);
					this.mAttribs.add(attributes[i]);
				}
				
				
			}
			
			// Link the two shaders together into a program.
			GLES20.glLinkProgram(mProgramHandle);

			// Get the link status.
			final int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(mProgramHandle, GLES20.GL_LINK_STATUS, linkStatus, 0);

			// If the link failed, delete the program.
			if (linkStatus[0] == 0) 
			{				
				Log.e("Shader", "Error compiling program: " + GLES20.glGetProgramInfoLog(mProgramHandle));
				GLES20.glDeleteProgram(mProgramHandle);
				mProgramHandle = 0;
			}
		}
		
		if (mProgramHandle == 0)
		{
			throw new RuntimeException("Error creating program.");
		}
		
		this.mIsInitilized = true;
	}
	
	/** 
	 * Helper function to compile a shader.
	 * 
	 * @param shaderType The shader type.
	 * @param shaderSource The shader source code.
	 * @return An OpenGL handle to the shader.
	 */
	private int compileShader(final int shaderType, final String shaderSource) {
		int shaderHandle = GLES20.glCreateShader(shaderType);

		if (shaderHandle != 0) 
		{
			// Pass in the shader source.
			GLES20.glShaderSource(shaderHandle, shaderSource);

			// Compile the shader.
			GLES20.glCompileShader(shaderHandle);

			// Get the compilation status.
			final int[] compileStatus = new int[1];
			GLES20.glGetShaderiv(shaderHandle, GLES20.GL_COMPILE_STATUS, compileStatus, 0);

			// If the compilation failed, delete the shader.
			if (compileStatus[0] == 0) 
			{
				Log.e("Shader", "Error compiling shader: " + GLES20.glGetShaderInfoLog(shaderHandle));
				GLES20.glDeleteShader(shaderHandle);
				shaderHandle = 0;
			}
		}

		if (shaderHandle == 0)
		{			
			throw new RuntimeException("Error creating shader.");
		}
		
		return shaderHandle;
	}

	
	
	public int getUniformLoc(String uniformName){
		int handler = GLES20.glGetUniformLocation(mProgramHandle, uniformName);
		return handler;
	}
	
	public int getAttribLoc(String attrName){
		int result;
		if(this.mAttribs.contains(attrName))
			result = GLES20.glGetAttribLocation(mProgramHandle, attrName);
		else
			result = -1;
		return result;
	}
	
	public void useShader(){
		GLES20.glUseProgram(this.mProgramHandle);
	}

}
