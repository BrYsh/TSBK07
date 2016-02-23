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



Model* sky;

// Perspective
#define near 1.0
#define far 100.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {
    2.0f*near/(right-left), 0.0f,                   (right+left)/(right-left),      0.0f,
    0.0f,                   2.0f*near/(top-bottom), (top+bottom)/(top-bottom),      0.0f,
    0.0f,                   0.0f,                   -(far + near)/(far - near),     -2*far*near/(far - near),
    0.0f,                   0.0f,                   -1.0f,                          0.0f };



void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(15, &OnTimer, value);
}




// vertex array object
unsigned int vertexArrayObjID;

unsigned int skyVertexArrayObjID;
unsigned int skyVertexBufferObjID;
unsigned int skyIndexBufferObjID;
unsigned int skyNormalBufferObjID;
unsigned int skyTexCoordBufferObjID;

mat4 rot;
mat4 trans;
mat4 total;

mat4 camera;
mat4 cameraSky;

GLfloat a = 0;
GLfloat b = 0;
GLfloat c = -0.5;
GLfloat d = 0;
GLfloat e = 0;
GLfloat f = 0;
GLfloat g = 0;
GLfloat h = 1;
GLfloat i = 0;


void KeyEvent(){
    if(glutKeyIsDown('i')) {
        e = e + 0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('k')) {
        e = e - 0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('j')) {
        f = f+0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('l')) {
        f = f-0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('w')) {
        c = c + 0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('s')) {
        c = c - 0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('a')) {
        a = a-0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    if(glutKeyIsDown('d')) {
        a = a+0.5;
        camera = lookAt(a,b,c,
                        d,e,f,
                        g,h,i);
    }
    cameraSky = camera;
    cameraSky.m[3] = 0;
    cameraSky.m[7] = 0;
    cameraSky.m[11] = 0;
}

GLuint programSky;
// Reference to texture
GLuint skytex;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
    camera = lookAt(a,b,c,
                    d,e,f,
                    g,h,i);
    
    cameraSky = camera;
    cameraSky.m[3] = 0;
    cameraSky.m[7] = 0;
    cameraSky.m[11] = 0;
    

	// Reference to shader program
    sky = LoadModelPlus("skybox.obj");

	dumpInfo();
    glutTimerFunc(1, &OnTimer, 0);

	// GL inits
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClearColor(0.1,0.5,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
    programSky = loadShaders("sky.vert", "sky.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
    
    glGenVertexArrays(1, &skyVertexArrayObjID);
    glGenBuffers(1, &skyVertexBufferObjID);
    glGenBuffers(1, &skyIndexBufferObjID);
    //glGenBuffers(1, &bunnyNormalBufferObjID);
    glGenBuffers(1, &skyTexCoordBufferObjID);
    
    glBindVertexArray(skyVertexArrayObjID);
    
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, skyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, sky->numVertices*3*sizeof(GLfloat), sky->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(programSky, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(programSky, "in_Position"));
    
    // VBO for normal data
    //glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    //glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    //glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sky->numIndices*sizeof(GLuint), sky->indexArray, GL_STATIC_DRAW);
    
    if (sky->texCoordArray != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, skyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, sky->numVertices*2*sizeof(GLfloat), sky->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(programSky, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(programSky, "inTexCoord"));
    }
    
    // VBO for normal data
    
    // Texture unit 0
    LoadTGATextureSimple("SkyBox512.tga", &skytex);

	
	printError("init arrays");
}

GLfloat t = 0.0;

void display(void)
{
    KeyEvent();
    t = t+0.01;

    
    glUseProgram(programSky);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "Camera"), 1, GL_TRUE, camera.m);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "CameraSky"), 1, GL_TRUE, cameraSky.m);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "projMatrixSky"), 1, GL_TRUE, projectionMatrix);
    
    // Global translation
    rot = Ry(0);
    trans = T(0, 0, -2);
    total = Mult(rot, trans);

    printError("init shader");
	printError("pre display");
    // Matrixes
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(culling);
    
    glUseProgram(programSky);
    //total = Mult(trans, rot);
    //total = Mult(trans,total);
    glUniformMatrix4fv(glGetUniformLocation(programSky, "mdlMatrix"), 1, GL_TRUE, total.m);
 
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex);
    glUniform1i(glGetUniformLocation(programSky, "skytex"), 0);
    glBindVertexArray(skyVertexArrayObjID);
    DrawModel(sky,programSky,"in_Position",NULL,"inTexCoord");

    glEnable(GL_DEPTH_TEST);
	
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
