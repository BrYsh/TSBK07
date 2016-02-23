#version 150


in  vec3 in_Position;
in  vec2 inTexCoord;
uniform mat4  CameraSky;
uniform mat4  Camera;
out vec2 intpTexCoord;
uniform mat4 projMatrixSky;
uniform mat4 mdlMatrix;

void main(void)
{
	gl_Position =  projMatrixSky * CameraSky * mdlMatrix * vec4(in_Position, 1.0);
    intpTexCoord = inTexCoord;
}
