#version 150

out vec4 outColor;
in vec2 texCoord;
in vec4 shade;
in vec3 exNormal;
uniform sampler2D tex;
in mat3 normalMatrix1;

void main(void)
{
    const vec3 light = vec3(1.0, 1.0, 1.0);
    float shade2 = dot(normalize(shade), vec4(light,1));
    outColor = vec4(shade2,shade2,shade2,1)/4+ 3*texture(tex, texCoord)/4;
}
