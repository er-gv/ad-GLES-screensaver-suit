
precision mediump float;

uniform sampler3D u_cubeMap;

varying vec3 v_reflectFactor;
varying vec3 v_refractFactor;
varying float v_ratio;


void main(){

	vec3 refractColor = vec3(texture3D(u_cubeMap, v_refractFactor));
	//vec3 reflectColor = vec3(texture3D(u_cubeMap, v_reflectFactor));
	
	//vec3 color = mix(refractColor, reflectColor, v_ratio);
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}