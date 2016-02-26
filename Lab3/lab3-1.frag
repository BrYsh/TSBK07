#version 150


out vec4 out_Color;
in vec3 ex_Normal;
in vec3 light;
in vec2 intpTexCoord;
uniform sampler2D tex;
uniform sampler2D tex0;
uniform sampler2D tex1;

// Phong

uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
uniform bool muti_tex_bool;
//End Phong
//uniform vec3 lSDPA
in vec4 model_pos;
in vec4 model_pos2;
in vec3 s_pos[4];
vec3 cos_theta;
vec3 s_vec[4];


void main(void)
{
    
    vec4 conc = texture(tex0,intpTexCoord.st);
    vec4 ground = texture(tex1,intpTexCoord.st);
    
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
    if (muti_tex_bool) {
        out_Color = vec4(0.3, 0.3, 0.3, 1) + vec4(cos_theta,1) + sin(model_pos2.x*2.0)*conc + (1.0-sin(model_pos2.x*2.0))*ground;
    }
    else{
        out_Color = vec4(0.3, 0.3, 0.3, 1) + vec4(cos_theta,1) + texture(tex,intpTexCoord);
    }
    
}


