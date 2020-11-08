#version 300 es
#extension GL_OES_texture_3D : require

precision mediump sampler3D; // Set the default precision to medium. We don't need as high of a
precision mediump float; // Set the default precision to medium. We don't need as high of a

// precision in the fragment shader.
uniform sampler3D u_Texture;    // The input texture.
uniform vec3 u_LightPos;       	// The mPosition of the light in eye space.
uniform vec3 u_Normal;         	// Interpolated normal for this fragment.

in vec3 v_Position;		// Interpolated mPosition for this fragment.
in vec3 v_TexCoord;   // Interpolated texture coordinate per fragment.

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
    vec4 color1 = vec4( 0.8, 0.7, 0.0, 1.0 );
    vec4 color2 = vec4( 0.6, 0.1, 0.0, 1.0 );

    float light = (0.475*calcDiffuse() + 0.4*calcSpecular()+0.225);
    vec4 noise = texture(u_Texture, v_TexCoord);
    float t = abs(noise[0]-0.25)+abs(noise[1]-0.125)+abs(noise[2]-0.0625)+abs(noise[3]-0.03125);
    float osilation = sqrt(pow(v_TexCoord.x*PI_2, 2.0)+pow(v_TexCoord.y*PI_2, 2.0)+pow(v_TexCoord.z*PI_2, 2.0));
    vec4 color = mix( color1, color2, clamp(t, 0.0, 1.0));
    // Multiply the color by the diffuse illumination level and texture value to get final output color.
    FragColor = vec4(color.rgb , 1.0);
}

