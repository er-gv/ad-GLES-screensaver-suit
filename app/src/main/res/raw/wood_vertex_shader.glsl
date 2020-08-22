uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.      		       
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view/ matrix.
     		
		  	
uniform float u_LuminumScale;   // A factor that intensify or dim the light intensity.
uniform vec3  u_LightPos;       //represent the light mPosition in world coordinates.
		  			
attribute vec3 a_Position;		// Per-vertex mPosition information we will pass in.
attribute vec3 a_Normal;		// Per-vertex normal information we will pass in.
attribute vec2 a_TexCoordinate; // Per vertex texture coordinate.        		
		  
//varying vec3  v_ModelPosition;  // interpolation of spatial data in eye coords.       		
varying vec3  v_Normal;	// let the normal values to intepolate in the fragment shader.          		
varying vec2 v_TexCoordinate;   	
		  
// The entry point for our vertex shader.  
void main()                                                 	
{             
    // Transform the vertex into eye space and let the fragment shader to deal with the interpolated data.
	vec4 Position4 = vec4(a_Position, 1.0);                                            
	//vec3 EyePosition = vec3(u_MVMatrix * Position4);            
	
	//
	//vec3 tnorm = normalize(u_MNormalsMatrix* a_Normal); 	
	//v_LightIntensity = dot(normalize(u_LightPos - EyePosition), tnorm);
	//v_LightIntensity *= 1.5;
	      
	// transfor vertex spatial data through the MVP transform and assign to gl_Position.
	gl_Position = u_MVPMatrix * Position4;
	
	//pass the texture coordinates as is.
	v_TexCoordinate = a_TexCoordinate;
	
	//pass the normal coordinates so they interpolate at the frament shader. We use that to do Phong lighting
	v_Normal = a_Normal;                       		  
}                                                          