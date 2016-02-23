#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
in  vec3 light;
in  vec2 inTexCoord;
uniform mat4 rotationMatrix;
uniform mat4 rotationMatrix2;
uniform mat4 translationMatrix;
uniform mat4 projMatrix;
out vec4 shade;
out vec2 intpTexCoord;


void main(void)
{
    //const vec3 light = vec3(0.58, 0.58, 0.58);
    shade = rotationMatrix * rotationMatrix2 * vec4(in_Normal,1.0);
	gl_Position = rotationMatrix * rotationMatrix2 * vec4(in_Position, 1.0);
    intpTexCoord = inTexCoord;
    
}
