

attribute vec4 MCvertex;
//attribute vec3 MCnormal;
uniform vec3 u_MCnormal;
uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;

uniform vec3 u_LightPosition;



varying float LightIntensity;
varying vec2 MCposition;

vec2 projectToXYPlane(vec4 vertex){
	vec3 deltaNormal = u_MCnormal-vec3(0.0, 0.0, 1.0);
	float len = length(deltaNormal);
	if(0.05>=len){
		return vertex.xy;
	}

	float a = deltaNormal.x/len;
	float b = deltaNormal.y/len;
	float c = deltaNormal.z/len;
	float d = sqrt(b*b+c*c);
	mat4 rortateX = mat4(
		vec4(1.0, 0.0, 0.0, 0.0),
		vec4(0, c/d, -b/d, 0.0),
		vec4(0, b/d, c/d, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);
	mat4 rortateY = mat4(
		vec4(d, 0.0, a, 0.0),
		vec4(0, 1.0, 0.0, 0.0),
		vec4(-a, 0.0, d, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);
	mat4 trasform = rortateY*rortateX;
	return (trasform*vertex).xy;

}

float computeLight(float ambiant){
	const float SpecularContribution = 0.3;
	const float DiffuseContribution = 1.0 - SpecularContribution;

	vec3 ecPosition = vec3(u_MVMatrix * MCvertex);
	vec3 tnorm = normalize(u_MCnormal);
	vec3 lightVec = normalize(u_LightPosition - ecPosition);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-ecPosition);
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	float spec = 0.0;

	if(diffuse > 0.0){
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, 16.0);
	}

	return ambiant+ DiffuseContribution * diffuse + SpecularContribution * spec;

}
void main(){
	
	//project the vertex to the xz plane. use the same transform needed to project the normal
	//to that plane
	LightIntensity = computeLight(0.1f);
	MCposition = projectToXYPlane(MCvertex);
	gl_Position = u_MVPMatrix * MCvertex;
}