#version 150

//uniform mat4 rotationMatrix;
//uniform mat4 rotationMatrix2; //z-axis
//uniform mat4 translationMatrix;


in  vec3 in_Position;
in  vec3 in_Normal;
// Phong
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
//End Phong
in  vec2 inTexCoord;
uniform mat4  Camera;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
out vec3 ex_Normal;
out vec2 intpTexCoord;
out vec4 model_pos;
out vec3 s_pos[4];




void main(void)
{

    ex_Normal = normalize(mat3(Camera) * mat3(mdlMatrix) *  in_Normal);
	gl_Position = projMatrix * Camera * mdlMatrix * vec4(in_Position, 1.0);
    intpTexCoord = inTexCoord;
    
    model_pos = Camera * mdlMatrix * vec4(in_Position, 1.0);

//    for( int i = 0; i < 4; i++){
//        if(isDirectional[i] == false){
//        s_pos[i] = vec3(Camera * vec4(lightSourcesDirPosArr[i],1.0));
//        }
//        else{
//        s_pos[i] = mat3(Camera) * lightSourcesDirPosArr[i];
//        }
//  }
    s_pos[0] = vec3(Camera * vec4(10.0f, 5.0f, 0.0f,1.0));
    s_pos[1] = vec3(Camera * vec4(0.0f, 5.0f, 10.0f,1.0));
    s_pos[2] = mat3(Camera) * vec3(-1.0f, 0.0f, 0.0f);
    s_pos[3] = mat3(Camera) * vec3(0.0f, 0.0f, -1.0f);
    
    
    


    
}
