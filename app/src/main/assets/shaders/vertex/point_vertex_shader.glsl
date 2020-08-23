uniform mat4 u_MVPMatrix;
uniform float u_pointSize;
attribute vec3 a_Position;

varying vec3 v_Position;
void main()
{
	gl_Position = u_MVPMatrix * vec4( a_Position, 1.0f);
    v_Position = a_Position;
    gl_PointSize = (0.0<u_pointSize) ? u_pointSize : 25.0;
}