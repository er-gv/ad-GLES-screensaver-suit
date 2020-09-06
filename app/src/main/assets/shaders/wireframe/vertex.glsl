attribute vec3 a_Position;
uniform mat4 u_MVPMat;


void main() {
    gl_Position = u_MVPMat*vec4(a_Position, 1.0);
}
