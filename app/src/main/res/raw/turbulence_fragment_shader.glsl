precision mediump float;       	// Set the default precision to medium. We don't need as high of a 
								// precision in the fragment shader.

uniform mat3 u_MNormalsMatrix;	// A constant representing the combined normal transformation (inverse transpose of scale and rotation)  
uniform sampler2D u_Texture;    // The input texture.

uniform vec3 u_cloudColor;  //the sky and cloud colors
uniform vec3 u_skyColor;    //to mix
  

varying vec2 v_TexCoordinate;   // Interpolated texture coordinate per fragment.
varying vec3 v_Normal;         	// Interpolated normal for this fragment.
  
// The entry point for our fragment shader.
void main()                    		
{                              
    vec4 noiseVec =	texture2D(u_Texture, v_TexCoordinate);
    float intensity = noiseVec[0];
	
	//vec3 tnorm = normalize(u_MNormalsMatrix* v_Normal); 	
	//float lightIntensity; = dot(normalize(u_LightPos - EyePosition), tnorm);
	
	vec3 color = mix(u_skyColor, u_cloudColor, intensity);
	gl_FragColor = vec4(color, 1.0);                                  		
}                                                                     	

