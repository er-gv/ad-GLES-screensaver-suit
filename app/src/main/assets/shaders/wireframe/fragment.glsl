
uniform vec3 u_Color;

void main() {
    gl_FragColor = (vec3(0.0) == u_Color) ? vec4(0.0) : vec4(u_Color, 1.0);

}
