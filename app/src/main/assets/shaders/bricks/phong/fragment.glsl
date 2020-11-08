
precision mediump float;


varying vec2 v_projectedPosition;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec3 u_LightPosition;

uniform vec3 u_BrickColor;
uniform vec3 u_MortarColor;
uniform vec2 u_BrickSize;
uniform vec2 u_BrickPct;

float computeLight(float ambiant){
	const float SpecularContribution = 0.3;
	const float DiffuseContribution = 1.0 - SpecularContribution;

	vec3 tnorm = normalize(v_normal);
	//lets assume for nw that the light is at the center of the scean
	vec3 lightVec = normalize(v_position-u_LightPosition);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-v_position);
	float diffuse=0.0;
	if(gl_FrontFacing){
		diffuse= max(dot(lightVec, tnorm), 0.0);
	}
	else
		diffuse= max(dot(lightVec, -tnorm), 0.0);
	float spec = 0.0;

	if(diffuse > 0.0){
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, 16.0);
	}

	return ambiant+ DiffuseContribution * diffuse + SpecularContribution * spec;

}

vec3 getBrickPatternColor(){

	vec2 position = v_projectedPosition / u_BrickSize;
	if(fract(position.y * 0.5) > 0.5)
	position.x += 0.5;

	position = fract(position);
	vec2 useBrick = step(position, u_BrickPct);
	return mix(u_MortarColor, u_BrickColor, useBrick.x * useBrick.y);

}


void main(){


	float lightIntensity =computeLight(0.45);
	vec3 color = getBrickPatternColor();

	gl_FragColor = vec4(color*lightIntensity, 1.0);
}