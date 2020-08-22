// A constance represent the combined model/view/projection
uniform mat4 u_MVPMatrix;

attribute vec4 a_Position;		// Per-vertex mPosition information we will pass in.
attribute vec3 a_Color;			// Per-vertex color information we will pass in.

varying vec3 v_Color;			// This will be passed into the fragment shader.

// The entry point for our vertex shader.
void main()
{
	// Pass through the color.
	v_Color = a_Color;

	// gl_Position is a special variable used to store the final mPosition.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
	gl_Position = u_MVPMatrix * a_Position;
}