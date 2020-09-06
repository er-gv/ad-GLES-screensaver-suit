//#version 300 es
//precision mediump float;
uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;
uniform mat4 u_NormalMat;

uniform vec3 u_LightPos;
uniform vec3 u_FaceNormal;

uniform float u_ambiantCoaff;
uniform float u_diffuseCoaff;
uniform float u_specularCoaff;
uniform float u_shininess;


attribute vec3 a_Position;
attribute vec2 a_TexCoord;

varying vec3 v_Position;
varying vec2 v_TexCoord;
varying float v_LightCoaff;
varying float v_attenation;

void main()
{
    vec3 eyeCPos = vec3(u_MVMatrix*vec4(a_Position, 1.0)); //OK
    v_Position = eyeCPos;
    vec3 tnorm = normalize(vec3(u_NormalMat * vec4(u_FaceNormal, 1.0)));
    vec3 lightVec = normalize((u_MVMatrix*vec4(u_LightPos, 1.0)).xyz-eyeCPos);
    vec3 reflectVec = normalize(reflect(lightVec, tnorm));
    vec3 viewVec = -eyeCPos;

    float spec = pow(max(dot(reflectVec, viewVec), 0.0), u_shininess);
    float lightDist = length(u_LightPos-eyeCPos);
    v_attenation = 1.0/(0.32+0.2*lightDist*lightDist);
    v_LightCoaff = u_diffuseCoaff*max(dot(lightVec, tnorm), 0.0)+u_specularCoaff*spec;
    v_TexCoord = a_TexCoord;
    gl_Position = u_MVPMatrix*vec4(a_Position, 1.0);
}
/*#version 300 es

uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.
uniform mat4 u_NormalMatrix;		// A constant representing the combined model/view matrix.

in vec3 a_Position;		// Per-vertex mPosition information we will pass in.
//in vec4 a_Normal;		// Per-vertex normal information we will pass in.
in vec2 a_TexCoord; // Per-vertex texture coordinate information we will pass in.

out vec3 v_Position;		// This will be passed into the fragment shader.
out vec2 v_TexCoord;   // This will be passed into the fragment shader.

void main()
{
    vec4 pos4 = vec4(a_Position, 1.0);
    v_Position = vec3(u_MVMatrix * pos4);

    // Pass through the texture coordinate.
    v_TexCoord = a_TexCoord;

    gl_Position = u_MVPMatrix * pos4;
}*/