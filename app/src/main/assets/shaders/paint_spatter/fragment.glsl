#version 300 es
#extension GL_OES_texture_3D : require

precision mediump sampler3D; // Set the default precision to medium. We don't need as high of a
precision mediump float; // Set the default precision to medium. We don't need as high of a

// precision in the fragment shader.
uniform sampler3D u_Texture;    // The input texture.
uniform vec3 u_LightPos;       	// The mPosition of the light in eye space.
uniform vec3 u_Normal;         	// Interpolated normal for this fragment.

uniform vec3 u_PaintColor;
uniform vec3 u_MaterialColor;

uniform float u_Threshold;

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
        diffuse = max(dot(u_Normal, lightVector), 0.0);
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

// The entry point for our fragment shader.
void main()
{

    vec4 noise = texture(u_Texture, v_TexCoord);
    vec3 color = u_MaterialColor;
    if (noise.g> u_Threshold) color = u_PaintColor;
    FragColor = vec4(color, 1.0);

    /*
    float light = (0.475*calcDiffuse() + 0.4*calcSpecular()+0.225);
    vec4 noise = texture(u_Texture, v_TexCoord);
    vec3 color = Material.Kd;
    if (noise.g> Threshold) color = PaintColor;
    gl_FragColor = vec4(color, 1.0);
    float t = (noise[0]+noise[1]+noise[2]+noise[3]+0.03125);
    vec4 color = mix( SkyColor, CloudColor, clamp( t + 0.25, 0.0, 1.0 ) );
    // Multiply the color by the diffuse illumination level and texture value to get final output color.
    FragColor = vec4(color.rgb , 1.0);*/
}


/*#version 120

// Uniforms for the Phong shading modeluniform

struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform sampler2D u_NoiseTex;// Input from the vertex shader
varying vec4 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoord;// The paint-spatter uniforms

uniform vec3 PaintColor = vec3(1.0);




void main(){
    vec4 noise = texture(NoiseTex, TexCoord);
    vec3 color = Material.Kd;
    if (noise.g> Threshold) color = PaintColor;
    gl_FragColor = vec4(color, 1.0);
}*/