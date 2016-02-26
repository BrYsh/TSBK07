// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include <math.h>




Model* m;
Model* n;
Model* p;
Model* q;
Model* ground;
Model* sky;
Model* bunny;
Model* teddy;
// Perspective
#define near 1.0
#define far 200.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {
    2.0f*near/(right-left), 0.0f,                   (right+left)/(right-left),      0.0f,
    0.0f,                   2.0f*near/(top-bottom), (top+bottom)/(top-bottom),      0.0f,
    0.0f,                   0.0f,                   -(far + near)/(far - near),     -2*far*near/(far - near),
    0.0f,                   0.0f,                   -1.0f,                          0.0f };


// The rotation matrices will be changed for animation
GLfloat rotationMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
GLfloat rotationMatrix2[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
GLfloat translationMatrix[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };



void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(15, &OnTimer, value);
}


mat4 rot;
mat4 trans;
mat4 total;
mat4 rotcam;

mat4 camera;
mat4 cameraSky;

GLfloat tx = 0;
GLfloat ty = 0;
GLfloat tz = -1;
GLfloat lx = 0;
GLfloat ly = 0;
GLfloat lz = 0;
GLfloat yaw = 0.0;
GLfloat pitch = 0.0;
GLfloat speed = 0.5;
GLfloat strafex = 0.0;
GLfloat strafez = 0.0;

void KeyEvent(){
    if(glutKeyIsDown('i')) {
        pitch = pitch + speed*0.1;
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
    }
    else if(glutKeyIsDown('k')) {
        pitch = pitch - speed*0.1;
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
    }
    else if(glutKeyIsDown('j')) {
        yaw = yaw - speed*0.1;
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
    }
    else if(glutKeyIsDown('l')) {
        yaw = yaw + speed*0.1;
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
    }
    
    if(glutKeyIsDown('w')) {
        
        float tlx = cos(yaw)*cos(pitch);
        float tly = sin(pitch);
        float tlz = sin(yaw)*cos(pitch);
        
        tx = tx + speed*lx;
        ty = ty + speed*ly;
        tz = tz + speed*lz;
    
        camera = lookAt(tx,ty,tz, tx + tlx ,ty + tly , tlz + tz , 0,1,0);

    }
    else if(glutKeyIsDown('s')) {
        float tlx = cos(yaw)*cos(pitch);
        float tly = sin(pitch);
        float tlz = sin(yaw)*cos(pitch);
        
        tx = tx + -speed*lx;
        ty = ty + -speed*ly;
        tz = tz + -speed*lz;
        
        camera = lookAt(tx,ty,tz, tx + tlx ,ty + tly , tlz + tz , 0,1,0);
    }
    else if(glutKeyIsDown('a')) {

        tx = tx + speed*strafex;
        tz = tz + speed*strafez;
        
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
        
    }
    else if(glutKeyIsDown('d')) {
        tx = tx + -speed*strafex;
        tz = tz + -speed*strafez;
        
        lx = cos(yaw) * cos(pitch);
        ly = sin(pitch);
        lz = sin(yaw) * cos(pitch);
        
        strafex = cos(yaw - M_PI_2);
        strafez = sin(yaw - M_PI_2);
        
        camera = lookAt(tx,ty,tz, tx + lx ,ty + ly , lz + tz , 0,1,0);
    }
}


GLuint program;
GLuint programSky;
// Reference to texture
GLuint conctex;
GLuint groundtex;
GLuint skytex;

Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
    {0.0f, 1.0f, 0.0f}, // Green light
    {0.0f, 0.0f, 1.0f}, // Blue light
    {1.0f, 1.0f, 1.0f} }; // White light

GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
GLint mult_tex_bool[] = {1};

Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
    {0.0f, 5.0f, 10.0f}, // Green light, positional
    {-1.0f, 0.0f, 0.0f}, // Blue light along X
    {0.0f, 0.0f, -1.0f} }; // White light along Z





