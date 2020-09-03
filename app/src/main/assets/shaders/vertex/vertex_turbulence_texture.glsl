#version 300 es

uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.
uniform mat4 u_NormalMatrix;		// A constant representing the combined model/view matrix.

in vec3 a_Position;		// Per-vertex mPosition information we will pass in.
//in vec4 a_Normal;		// Per-vertex normal information we will pass in.
in vec3 a_TexCoord; // Per-vertex texture coordinate information we will pass in.

out vec3 v_Position;		// This will be passed into the fragment shader.
//out vec3 v_Normal;			// This will be passed into the fragment shader.
out vec3 v_TexCoord;   // This will be passed into the fragment shader.




void main()
{
    // Transform the vertex into eye space.
    vec4 pos4 = vec4(a_Position, 1.0);
    v_Position = vec3(u_MVMatrix * pos4);

    // Pass through the texture coordinate.
    v_TexCoord = a_TexCoord;

    // Transform the normal's orientation into eye space.
    //v_Normal = vec3(u_NormalMatrix * a_Normal);

    // gl_Position is a special variable used to store the final mPosition.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = u_MVPMatrix * pos4;
}