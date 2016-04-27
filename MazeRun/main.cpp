//
//  main.cpp
//  
//
//  Created by Andreas Brorsson on 2016-04-07.
//
//

#include <stdio.h>


#include "./Game/game.hpp"

#include "./MG/MicroGlut.h"
#include "./MG/GL_utilities.h"
#include "./MG/VectorUtils3.h"
#include "./MG/loadobj.h"
#include "./MG/LoadTGA.h"
#include "./MG/simplefont.h"
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

void init(void);
void timer(int i);
void GenerateTerrain(Maze*);
//GLfloat height_controll(GLfloat xf, GLfloat zf,Model* M);
void KeyEvent();
//void ball_dir();
void InitTerrain();

mat4 projectionMatrix, camera;
Model* sphere;
Model* LegR;
Model* LegL;
Model* Body;
Model* ArmR;
Model* ArmL;
Model* skybox;




GLfloat t = 0;


int width_ = 50, height_ = 17;
Game* game_ = new Game(width_,height_);


GLfloat tx = 0;
GLfloat ty = 0;
GLfloat tz = 0;
GLfloat lx = 0;
GLfloat ly = 0;
GLfloat lz = 0;
GLfloat yaw = 0.0;
GLfloat pitch = 0.0;
GLfloat speed = 0.0;
GLfloat strafex = 0.0;
GLfloat strafez = 0.0;

GLfloat time_count = 0.0;
GLuint time_count_out;
char time_count_out_str;
char buffer [50];

// vertex array object
Model *m, *m2;



// Reference to shader program
GLuint program, skyprogram;
GLuint tex1, tex2, tex_body, tex_head, skytex;
TextureData ttex; // terrain



void display(void)
{
    time_count += 0.02;
    
    

    
    t += 0.005;
    game_->world_dir();
    //KeyEvent();
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    snprintf(buffer, 50, "%f", time_count);
    
   // time_count_out_str = std::to_string(time_count_out);
    sfDrawString(30, 30, "Elapsed Time:");
    sfDrawString(170, 30, buffer);
    
    mat4 total, modelView, camMatrix;
    
    printError("pre display");
    
    glUseProgram(program);
    // Build matrix

    vec3 cam = {0, 5, 8};
    vec3 lookAtPoint = {2, 0, 2};
    camMatrix = lookAt(cam.x, cam.y, cam.z,
                       lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
                       0.0, 1.0, 0.0);
    modelView = IdentityMatrix();
    
    total = Mult(camMatrix, modelView);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_TRUE, camera.m);
    
    glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
    glActiveTexture(GL_TEXTURE0);

    
    
    
    
    
    
    //rotate ground
    mat4 rot = Ry(0);
    rot = Rx(0);
    mat4 trans =T(0, 0, 0);
    
    
    total = game_->maze->get_total();
    
    //Current
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->track, program, "inPosition", "inNormal", "inTexCoord");
    
    total = game_->maze->left->get_total();
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->left->track, program, "inPosition", "inNormal", "inTexCoord");
    
    total = game_->maze->right->get_total();
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->right->track, program, "inPosition", "inNormal", "inTexCoord");
    
    
    
    
    // skybox
    
    // Disable Z-buffer to draw sky
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // Use skytex as texture and switch to skyprogram
    glUseProgram(skyprogram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex);
    
    //Remove translation from skylookAt
    mat4 skylookAT = camera;
    skylookAT.m[3] = 0;
    skylookAT.m[7] = 0;
    skylookAT.m[11] = 0;
    
    //mat4 modelv = Mult(S(-0.8,1,1), modelView);
    
    // Send skylookAT to skyprogram
    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "lookAT"), 1, GL_TRUE, skylookAT.m);
    
    // Sky
    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "mdlMatrix"),1, GL_TRUE, modelView.m);
    DrawModel(skybox, skyprogram, "in_Position", NULL, "in_TexCoord");
    printError("sky shader");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    
    glUseProgram(program);
    
    
    
    
    
    
    
    //DrawModel(game_->right_track, program, "inPosition", "inNormal", "inTexCoord");

    //sphere
    glBindTexture(GL_TEXTURE_2D, tex2);
    glActiveTexture(GL_TEXTURE0);

    //    GLfloat y_pos_des = height_controll(x_pos, z_pos,&ttex,tm)+0.1;
    //    y_pos -= (y_pos - y_pos_des)/1*0.5;
    game_->update();

    //trans = T(0,0,0); // <<< --  Vart spheren är
    //rot = Rx(0);
    //rot = Ry(M_PI_2/4);
    //total = game_->player_->total_pos();//Mult(game_->strans, rot);

    camera = game_->update_camera();
    
    //Body
    total = game_->player_->body_total;
    
    glBindTexture(GL_TEXTURE_2D, tex_body);
    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(Body,program,"inPosition","inNormal","inTexCoord");
    
    //Rest of player init tex
    glBindTexture(GL_TEXTURE_2D, tex_head);
    glActiveTexture(GL_TEXTURE0);
    
    total = game_->player_->head_total;
    
    //head
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(Body,program,"inPosition","inNormal","inTexCoord");
    
    //arms
    total = game_->player_->arm_total_l;
    //lsft
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(ArmL,program,"inPosition","inNormal","inTexCoord");
    
    total = game_->player_->arm_total_r;
    //right
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(ArmR,program,"inPosition","inNormal","inTexCoord");
    
    //legs
    total = game_->player_->leg_total_l;
    //Left
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(LegL,program,"inPosition","inNormal","inTexCoord");
    
    total = game_->player_->leg_total_r;
    //Right
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(LegR,program,"inPosition","inNormal","inTexCoord");
    
    
    
   
    printError("display 2");
    
    glutSwapBuffers();
}




