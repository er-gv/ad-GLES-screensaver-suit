//#version 300 es

//precision highp sample2D; // Set the default precision to medium. We don't need as high of a
precision highp float; // Set the default precision to medium. We don't need as high of a


uniform highp sampler2D u_Texture;    // The input texture.

uniform vec4 u_FaceColor;
uniform vec3 u_FaceNormal;         	// Interpolated normal for this fragment.
uniform float u_noiseScale;


varying vec3 v_Position;
varying vec2 v_TexCoord;
varying float v_LightCoaff;
varying float v_attenation;

//out vec4 FragColor;
//uniform float u_ambiantLight;


// The entry point for our fragment shader.
void main() {
    vec4 noise = texture2D(u_Texture, v_TexCoord*20.0);
    float t = min(1.0, noise[3]*3.5);
    gl_FragColor = u_FaceColor*vec4(vec3(t), 1.0);
    //*max(v_LightCoaff*v_attenation, u_ambiantCoaff)/**u_LightColor*/;
}
