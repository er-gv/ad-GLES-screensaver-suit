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

    float intencity = (gl_FrontFacing ? v_LightCoaff : u_ambiantCoaff);
    gl_FragColor = clamp((u_Color*v_LightCoaff)*(vec4(1.0, 1.0, 0.25, 1.0)*v_attenation), vec4(0.0), vec4(1));
}