//void mouse(int x, int y)
//{
//	printf("%d %d\n", x, y);
//}



int main(int argc, char **argv)
{
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize (600, 600);
    glutCreateWindow ("MazeRun");
    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &timer, 0);
    printf("bpp %f\n", 1.0);
    glutMainLoop();
    
    
    exit(0);
}




void timer(int i)
{

    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

void GenerateTerrain(Maze* maze)
{
    GLfloat b;
    GLfloat skirt = 0;
    int x, z;
    int width_g = width_, height_g = height_;
    maze->length = width_g;
    maze->width = height_g;
    
    int vertexCount = width_g * height_g;
    int triangleCount = (width_g-1) * (height_g-1) * 2;
    
    
    GLfloat *vertexArray = new GLfloat[sizeof(GLfloat) * 3 * vertexCount];
    GLfloat *normalArray = new GLfloat[sizeof(GLfloat) * 3 * vertexCount];
    GLfloat *texCoordArray = new GLfloat[sizeof(GLfloat) * 2 * vertexCount];
    GLuint *indexArray = new GLuint[sizeof(GLuint) * triangleCount*3];
    
    GLfloat last_b;
    
    for (x = 0; x < width_g; x++)
        for (z = 0; z < height_g; z++)
        {
            if (z < 2 || z > height_g -3) {
                if (z == 0 || z == height_g -1) {
                    skirt = 3;
                }
                else
                    skirt = 1;
            }
            else
                skirt = 0;
            // Vertex array. You need to scale this properly
            vertexArray[(x + z * width_g)*3 + 0] = x / 1.0;
            if (x < width_g - height_g) {
                vertexArray[(x + z * width_g)*3 + 1] = 1.0*b - skirt; //+ x*0.1;
                last_b = vertexArray[(x + z * width_g)*3 + 1] + skirt;
            }
            else{
                vertexArray[(x + z * width_g)*3 + 1] = last_b - skirt;
            }
            vertexArray[(x + z * width_g)*3 + 2] = z / 1.0;
            
            // Beräkna normalvektorer
            vec3 vector1 = vec3(0,0,0);
            vec3 vector2 = vec3(0,0,0);
            vec3 normalvektor = vec3(0, 1, 0);
            
            if(x > 0 && z > 0){
                vector1.x = vertexArray[(x-1 + z * width_g)*3 + 0] - vertexArray[(x + z * width_g)*3 + 0];
                vector1.y = vertexArray[(x-1 + z * width_g)*3 + 1] - vertexArray[(x + z * width_g)*3 + 1];
                vector1.z = vertexArray[(x-1 + z * width_g)*3 + 2] - vertexArray[(x + z * width_g)*3 + 2];
                
                
                vector2.x = vertexArray[(x + (z-1) * width_g)*3 + 0] - vertexArray[(x + z * width_g)*3 + 0];
                vector2.y = vertexArray[(x + (z-1) * width_g)*3 + 1] - vertexArray[(x + z * width_g)*3 + 1];
                vector2.z = vertexArray[(x + (z-1) * width_g)*3 + 2] - vertexArray[(x + z * width_g)*3 + 2];
                
                
                normalvektor = CrossProduct(vector2, vector1);
            }
            
            
            // Normal vectors. You need to calculate these.
            normalArray[(x + z * width_g)*3 + 0] = normalvektor.x;
            normalArray[(x + z * width_g)*3 + 1] = normalvektor.y;
            normalArray[(x + z * width_g)*3 + 2] = normalvektor.z;
            
            // Texture coordinates. You may want to scale them.
            texCoordArray[(x + z * width_g)*2 + 0] = x; // (float)x / width_g;
            texCoordArray[(x + z * width_g)*2 + 1] = z; // (float)z / height_g;
        }
    for (x = 0; x < width_g-1; x++)
        for (z = 0; z < height_g-1; z++)
        {
            // Triangle 1
            indexArray[(x + z * (width_g-1))*6 + 0] = x + z * width_g;
            indexArray[(x + z * (width_g-1))*6 + 1] = x + (z+1) * width_g;
            indexArray[(x + z * (width_g-1))*6 + 2] = x+1 + z * width_g;
            // Triangle 2
            indexArray[(x + z * (width_g-1))*6 + 3] = x+1 + z * width_g;
            indexArray[(x + z * (width_g-1))*6 + 4] = x + (z+1) * width_g;
            indexArray[(x + z * (width_g-1))*6 + 5] = x+1 + (z+1) * width_g;
        }
    
    // End of terrain generation
    
    // Create Model and upload to GPU:
    
    maze->track = LoadDataToModel(
                                   vertexArray,
                                   normalArray,
                                   texCoordArray,
                                   NULL,
                                   indexArray,
                                   vertexCount,
                                   triangleCount*3);
    maze->length = width_g;
    maze->width = height_g;
 
}


void init(void)
{
    
    sphere = LoadModelPlus("./OBJ/groundsphere.obj");
    
    camera = lookAt(0.0,2.0,-8.0,
                    0.0,0.0,0.0,
                    0.0,1.0,0.0);
    
    
    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
    
    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 150.0);
    
    // Load and compile shader
    program = loadShaders("./Shader/terrain.vert", "./Shader/terrain.frag");
    skyprogram = loadShaders("./Shader/sky.vert", "./Shader/sky.frag");
    glUseProgram(program);
    printError("init shader");
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("./TGA/maskros512.tga", &tex1);
    LoadTGATextureSimple("./TGA/conc.tga", &tex2);
    

    
    // Load terrain data
    InitTerrain();
    
    LoadTGATextureData("./TGA/fft-terrain.tga", &ttex);
    printError("init terrain");
    
    LoadTGATextureSimple("./TGA/tex_01_lfy_weapon1out.tga", &tex_head);
    LoadTGATextureSimple("./TGA/red.tga", &tex_body);
    
    LegR = LoadModelPlus("./OBJ/LegR.obj");//Load Right Leg
    LegL = LoadModelPlus("./OBJ/LegL.obj");//Load Left Leg
    Body = LoadModelPlus("./OBJ/groundsphere.obj"); //Load Body & head
    ArmR = LoadModelPlus("./OBJ/armr.obj");//Load Right Arm
    ArmL = LoadModelPlus("./OBJ/arml.obj");//Load Right Arm
    
    skybox = LoadModelPlus("./OBJ/cubeplus.obj");//Load skybox
    
    // Load skybox data
    glUseProgram(skyprogram);
    
    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    LoadTGATextureSimple("./TGA/skybox2.tga", &skytex); 	// Texture unit 1
    
    glUseProgram(program);
    sfMakeRasterFont();
    
}


void InitTerrain(){
    
    GenerateTerrain(game_->maze);
    GenerateTerrain(game_->maze->left);
    GenerateTerrain(game_->maze->left->left);
    GenerateTerrain(game_->maze->left->right);
    GenerateTerrain(game_->maze->right);
    GenerateTerrain(game_->maze->right->right);
    GenerateTerrain(game_->maze->right->left);
 
    game_->maze->generate_transform();
    
    
}

