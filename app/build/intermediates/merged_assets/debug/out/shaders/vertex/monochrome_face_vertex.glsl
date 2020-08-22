

uniform mat4 u_MVPMatrix;
attribute vec3 a_Position;
varying vec3 v_Position;

void main()
{
    gl_Position = u_MVPMatrix * vec4( a_Position, 1.0f);
    v_Position = a_Position;
    gl_PointSize = 15.0;
}