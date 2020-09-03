precision mediump float;       	// Set the default precision to medium. We don't need as high of a
// precision in the fragment shader.

uniform float u_ambiantCoaff;
uniform vec4 u_Color;
uniform vec3 u_LightColor;
varying float v_LightCoaff;
varying float v_attenation;

//uniform float u_ambiantLight;


// The entry point for our fragment shader.
void main() {
    gl_FragColor = u_Color*max(v_LightCoaff*v_attenation, u_ambiantCoaff)/**u_LightColor*/;
}

