
precision mediump float;

attribute vec4 MCposition;
attribute vec3 MCnormal;

uniform mat4 m_mvMatrix;
uniform mat4 m_mvpMatrix;
uniform mat4 m_textureMatrix;
uniform mat3 m_normalMatrix;

const float ETA = 0.66;
const float FRESNEL_POWER = 5.0;
const float F = ((1.0 -ETA)*(1.0 -ETA)) / ((1.0 +ETA)*(1.0 +ETA));

varying vec3 v_reflectFactor;
varying vec3 v_refractFactor;
varying float v_ratio;



void main(){

	vec4 ECposition = m_mvpMatrix* MCposition;
	vec3 ECposition3 = ECposition.xyz / ECposition.w;
	vec3 i = normalize(ECposition3);
	vec3 n = normalize(m_normalMatrix * MCnormal);
	
	v_ratio = F +(1.0 -F)*pow((1.0 -dot(-i, n)), FRESNEL_POWER);
	
	v_refractFactor = refract(i, n, ETA);
	v_refractFactor = vec3(m_textureMatrix* vec4(v_refractFactor, 1.0));
	
	v_reflectFactor = reflect(i, n);
	v_reflectFactor = vec3(m_textureMatrix* vec4(v_reflectFactor, 1.0));
	
	gl_Position = m_mvpMatrix * MCposition;

	
}