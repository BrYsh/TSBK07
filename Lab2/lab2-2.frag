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
	out_Color = texture(tex, intpTexCoord);
}
