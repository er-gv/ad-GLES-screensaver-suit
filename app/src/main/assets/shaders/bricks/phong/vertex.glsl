

attribute vec4 MCposition;
attribute vec3 MCnormal;
uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;


varying vec2 v_projectedPosition;
varying vec3 v_position;
varying vec3 v_normal;

vec2 projectToXYPlane(){
	vec3 deltaNormal = MCnormal-vec3(0.0, 0.0, 1.0);
	float len = length(deltaNormal);
	if(0.05>=len){
		return MCposition.xy;
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
	return (trasform*MCposition).xy;

}


void main(){
	
	//project the vertex to the xz plane. use the same transform needed to project the normal
	//to that plane
	v_projectedPosition = projectToXYPlane();
	v_normal = vec3(u_MVMatrix * vec4(MCnormal, 1.0));
	v_position = vec3(u_MVMatrix * MCposition);
	gl_Position = u_MVPMatrix * MCposition;
	gl_PointSize = 8.0;

}