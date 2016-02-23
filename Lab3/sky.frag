#version 150


out vec4 out_Color;
in vec2 intpTexCoord;
uniform sampler2D skytex;


void main(void)
{
    out_Color = texture(skytex, intpTexCoord);
}
