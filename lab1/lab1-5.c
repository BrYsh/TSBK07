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
#include <math.h>

// Globals
// Data would normally be read from files
GLfloat vertices[36*3] = {
    -0.5,-0.5,-0.5,     // 0
    -0.5,0.5,-0.5,		// 3
    0.5,0.5,-0.5,		// 2
    -0.5,-0.5,-0.5,     // 0
    0.5,0.5,-0.5,		// 2
    0.5,-0.5,-0.5,		// 1
    
    0.5,0.5,-0.5,		// 2
    -0.5,0.5,-0.5,		// 3
    -0.5,0.5,0.5,		// 7
    0.5,0.5,-0.5,		// 2
    -0.5,0.5,0.5,		// 7
    0.5,0.5,0.5,		// 6
    
    -0.5,-0.5,-0.5,     // 0
    -0.5,-0.5,0.5,		// 4
    -0.5,0.5,0.5,		// 7
    -0.5,-0.5,-0.5,     // 0
    -0.5,0.5,0.5,		// 7
    -0.5,0.5,-0.5,		// 3
    
    0.5,-0.5,-0.5,		// 1
    0.5,0.5,-0.5,		// 2
    0.5,0.5,0.5,		// 6
    0.5,-0.5,-0.5,		// 1
    0.5,0.5,0.5,		// 6
    0.5,-0.5,0.5,		// 5
    
    -0.5,-0.5,0.5,		// 4
    0.5,-0.5,0.5,		// 5
    0.5,0.5,0.5,		// 6
    -0.5,-0.5,0.5,		// 4
    0.5,0.5,0.5,		// 6
    -0.5,0.5,0.5,		// 7
    
    -0.5,-0.5,-0.5,     // 0
    0.5,-0.5,-0.5,		// 1
    0.5,-0.5,0.5,		// 5
    -0.5,-0.5,-0.5,     // 0
    0.5,-0.5,0.5,		// 5
    -0.5,-0.5,0.5		// 4
};

GLfloat colors[36*3] = {
    1.0, 0.0, 0.0,	// Red
    1.0, 0.0, 0.0,	// Red
    1.0, 0.0, 0.0,	// Red
    1.0, 0.0, 0.0,	// Red
    1.0, 0.0, 0.0,	// Red
    1.0, 0.0, 0.0,	// Red
    
    0.0, 1.0, 0.0,	// Green
    0.0, 1.0, 0.0,	// Green
    0.0, 1.0, 0.0,	// Green
    0.0, 1.0, 0.0,	// Green
    0.0, 1.0, 0.0,	// Green
    0.0, 1.0, 0.0,	// Green
    
    0.0, 0.0, 1.0,	// Blue
    0.0, 0.0, 1.0,	// Blue
    0.0, 0.0, 1.0,	// Blue
    0.0, 0.0, 1.0,	// Blue
    0.0, 0.0, 1.0,	// Blue
    0.0, 0.0, 1.0,	// Blue
    
    0.0, 1.0, 1.0,	// Cyan
    0.0, 1.0, 1.0,	// Cyan
    0.0, 1.0, 1.0,	// Cyan
    0.0, 1.0, 1.0,	// Cyan
    0.0, 1.0, 1.0,	// Cyan
    0.0, 1.0, 1.0,	// Cyan
    
    1.0, 0.0, 1.0,	// Magenta
    1.0, 0.0, 1.0,	// Magenta
    1.0, 0.0, 1.0,	// Magenta
    1.0, 0.0, 1.0,	// Magenta
    1.0, 0.0, 1.0,	// Magenta
    1.0, 0.0, 1.0,	// Magenta
    
    1.0, 1.0, 0.0,	// Yellow
    1.0, 1.0, 0.0,	// Yellow
    1.0, 1.0, 0.0,	// Yellow
    1.0, 1.0, 0.0,	// Yellow
    1.0, 1.0, 0.0,	// Yellow
    1.0, 1.0, 0.0,	// Yellow
};

// Perspective
#define near 1.0
#define far 40.0
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
GLfloat rotationMatrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
GLfloat rotationMatrix2[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };
GLfloat translationMatrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, -2.0f,
    0.0f, 0.0f, 0.0f, 1.0f };


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(15, &OnTimer, value);
}


// vertex array object
unsigned int vertexArrayObjID;

    GLuint program;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
    unsigned int colorBufferObjID;
	// Reference to shader program

	dumpInfo();
    glutTimerFunc(1, &OnTimer, 0);

	// GL inits
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClearColor(0.1,0.5,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &colorBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    
    // VBO for colour data
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
 	
    glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	// End of upload of geometry
	
	printError("init arrays");
}

GLfloat t = 0.0;

void display(void)
{
    t = t+0.1;
    

    printError("init shader");
	printError("pre display");
    // Matrixes
    
    rotationMatrix2[0] = cos(t/5.0);
    rotationMatrix2[1] = -sin(t/5.0);
    rotationMatrix2[4] = sin(t/5.0);
    rotationMatrix2[5] = cos(t/5.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix2"), 1, GL_TRUE, rotationMatrix2);
    rotationMatrix[5] = cos(t);
    rotationMatrix[6] = -sin(t);
    rotationMatrix[9] = sin(t);
    rotationMatrix[10] = cos(t);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
    

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
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
	glutMainLoop();
}
