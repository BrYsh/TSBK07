#version 150


out vec4 out_Color;
in vec4 shade;
out vec3 light;
in vec2 intpTexCoord;
uniform sampler2D tex;


void main(void)
{
    const vec3 light = vec3(0.58, 0.58, 0.58);
    float shade2 = dot(normalize(shade), vec4(light,1));
    float a = sin(intpTexCoord.s*30)/2+0.5;
    float b = sin(intpTexCoord.t*30)/2+0.5;
    out_Color = vec4(a, b, 1.0, 0.0);
}
