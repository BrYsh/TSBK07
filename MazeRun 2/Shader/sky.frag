#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(ex_Normal), light);


	// Shading only:
//	out_Color = vec4(shade, shade, shade, 1.0); // inColor;

	// Shading + texture
//	out_Color = shade * texture(texUnit, ex_TexCoord);

	// Texture only:
	out_Color = texture(texUnit, ex_TexCoord);


}
