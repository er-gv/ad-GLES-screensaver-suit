//#version 120

uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.
uniform mat4 u_MVMatrix;		// A constant representing the combined model/view matrix.
uniform vec3 u_Normal;
attribute vec4 a_Position;		// Per-vertex mPosition information we will pass in.
attribute vec2 a_TexCoordinate; // Per-vertex texture coordinate information we will pass in.

varying vec3 v_Position;		// This will be passed into the fragment shader.
varying vec2 v_TexCoordinate;   // This will be passed into the fragment shader.
varying vec2 v_flat_pos;

vec2 getFlatPosition(vec3 pos){
    float len = length(u_Normal-vec3(0.0f,0.0f,1.0f));
    if(len<0.05f)
    return pos.xy;

    float a = u_Normal.x/len;
    float b = (u_Normal.y-1.0f)/len;
    float c = u_Normal.z/len;
    float d = sqrt(b*b+c*c);
    mat4 translate =
        mat4(1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        (vec3(0,0,1)-u_Normal),1);
    mat4 rotateX = mat4(
        vec4(1,0,0,0),
        vec4(0, c/d, b/d, 0),
        vec4(0, -b/d, c/d, 0),
        vec4(0,0,0,1)
    );
    mat4 rotateY = mat4(
        vec4(d,0,a,0),
        vec4(0, 1, 0, 0),
        vec4(-a, 0, d, 0),
        vec4(0,0,0,1)
    );
    mat4 transform = rotateY*rotateX*translate;
    return (transform*vec4(pos, 1)).xy;
}

// The entry point for our vertex shader.
void main()
{

    // Transform the vertex into eye space.
    v_Position = vec3(u_MVMatrix * a_Position);
    v_flat_pos = getFlatPosition(a_Position.xyz);

    // Pass through the texture coordinate.
    v_TexCoordinate = a_TexCoordinate;

    // gl_Position is a special variable used to store the final mPosition.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = u_MVPMatrix * a_Position;
}
