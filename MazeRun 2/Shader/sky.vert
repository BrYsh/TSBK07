#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 ex_Normal;
out vec2 ex_TexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 lookAT;


void main(void)
{

	mat3 normalMatrix = mat3(mdlMatrix); // Cheap normal matrix
	ex_Normal = normalMatrix * in_Normal; // Phong

	ex_TexCoord = in_TexCoord;

	gl_Position = projMatrix * lookAT * mdlMatrix * vec4(in_Position, 1.0);
	
}
