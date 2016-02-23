#version 150


out vec4 out_Color;
in vec3 ex_Normal;
in vec3 light;
in vec2 intpTexCoord;
uniform sampler2D tex;

// Phong

uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
//End Phong
//uniform vec3 lSDPA
in vec4 model_pos;
in vec3 s_pos[4];
vec3 cos_theta;
vec3 s_vec[4];


void main(void)
{
    cos_theta = vec3(0);
    for (int i=0; i < 4; i++) {
        if (isDirectional[i] == false) {
            s_vec[i] = normalize(s_pos[i] - vec3(model_pos));
        }
        else{
            s_vec[i] = normalize(s_pos[i]);
        }
        
        cos_theta += vec3(max(dot(ex_Normal,s_vec[i]),0))*lightSourcesColorArr[i]*0.25;

    }
    
    out_Color = vec4(0.3, 0.3, 0.3, 1) + vec4(cos_theta,1) + texture(tex,intpTexCoord);
    
}


