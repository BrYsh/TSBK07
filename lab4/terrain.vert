#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 Camera;
out vec2 texCoord;
out vec4 shade;
//out mat3 normalMatrix1;



// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
    shade = mdlMatrix * vec4(inNormal, 1.0);
	//normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix* Camera * mdlMatrix* vec4(inPosition, 1.0);
}
