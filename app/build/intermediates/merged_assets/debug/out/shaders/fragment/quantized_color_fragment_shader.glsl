precision mediump float;       	// Set the default precision to medium. We don't need as high of a 
								// precision in the fragment shader.
varying vec3 v_Color;          	// This is the color from the vertex shader interpolated across the 
  								// triangle per fragment.
uniform float u_Quanta;  
// The entry point for our fragment shader.
void main()                    		
{                              	
	// Pass through the color
    gl_FragColor = vec4( (floor(v_Color *u_Quanta)/u_Quanta), 1.0);
}