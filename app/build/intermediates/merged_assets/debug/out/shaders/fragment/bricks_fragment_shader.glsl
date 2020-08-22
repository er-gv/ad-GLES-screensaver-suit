
precision mediump float;

varying float LightIntensity;
varying vec2 MCposition;

uniform vec3 u_BrickColor;
uniform vec3 u_MortarColor;

uniform vec2 u_BrickSize;
uniform vec2 u_BrickPct;

void main(){

	vec3 color;
	vec2 position, useBrick;
	position = MCposition / u_BrickSize;
	if(fract(position.y * 0.5) > 0.5)
		position.x += 0.5;
		
	position = fract(position);
	useBrick = step(position, u_BrickPct);
	
	color = mix(u_MortarColor, u_BrickColor, useBrick.x * useBrick.y);
	//color *= LightIntensity;
	gl_FragColor = vec4(color, 1.0);
}