//#version 300 es

//precision highp sample2D; // Set the default precision to medium. We don't need as high of a
precision highp float; // Set the default precision to medium. We don't need as high of a


uniform highp sampler2D u_Texture;    // The input texture.

uniform vec4 u_FaceColor;
uniform vec3 u_FaceNormal;         	// Interpolated normal for this fragment.
uniform float u_noiseScale;


varying vec3 v_Position;
varying vec2 v_TexCoord;
varying float v_LightCoaff;
varying float v_attenation;

//out vec4 FragColor;
//uniform float u_ambiantLight;


// The entry point for our fragment shader.
void main() {
    vec4 noise = texture2D(u_Texture, v_TexCoord*20.0);
    float t = min(1.0, noise[3]*3.5);
    gl_FragColor = u_FaceColor*vec4(vec3(t), 1.0);
    //*max(v_LightCoaff*v_attenation, u_ambiantCoaff)/**u_LightColor*/;
}

/*
    #version 300 es
#extension GL_OES_texture_3D : require

precision mediump sampler2D; // Set the default precision to medium. We don't need as high of a
precision mediump float; // Set the default precision to medium. We don't need as high of a

// precision in the fragment shader.
uniform sampler2D u_Texture;    // The input texture.
uniform vec3 u_LightPos;       	// The mPosition of the light in eye space.
uniform vec3 u_Normal;         	// Interpolated normal for this fragment.
uniform float u_noiseScale;

in vec3 v_Position;		// Interpolated mPosition for this fragment.
in vec2 v_TexCoord;   // Interpolated texture coordinate per fragment.

out vec4 FragColor;

float calcDiffuse(){
    float distance = length(u_LightPos - v_Position);

    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(u_LightPos - v_Position);

    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse;

    if (gl_FrontFacing) {
        diffuse = max(dot(-u_Normal, lightVector), 0.0);
    } else {
        diffuse = max(dot(u_Normal, lightVector), 0.0);
    }

    // Add attenuation.
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance)));
    return diffuse;
}

float calcSpecular(){

    vec3 lightVector = normalize(u_LightPos - v_Position);
    vec3 halfVec = normalize(u_LightPos + v_Position);

    float specular = max(pow(dot(halfVec, u_Normal),0.12), 0.0)*max(dot(u_Normal, lightVector), 0.0);

    return specular;
}
float PI_2 = 3.1415926/2.0;
float epsilon = 0.005;
// The entry point for our fragment shader.
void main()
{
    vec4 color1 = vec4( 0.0, 0.0, 0.0, 1.0 );
    vec4 color2 = vec4( 1.0, 1.0, 1.0, 1.0 );

    float light = 1.0;//(0.475*calcDiffuse() + 0.4*calcSpecular()+0.225);
    vec4 noise = texture2D(u_Texture, v_TexCoord*u_noiseScale);
    //float t = min(1.0, noise[3]*18.0);
    //vec4 color = vec4(vec3(light*t), 1.0);

    FragColor = vec4(1.0, 0.0,0.0,0.0);
}

*/