void init(void)
{
	// vertex buffer object, used for uploading the geometry
    camera = lookAt(0.0,2.0,-8.0,
                    0.0,0.0,0.0,
                    0.0,1.0,0.0);
    
    cameraSky = camera;
    cameraSky.m[3] = 0;
    cameraSky.m[7] = 0;
    cameraSky.m[11] = 0;
    

	// Reference to shader program
    m = LoadModelPlus("windmill-walls.obj");
    n = LoadModelPlus("windmill-roof.obj");
    p = LoadModelPlus("blade.obj");
    q = LoadModelPlus("windmill-balcony.obj");
    ground = LoadModelPlus("ground2.obj");
    sky = LoadModelPlus("skybox.obj");
    teddy = LoadModelPlus("teddy.obj");
    bunny = LoadModelPlus("bunnyplus.obj");

	dumpInfo();
    glutTimerFunc(1, &OnTimer, 0);

	// GL inits
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClearColor(0.1,0.5,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
    programSky = loadShaders("sky.vert", "sky.frag");
	printError("init shader");
    
    // Texture unit 0
    LoadTGATextureSimple("grass.tga", &groundtex);
    LoadTGATextureSimple("dirt.tga", &conctex);
    LoadTGATextureSimple("SkyBox512.tga", &skytex);
    

    
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	
	printError("init arrays");
 
}

GLfloat t = 0.0;


void display(void)
{
    KeyEvent();
    t = t+0.01;
    rot = Ry(0);
    total = Mult(rot, trans);
    
    cameraSky = camera;
    cameraSky.m[3] = 0;
    cameraSky.m[7] = 0;
    cameraSky.m[11] = 0;
    


    
    glUseProgram(programSky);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "CameraSky"), 1, GL_TRUE, cameraSky.m);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "projMatrixSky"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "Camera"), 1, GL_TRUE, camera.m);
    
    // Global translation
    trans = T(0, 0, 0);
    total = Mult(rot, trans);

    printError("init shader");
	printError("pre display");
    // Matrixes
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);  //Borde den vara på ?????? <--------------
    

    total = Mult(trans, rot);
    total = Mult(trans,total);


    //Sky
    glDisable(GL_DEPTH_TEST);
    glUseProgram(programSky);
    trans = T(0,0,0);
    rot = Rx(0);
    total = Mult(rot, trans);
    glBindTexture(GL_TEXTURE_2D, skytex);
    glUniform1i(glGetUniformLocation(programSky, "skytex"), 0);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "mdlMatrix"), 1, GL_TRUE, total.m);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "CameraSky"), 1, GL_TRUE, cameraSky.m);
    DrawModel(sky,programSky,"in_Position",NULL,"inTexCoord");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix2"), 1, GL_TRUE, rotationMatrix2);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_TRUE, camera.m);
    

    
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    
    
    trans = T(0,0,0);
    rot = Rx(0);
    glUseProgram(program);
    mat4 gtrans = T(0, 0, 0);
    rot = Rz(M_PI/2);
    //Ground
    
    //

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,conctex);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,groundtex);
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(program, "tex1"), 1);
    mult_tex_bool[0] = 1;

    glUniform1iv(glGetUniformLocation(program, "muti_tex_bool"), 1, mult_tex_bool);
    
//    glGenTextures(0, &conctex);
//    glGenTextures(1, &groundtex);
    
    mult_tex_bool[0] = 0;
    //
    //glUniform1i(glGetUniformLocation(program, "tex"), 0);

    total = Mult(gtrans, rot);
    total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(ground,program,"in_Position","in_Normal","inTexCoord");
    //
    glUniform1iv(glGetUniformLocation(program, "muti_tex_bool"), 1, mult_tex_bool);
    
    glBindTexture(GL_TEXTURE_2D, conctex);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "conctex"), 0);
    rot = Rx(0);
    // mill
    total = Mult(rot, trans);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(m,program,"in_Position","in_Normal","inTexCoord");
    
    // Taket
    total = Mult(rot, trans);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(n,program,"in_Position","in_Normal","inTexCoord");
    
    // Balkong
    total = Mult(rot, trans);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(q,program,"in_Position","in_Normal","inTexCoord");

    
    
    
    mat4 brot = Rx(t*M_PI);
    mat4 btrans = T(4.5, 9.5, 0);
    trans = T(0, 0, 0);
    glUseProgram(program);
    // Blades
    rot = Mult(brot,Rx(M_PI));
    total = Mult(btrans, rot);
    total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(p,program,"in_Position","in_Normal","inTexCoord");
    
    // Blades
    //trans = T(4, 10.5, -0.9);
    rot = brot;
    total = Mult(btrans, rot);
    total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(p,program,"in_Position","in_Normal","inTexCoord");
    
    // Blades
    //trans = T(4, 10.0, -0.9);
    rot = Mult(brot,Rx(M_PI/2));
    total = Mult(btrans, rot);
    total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(p,program,"in_Position","in_Normal","inTexCoord");
    
    // Blades
    //trans = T(4, 10.7, 0.3);
    rot = Mult(brot,Rx(-M_PI/2));
    total = Mult(btrans, rot);
    total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(p,program,"in_Position","in_Normal","inTexCoord");
    
    
    //Bunny
    mat4 ttrans = T(0,0,10);
    rot = Rx(0);
    total = Mult(ttrans, rot);
    total = Mult(trans,total);
    
    glBindTexture(GL_TEXTURE_2D, skytex);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny,program,"in_Position","in_Normal","inTexCoord");
    
    ttrans = T(0,0,8);
    rot = Rx(0);
    total = Mult(ttrans, rot);
    total = Mult(trans,total);
    //Teddy
    glBindTexture(GL_TEXTURE_2D, conctex);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "conctex"), 0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(teddy,program,"in_Position","in_Normal","inTexCoord");
    
	
	printError("display");
	
	glutSwapBuffers();                  // Swap
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
    initKeymapManager();
	glutMainLoop();
    
}
