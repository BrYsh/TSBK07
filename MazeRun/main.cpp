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

#include <time.h>       /* time */


void init(void);
void timer(int i);
void GenerateTerrain(Maze*);
void NewTerrain(GLfloat);
void KeyEvent();
void InitTerrain();
void draw_boost(Maze* maze);
void draw_obsticle(Maze* maze);

mat4 projectionMatrix, camera;
Model* sphere;
Model* LegR;
Model* LegL;
Model* Body;
Model* ArmR;
Model* ArmL;
Model* skybox;
Model* Lava;
Model* Star;
Model* SpeedObj;
Model* Imortal;
Model* Gate;

GLfloat t = 0;

int width_ = 50, height_ = 17;
Game* game_ = new Game(width_,height_-1);

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
int time_boost = 0;
char buffer [50];


// Reference to shader program
GLuint program, skyprogram;
GLuint tex1, tex2, tex_body, tex_head, skytex, lavatex,startex, speedtex, imortaltex, gatetex;
TextureData ttex; // terrain
mat4 skytot, lavatot;



void display(void)
{
    //std::cout << "time boost = " << time_boost << std::endl;
    if (time_boost >= 1) {
        time_boost -= 1;
        if (time_boost < 1) {
            std::cout << "BOOST OFF!" << std::endl;
            game_->ball_speed = game_->ball_speed_global;
            game_->imortality = false;
        }
    }
    
    
    //printf("d1 \n");

    
    t += 0.005;
    game_->world_dir();
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    mat4 total, modelView, camMatrix;
    
    printError("pre display");
    
    glUseProgram(program);
    // Build matrix

    vec3 cam = vec3(0, 5, 8);
    vec3 lookAtPoint = vec3(2, 0, 2);
    camMatrix = lookAt(cam.x, cam.y, cam.z,
                       lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
                       0.0, 1.0, 0.0);
    modelView = IdentityMatrix();
    total = Mult(camMatrix, modelView);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_TRUE, camera.m);
    
    // skybox
    
    // Disable Z-buffer to draw sky
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // Use skytex as texture and switch to skyprogram
    glUseProgram(skyprogram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex);
    
    
    // Sky
    mat4 skylookAT = camera;
    skylookAT.m[3] = 0;
    skylookAT.m[7] = 0;
    skylookAT.m[11] = 0;
    
    skylookAT = Mult(T(-0.2,0,0), skylookAT);
    mat4 skyrot = Ry(game_->turn_angle*game_->pol_dir);
    skytot = Mult(skytot, skyrot);

    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "lookAT"), 1, GL_TRUE, skylookAT.m);
    
    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "mdlMatrix"),1, GL_TRUE, skytot.m);
    DrawModel(skybox, skyprogram, "in_Position", NULL, "in_TexCoord");
    printError("sky shader");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glUseProgram(program);

    
    glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
    glActiveTexture(GL_TEXTURE0);
    
    
    // game clock
    if (!game_->dead) {
        // game clock
        time_count += 0.02;
        snprintf(buffer, 50, "%f", time_count);
        sfDrawString(30, 30, "Elapsed Time:");
        sfDrawString(170, 30, buffer);
        if (game_->ball_speed_global < 5) {
            game_->ball_speed_global += 0.0002;
        }
        
    }
    
    
    
    //Current
    total = game_->maze->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->track, program, "inPosition", "inNormal", "inTexCoord");
    
    //left
    total = game_->maze->left->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->left->track, program, "inPosition", "inNormal", "inTexCoord");
    
    //left->left
    total = game_->maze->left->left->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->left->left->track, program, "inPosition", "inNormal", "inTexCoord");
    
    //left->right
    total = game_->maze->left->right->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->left->right->track, program, "inPosition", "inNormal", "inTexCoord");
    
    
    //right
    total = game_->maze->right->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->right->track, program, "inPosition", "inNormal", "inTexCoord");
    //right->left
    total = game_->maze->right->left->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->right->left->track, program, "inPosition", "inNormal", "inTexCoord");
    //right->right
    total = game_->maze->right->right->get_total();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(game_->maze->right->right->track, program, "inPosition", "inNormal", "inTexCoord");
    
    
    


    //sphere
    glBindTexture(GL_TEXTURE_2D, tex2);
    glActiveTexture(GL_TEXTURE0);

    //    GLfloat y_pos_des = height_controll(x_pos, z_pos,&ttex,tm)+0.1;
    //    y_pos -= (y_pos - y_pos_des)/1*0.5;
    game_->update();
    if (game_->generate_terrain_bool) {
        NewTerrain(game_->x_pos);
        game_->generate_terrain_bool = false;
    }

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
    
    
    //Lava
    glBindTexture(GL_TEXTURE_2D, lavatex);
    glActiveTexture(GL_TEXTURE0);
    
    lavatot = Mult(T(0.0,(game_->y_pos_t),0.0),lavatot);
    lavatot = Mult(lavatot, skyrot);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, lavatot.m);
    DrawModel(Lava,program,"inPosition","inNormal","inTexCoord");
    
    
    
    //BOOSTERS
    // ---------------------- ______________------------------
    //printf("b1 \n");
    
    bool b_col = game_->maze->b_collision;
    // printf("bool b_collision: %d \n", b_col );
    
    if (b_col == false){
        
        game_->boost_collision();
        draw_boost(game_->maze);
        b_col = game_->maze->b_collision;
        if (b_col == true) { // if collision give points
            time_count += 100;
            time_boost = (3*(1+game_->ball_speed))*(1/0.02);
            std::cout << "tB = " << time_boost << std::endl;
            if (game_->maze->boost == 1) { //if speed
                game_->ball_speed += 1;
            }
            else{ // if imortality
                game_->imortality = true;
            }
        }
        
    }

    draw_boost(game_->maze->right);
    draw_boost(game_->maze->left);
    
    

    
    //  printf("obstacle main%d \n", obstacle );
    //  printf("obstaclex main %d \n", obstacle_x_pos );

    draw_obsticle(game_->maze);
        draw_obsticle(game_->maze->left);
        draw_obsticle(game_->maze->right);
    
    
    if (game_->dead) {
        sfSetRasterSize(600, 200);
        sfDrawString(250, 90, "GAME OVER");
        
        GLfloat time_score = time_count;
        snprintf(buffer, 50, "%f", time_score);
        sfDrawString(220, 110, "Score: ");
        sfDrawString(290, 110, buffer);
    }

   
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
    
    int pit_start = 2000;
    
    if (maze->obstacle == 0) {
        pit_start = maze->obstacle_x_pos - 5;
    }
    

    
    int x, z;
    int height_g = height_;
    int width_g = maze->length;
    maze->width = height_g-1;
    
    int vertexCount = width_g * height_g;
    int triangleCount = (width_g-1) * (height_g-1) * 2;
    
    
    GLfloat *vertexArray = new GLfloat[sizeof(GLfloat) * 3 * vertexCount];
    GLfloat *normalArray = new GLfloat[sizeof(GLfloat) * 3 * vertexCount];
    GLfloat *texCoordArray = new GLfloat[sizeof(GLfloat) * 2 * vertexCount];
    GLuint *indexArray = new GLuint[sizeof(GLuint) * triangleCount*3];
    
    GLfloat last_b, pit = 0;
    
    for (x = 0; x < width_g; x++)
        for (z = 0; z < height_g; z++)
        {
            //if (z < 2 || z > height_g -2) {
            if (z == 0 || z == height_g -1) {
                skirt = 1.5;
                //  }
                //else
                //  skirt = 1;
            }
            else{
                skirt = 0;
            }
            
            if (x > pit_start && x < pit_start + 10) {
                pit = 30;
            }
            else{
                pit = 0;
            }
            // Vertex array. You need to scale this properly
            vertexArray[(x + z * width_g)*3 + 0] = x / 1.0;
            if (x < width_g - height_g) {
                vertexArray[(x + z * width_g)*3 + 1] = 1.0*b - skirt - pit; //+ x*0.1;
                last_b = vertexArray[(x + z * width_g)*3 + 1] + skirt + pit;
            }
            else{
                vertexArray[(x + z * width_g)*3 + 1] = last_b - skirt - pit;
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
            texCoordArray[(x + z * width_g)*2 + 0] = 2.0*(float)x / width_g;
            texCoordArray[(x + z * width_g)*2 + 1] = 2.0*(float)z / height_g;
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
    
    printf("i1 \n");
    sfMakeRasterFont();
    
    sphere = LoadModelPlus("./OBJ/groundsphere.obj");
    
    camera = lookAt(0.0,2.0,-8.0,
                    0.0,0.0,0.0,
                    0.0,1.0,0.0);
    srand (time(NULL));
    
    skytot = T(0.0,0.0,0.0);
    lavatot = Mult(T(0,-1, 0), S(20, 0, 20));
    
    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
    
    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 100.0);
    
    printf("i2 \n");
    // Load and compile shader
    program = loadShaders("./Shader/terrain.vert", "./Shader/terrain.frag");
    skyprogram = loadShaders("./Shader/sky.vert", "./Shader/sky.frag");
    glUseProgram(program);
    printError("init shader");
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("./TGA/lavaroad.tga", &tex1);
    LoadTGATextureSimple("./TGA/conc.tga", &tex2);
    

    printf("i3 \n");
    // Load terrain data
    InitTerrain();
    
    LoadTGATextureData("./TGA/fft-terrain.tga", &ttex);
    printError("init terrain");
    
    //Body
    LoadTGATextureSimple("./TGA/tex_01_lfy_weapon1out.tga", &tex_head);
    LoadTGATextureSimple("./TGA/red.tga", &tex_body);
    
    LegR = LoadModelPlus("./OBJ/LegR.obj");//Load Right Leg
    LegL = LoadModelPlus("./OBJ/LegL.obj");//Load Left Leg
    Body = LoadModelPlus("./OBJ/groundsphere.obj"); //Load Body & head
    ArmR = LoadModelPlus("./OBJ/armr.obj");//Load Right Arm
    ArmL = LoadModelPlus("./OBJ/arml.obj");//Load Right Arm
    printf("i4 \n");
    //boost & Obst
    LoadTGATextureSimple("./TGA/red.tga", &startex);
    LoadTGATextureSimple("./TGA/speed.tga", &speedtex);
    LoadTGATextureSimple("./TGA/imortal.tga", &imortaltex);
    //LoadTGATextureSimple("./TGA/spike.tga", &gatetex);
    LoadTGATextureSimple("./TGA/dirt.tga", &gatetex); //NY ANDREAS

    Star = LoadModelPlus("./OBJ/groundsphere.obj");//extra points
    SpeedObj = LoadModelPlus("./OBJ/groundsphere.obj");//speed
    Imortal = LoadModelPlus("./OBJ/groundsphere.obj");//speed

    //Gate = LoadModelPlus("./OBJ/spike.obj");//gate
    Gate = LoadModelPlus("./OBJ/gatecube.obj");//gate //NY ANDREREAS

    
    
    // Lava Ground
    Lava = LoadModelPlus("./OBJ/ground.obj");//load ground aka lava
    LoadTGATextureSimple("./TGA/lava.tga", &lavatex); //load the lava texture
    
    //Sky
    skybox = LoadModelPlus("./OBJ/cubeplus.obj");//Load skybox
    glUseProgram(skyprogram);
    glUniformMatrix4fv(glGetUniformLocation(skyprogram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    LoadTGATextureSimple("./TGA/skybox2.tga", &skytex); 	// Texture unit 1
    glUseProgram(program);
    
}


void InitTerrain(){
    
    GenerateTerrain(game_->maze);
    GenerateTerrain(game_->maze->left);
    GenerateTerrain(game_->maze->left->left);
    GenerateTerrain(game_->maze->left->right);
    GenerateTerrain(game_->maze->right);
    GenerateTerrain(game_->maze->right->right);
    GenerateTerrain(game_->maze->right->left);
 
    game_->maze->generate_transform(0.0,-(height_-1.0)/2.0);
    
    
}


void NewTerrain(GLfloat pos){
    
    std::cout << "New Terrrain!!  KOM IHÅG ATT DESTUERA MODELLERNA NÄR MAN TILLSÄTTER NY!!!!!! " << std::endl;
    


    
    GenerateTerrain(game_->maze->left->left);
    GenerateTerrain(game_->maze->left->right);
    
    GenerateTerrain(game_->maze->right->right);
    GenerateTerrain(game_->maze->right->left);
    
    game_->maze->generate_transform(pos,-(height_-1.0)/2.0);
    //game_->maze->get_all_pos();
    
    
}


void draw_boost(Maze* maze){
    
    mat4 total;
    int boost = maze->boost;
    int boost_x_pos = maze->boost_x_pos;
    int boost_z_pos = maze->boost_z_pos;
    mat4 boost_pos = T(boost_x_pos, 5, boost_z_pos);
    
    if (boost == 0) {
        
        //Star
        glBindTexture(GL_TEXTURE_2D, startex);
        glActiveTexture(GL_TEXTURE0);
        total = maze->get_total();
        total = Mult(total, boost_pos); //T(20,5,4));
        total = Mult(total, Rx(M_PI_2));
        total = Mult(total, Rz(M_PI_2));
        total = Mult(total, S(0.5, 0.5, 0.5));
        
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(Star,program,"inPosition","inNormal","inTexCoord");
    }
    
    else if (boost == 1) {
        //speed
        glBindTexture(GL_TEXTURE_2D, speedtex);
        glActiveTexture(GL_TEXTURE0);
        total = maze->get_total();
        total = Mult(total, boost_pos); //T(5,5,8));
        //total = Mult(total, S(0.1, 0.1, 0.1));
        total = Mult(total, S(0.5, 0.5, 0.5));
        
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(SpeedObj,program,"inPosition","inNormal","inTexCoord");
    }
    
    
    else if ( boost == 2){
        //imortal
        glBindTexture(GL_TEXTURE_2D, imortaltex);
        glActiveTexture(GL_TEXTURE0);
        total = maze->get_total();
        total = Mult(total, boost_pos); //T(8,4.5,8));
        //total = Mult(total, S(0.3, 0.3, 0.3));
        total = Mult(total, S(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(Imortal,program,"inPosition","inNormal","inTexCoord");
    }
}

void draw_obsticle(Maze* maze){
    //gate
    
    //OBSTACLES
    int obstacle = maze->obstacle;
    int obstacle_x_pos = maze->obstacle_x_pos;
    mat4 obstacle_pos = T(obstacle_x_pos, 4, 8);
    
    if ( obstacle == 1){ //BYT ALLT ANDREAS
        //gate
        glBindTexture(GL_TEXTURE_2D, gatetex);
        glActiveTexture(GL_TEXTURE0);
        
        //hö pelare
        mat4 total = maze->get_total();
        total = Mult(total, obstacle_pos);
        total = Mult(total,S(2 , 6 , 2));
        total = Mult(total, T(0,-0.25,2));
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(Gate,program,"inPosition","inNormal","inTexCoord");
        
        //vänstra pelare
        total = maze->get_total();
        total = Mult(total, obstacle_pos);
        total = Mult(total,S(2 , 6 , 2));
        total = Mult(total, T(0,-0.25,-2));
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(Gate,program,"inPosition","inNormal","inTexCoord");
        
        //tak
        total = maze->get_total();
        total = Mult(total, obstacle_pos);
        total = Mult(total,S(2 , 2 , 6));
        total = Mult(total, T(0,0.27,0));
        total = Mult(total, Ry(M_PI_2));
        total = Mult(total, Rz(M_PI_2));
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(Gate,program,"inPosition","inNormal","inTexCoord");
        
        
    }
}

