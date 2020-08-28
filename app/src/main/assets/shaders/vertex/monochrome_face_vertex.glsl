
precision mediump float;
uniform mat4 u_MVMatrix;
uniform mat4 u_MVPMatrix;
uniform mat4 u_NormalMat;
uniform vec3 u_LightPos;
uniform vec3 u_FaceNormal;

uniform float u_diffuseCoaff;
uniform float u_specularCoaff;
uniform float u_shininess;

attribute vec3 a_Position;

varying float v_LightCoaff;
varying float v_attenation;

void main()
{
    vec3 eyeCPos = vec3(u_MVMatrix*vec4(a_Position, 1.0)); //OK
    vec3 tnorm = normalize(vec3(u_NormalMat * vec4(u_FaceNormal, 1.0)));
    vec3 lightVec = normalize(u_LightPos-eyeCPos);  //OK
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec = normalize(vec3(u_NormalMat*vec4(a_Position, 1.0)));

    float spec = max(dot(tnorm, lightVec), 0.0);
    vec3 halfAngle = normalize(lightVec+viewVec);
    spec = spec*pow(max(dot(tnorm, halfAngle), 0.0), u_shininess);
    float lightDist = length(u_LightPos-eyeCPos);
    v_attenation = 1.0/(0.32+0.2*lightDist*lightDist);
    v_LightCoaff = u_diffuseCoaff*max(dot(lightVec, tnorm), 0.0)+u_specularCoaff*spec;

    gl_Position = u_MVPMatrix*vec4(a_Position, 1.0);
}