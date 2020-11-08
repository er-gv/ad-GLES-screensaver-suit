precision mediump float;
uniform vec3 u_Color;
void main() {
	if(vec3(0.0) == u_Color)
	gl_FragColor = vec4(1.0);
	else  gl_FragColor = vec4(u_Color, 1.0);
}                              