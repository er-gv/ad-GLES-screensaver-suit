#version 300 es

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
}