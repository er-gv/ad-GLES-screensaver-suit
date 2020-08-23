precision mediump float;       	// Set the default precision to medium. We don't need as high of a 
								// precision in the fragment shader.

uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.

uniform vec3 u_LightPos;
uniform sampler2D u_Spectrum;    // The spectrum texture.
uniform vec2 u_SpectrumCoords;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoordinate;

vec2 complex_squared(vec2 c){
    vec2 result;
    result.x = c.x*c.x-c.y*c.y;
    result.y = 2.0*c.x*c.y;
    return result;
}

float juliaTester(float threashold, int max_itr, vec3 resulotion){

	//first let's see if this pixel is a part of the Julia set. if not it will be black.

	vec2 z,c;
    c.x = (v_TexCoordinate.x - 0.5)*2.7-0.75;
    c.y = (v_TexCoordinate.y - 0.5)*2.7;////
    z = c;
    int i;
    float normelizedDist = 0.0;//
    for(i=0; i<max_itr; i++) {
        vec2 new_z = (complex_squared(z))+c;
        normelizedDist = (new_z.x * new_z.x) + (new_z.y * new_z.y);
		if(normelizedDist > threashold){
        	break;
        }//
        z = new_z;

    }
    if(i==max_itr) return 1.0;
    return float(i)/float(max_itr);

}
// The entry point for our fragment shader.

void main(){


    // Transform the normal's orientation into eye space.

    // Will be used for attenuation.
    float distance = length(u_LightPos - v_Position);                  
	
	// Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(u_LightPos - v_Position);              	

	// Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
	// pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(normalize(v_Normal), lightVector), 0.0);

	// Add attenuation. 
    diffuse = diffuse * (1.0 / (1.0 + (0.10 * distance)));
    
    // Add ambient lighting
    diffuse = diffuse + 0.5;
    //gl_FragColor = vec4(u_Color, 1.0f)*diffuse* texture2D(u_Texture, v_TexCoordinate);

    float mask = juliaTester(4.0, 65, vec3(0,0,0));
    float coord = u_SpectrumCoords.x+mask*u_SpectrumCoords.y;
  	gl_FragColor = texture2D(u_Spectrum, vec2(coord, 0)) *1.5;//
}

