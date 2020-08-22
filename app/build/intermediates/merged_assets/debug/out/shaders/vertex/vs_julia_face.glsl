uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.      		       
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.       		

attribute vec4 a_Position;		// Per-vertex mPosition information we will pass in.
attribute vec2 a_TexCoordinate; // Per-vertex texture coordinate information we will pass in.
attribute vec3 a_Normal;

varying vec3 v_Position;		// This will be passed into the fragment shader.
varying vec2 v_TexCoordinate;   // This will be passed into the fragment shader.
varying vec3 v_Normal;
// The entry point for our vertex shader.  
void main()
{
    // Transform the vertex into eye space.
    v_Position = vec3(u_MVMatrix * a_Position);

    // Pass through the texture coordinate.
    v_TexCoordinate = a_TexCoordinate;

    v_Normal = (u_MVMatrix * vec4(a_Normal,1.0)).xyz;
    gl_Position = u_MVPMatrix * a_Position;
}                                                          