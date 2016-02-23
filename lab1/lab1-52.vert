#version 150

in  vec3 in_Position;
in  vec3 in_Color;
uniform mat4 myMatrix;
out vec3 ex_Color;

void main(void)
{
    ex_Color    = in_Color;
	gl_Position = myMatrix * vec4(in_Position, 1.0);
    
}
