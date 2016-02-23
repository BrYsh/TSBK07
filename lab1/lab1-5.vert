#version 150

in  vec3 in_Position;
in  vec3 in_Color;
uniform mat4 rotationMatrix;
uniform mat4 rotationMatrix2;
uniform mat4 translationMatrix;
uniform mat4 projMatrix;

out vec3 ex_Color;

void main(void)
{
    ex_Color    = in_Color;
	gl_Position = projMatrix * translationMatrix * rotationMatrix * rotationMatrix2 *vec4(in_Position, 1.0);
    
}
