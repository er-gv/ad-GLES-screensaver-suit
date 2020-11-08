//#version 300 es

//precision highp sample2D; // Set the default precision to medium. We don't need as high of a
precision highp float; // Set the default precision to medium. We don't need as high of a


uniform highp sampler2D u_Texture;    // The input texture.
uniform vec3 u_DarkWood;
uniform vec3 u_LightWood;
uniform mat4 u_SliceMat;

uniform vec3 u_FaceNormal;


varying vec3 v_Position;
varying vec2 v_TexCoord;
varying float v_LightCoaff;
varying float v_attenation;


void main() {

    vec4 cylinder = u_SliceMat*vec4(v_TexCoord, 0.0, 1.0);////
    float dist = length(cylinder.xy);
    vec4 noise = texture2D(u_Texture, v_TexCoord*1.25);
    dist+= noise[2]+noise[3]+0.03125;

    float t = 1.0 - abs(fract(dist)*2.0 -1.0);
    t = smoothstep(0.2, 0.5, t);
    vec3 color = mix(u_DarkWood, u_LightWood, t);
    gl_FragColor = vec4(color, 1.0);//*v_LightCoaff;
}