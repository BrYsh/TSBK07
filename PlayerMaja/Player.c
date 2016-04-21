// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>


mat4 projectionMatrix, camera;
Model* LegR;
Model* LegL;
Model* Body;
Model* ArmR;
Model* ArmL;

GLfloat x_pos = 10.0;
GLfloat y_pos = 5.0;
GLfloat z_pos = 10.0;

GLfloat x_pos_c = 0;
GLfloat y_pos_c = 0;
GLfloat z_pos_c = 0;

GLfloat t = 0;

GLfloat ball_angle = 0;
GLfloat ball_speed = 0;
bool jump = false;
int jump_count = 0;
bool duck = false;
int duck_count = 0;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
            
            
            
            // Beräkna normalvektorer
            vec3 vector1 = {0,0,0};
            vec3 vector2 = {0,0,0};
            vec3 normalvektor = {0, 1, 0};
            
            if(x > 0 && z > 0){
            vector1.x = vertexArray[(x-1 + z * tex->width)*3 + 0] - vertexArray[(x + z * tex->width)*3 + 0];
            vector1.y = vertexArray[(x-1 + z * tex->width)*3 + 1] - vertexArray[(x + z * tex->width)*3 + 1];
            vector1.z = vertexArray[(x-1 + z * tex->width)*3 + 2] - vertexArray[(x + z * tex->width)*3 + 2];
            
        
            vector2.x = vertexArray[(x + (z-1) * tex->width)*3 + 0] - vertexArray[(x + z * tex->width)*3 + 0];
            vector2.y = vertexArray[(x + (z-1) * tex->width)*3 + 1] - vertexArray[(x + z * tex->width)*3 + 1];
            vector2.z = vertexArray[(x + (z-1) * tex->width)*3 + 2] - vertexArray[(x + z * tex->width)*3 + 2];
            
           
            normalvektor = CrossProduct(vector2, vector1);
            }
            
            
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = normalvektor.x;
			normalArray[(x + z * tex->width)*3 + 1] = normalvektor.y;
			normalArray[(x + z * tex->width)*3 + 2] = normalvektor.z;
            
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:
    
	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

GLfloat height_controll(GLfloat xf, GLfloat zf,TextureData* tex,Model* M){
    
    int x = floor(xf);
    int z = floor(zf);
    
    GLfloat dx = xf - (GLfloat)x;
    GLfloat dz = zf - (GLfloat)z;
    
    GLfloat w11 = (1.0-dx)*(1.0-dz);
    GLfloat w12 = dx*(1.0-dz);
    GLfloat w21 = (1.0-dx)*dz;
    GLfloat w22 = dx*dz;
    
    GLfloat y11,y12,y21,y22;
    
    y11 = M->vertexArray[(x + z * tex->width)*3 + 1]; //M->vertexArray[((int)VertexIndex.x)*3 + 1];
    y12 = M->vertexArray[((x+1) + z * tex->width)*3 + 1]; //M->vertexArray[((int)VertexIndex.y)*3 + 1];
    y21 = M->vertexArray[(x + (z+1) * tex->width)*3 + 1]; //M->vertexArray[((int)VertexIndex.z)*3 + 1];
    y22 = M->vertexArray[((x+1) + (z+1) * tex->width)*3 + 1]; //M->vertexArray[((int)VertexIndex.w)*3 + 1];
    
    return (w11*y11 + w12*y12 + w21*y21 + w22*y22);
    
}



Point3D lightSourcesColorsArr[] = {
    {0.0f, 0.0f, 0.0f}, // Red light
    {0.0f, 0.0f, 0.0f}, // Green light
    {0.0f, 0.0f, 0.0f}, // Blue light
    {0.0f, 0.0f, 0.0f} }; // White light

GLfloat specularExponent[] = {10.0, 20.0, 40.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
GLint mult_tex_bool[] = {1};

Point3D lightSourcesDirectionsPositions[] = {
    {10.0f, 5.0f, 0.0f}, // Red light, positional
    {0.0f, 5.0f, 10.0f}, // Green light, positional
    {-1.0f, 0.0f, 0.0f}, // Blue light along X
    {0.0f, 0.0f, -1.0f} }; // White light along Z




GLfloat tx = 0;
GLfloat ty = 0;
GLfloat tz = 0;
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


void ball_dir(){
    
    if (jump || duck) {
        return;
    }
    if (glutKeyIsDown(' ')) {
        jump = true;
        jump_count = 0;
    }
    if (glutKeyIsDown('h')) {
        duck = true;
        duck_count = 0;
    }

    if(glutKeyIsDown(GLUT_KEY_LEFT)) {
        ball_angle -= 0.015;
    }
    else if(glutKeyIsDown(GLUT_KEY_RIGHT)) {
        ball_angle += 0.015;
    }
    if(glutKeyIsDown(GLUT_KEY_UP)) {
        if (ball_speed < 0.72) {
            ball_speed += 0.01;
        }
    }
    else if(glutKeyIsDown(GLUT_KEY_DOWN)) {
        if (ball_speed >= 0.01) {
            ball_speed -= 0.01;
        }
    }
    
}

GLfloat leg_state_l;
GLfloat leg_state_r;
mat4 leg_angle;
mat4 leg_trans;
mat4 leg_total_l;
mat4 leg_total_r;

GLfloat arm_state_l;
GLfloat arm_state_r;
mat4 arm_angle;
mat4 arm_angle_l;
mat4 arm_trans;
mat4 arm_trans_l;
mat4 arm_total_l;
mat4 arm_total_r;


mat4 leg_run_l(){
    if (!jump && !duck){
    if (leg_state_l < 10){
        leg_state_l++;
        leg_angle = Rx(M_PI/7);
        leg_trans = T(0, 0, -0.36);
    }
   else{
        leg_state_l++;
        leg_angle = Rx(-M_PI/7);
        leg_trans = T(0, 0, 0.36);
       if (leg_state_l == 20) {
           leg_state_l = 0;
       }
    }
    }
    else{
        leg_angle = Rx(0);
    leg_trans = T(0, 0,0);
}
    leg_total_l = Mult(leg_trans, leg_angle);
    return leg_total_l;
}

mat4 leg_run_r(){
    if (!jump && !duck){
    if (leg_state_r < 10){
        leg_state_r++;
        leg_angle = Rx(M_PI/7);
        leg_trans = T(0, 0, -0.36);
    }
    else{
        leg_state_r++;
        leg_angle = Rx(-M_PI/7);
        leg_trans = T(0, 0, 0.36);
        if (leg_state_r == 20) {
            leg_state_r = 0;
        }
    }
    }
    else{
        leg_angle = Rx(0);
        leg_trans = T(0, 0, 0);

    }
    leg_total_r = Mult(leg_trans, leg_angle);
    return leg_total_r;
}

mat4 arm_run_l(){
    if(!duck){
    if (arm_state_l < 10){
        arm_state_l++;
        arm_angle_l = Rz(0);
        arm_trans_l = T(x_pos+0.5, y_pos+1.2, z_pos);
    }
    
    else{
        arm_state_l++;
        arm_angle_l = Rz(-M_PI/4);
        arm_trans_l = T(x_pos+0.5, y_pos+1, z_pos);
    
        
        if (arm_state_l == 20) {
            arm_state_l = 0;
        }
    }
    }
    else{
        arm_angle_l = Rx(0);
        arm_trans_l = T(x_pos+0.5, y_pos+1.2, z_pos);
    }
    arm_total_l = Mult(arm_trans_l, arm_angle_l);
    return arm_total_l;
}

mat4 arm_run_r(){
    if(!duck){
    if (arm_state_r < 10){
        arm_state_r++;
        arm_angle = Rz(0);
        arm_trans = T(x_pos-0.5, y_pos+1.2, z_pos);
    }
    else{
        arm_state_r++;
        arm_angle = Rz(M_PI_2/2);
        arm_trans = T(x_pos-0.5, y_pos+1, z_pos);
        
        if (arm_state_r == 20) {
            arm_state_r = 0;
        }
    }
    }
    else{
        arm_angle = Rx(0);
        arm_trans = T(x_pos-0.5, y_pos+1.2, z_pos);
    }
    arm_total_r = Mult(arm_trans, arm_angle);
    return arm_total_r;
    
}



// vertex array object
Model *m, *m2, *tm;

// Reference to shader program
GLuint program;
GLuint tex1, tex2, tex3;
TextureData ttex; // terrain

void init(void)
{
    //initiate Leg things
    leg_state_l = 10;
    leg_state_r =0;
    leg_angle = Rx(0);
    leg_trans = T(0, 0, 0);
    leg_total_l = Mult(leg_trans,leg_angle);
    leg_total_r = Mult(leg_trans,leg_angle);
    
    //initiate Left Arm things
    arm_state_l = 0;
    arm_angle_l = Rx(0);
    arm_trans_l = T(0, 0, 0);
    arm_total_l = Mult(arm_trans_l,arm_angle_l);
   
    //initiate Right Arm things
    arm_state_r = 10;
    arm_angle = Rx(0);
    arm_trans = T(0, 0, 0);
    arm_total_r = Mult(arm_trans,arm_angle);

    LegR = LoadModelPlus("LegR.obj");//Load Right Leg
    LegL = LoadModelPlus("LegL.obj");//Load Left Leg
    Body = LoadModelPlus("groundsphere.obj"); //Load Body & head
    ArmR = LoadModelPlus("armr.obj");//Load Right Arm
    ArmL = LoadModelPlus("arml.obj");//Load Right Arm
    
    camera = lookAt(0.0,2.0,-8.0,
                    0.0,0.0,0.0,
                    0.0,1.0,0.0);
    

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
    LoadTGATextureSimple("tex_01_lfy_weapon1out.tga", &tex2);
    LoadTGATextureSimple("red.tga", &tex3);
	
// Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
    
    x_pos = x_pos + 1.07;
    z_pos = z_pos + 1.07;
    y_pos = height_controll(x_pos, z_pos,&ttex,tm)+0.1;

}


void display(void)
{
    t += 0.005;
    ball_dir();
    
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	mat4 total, modelView, camMatrix;
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	vec3 cam = {0, 5, -8};
	vec3 lookAtPoint = {2, 10, 2};
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
    mat4 rot = Ry(M_PI_2/4);
    rot = Rx(0);
    mat4 trans =T(0, 0, 0);
    total = Mult(trans, rot);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
    
    
    x_pos = x_pos + ball_speed*cos(ball_angle);
    z_pos = z_pos + ball_speed*sin(ball_angle);
    
    if (jump) {
        if (jump_count > 30) {
            y_pos -= 0.1*jump_count/20;
        }
        else{
            y_pos += 0.1*cos(jump_count*M_PI_2/60);
        }
        GLfloat control = height_controll(x_pos, z_pos,&ttex,tm);
        jump_count++;
        if (y_pos < control) {
            jump = false;
            y_pos = control;
        }
    }
    else{
    y_pos = height_controll(x_pos, z_pos,&ttex,tm);
    }
    
    
    if (duck) {
        if (duck_count < 50){
            y_pos = height_controll(x_pos, z_pos,&ttex,tm) - 0.5;
            duck_count++;
        }
        else {
            y_pos = height_controll(x_pos, z_pos,&ttex,tm);
        duck = false;
    }
    }
            
    

    mat4 strans = T(x_pos, y_pos, z_pos);
    trans = T(0,0,0);
    rot = Rx(0);
    rot = Ry(M_PI_2/4);
    total = Mult(strans, rot);
    
    x_pos_c = x_pos -7*cos(ball_angle);
    z_pos_c = z_pos -7*sin(ball_angle);
    GLfloat camera_control = height_controll(x_pos_c, z_pos_c,&ttex,tm) + 3;
    
    GLfloat dist = y_pos_c - camera_control;
    y_pos_c -= dist/2.0*0.25;
    
    camera = lookAt(x_pos_c,y_pos_c,z_pos_c,
                    x_pos,y_pos+3,z_pos,
                    0.0,1.0,0.0);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    
    //PLAYER
   
    //LEGS
    glBindTexture(GL_TEXTURE_2D, tex2);
    glActiveTexture(GL_TEXTURE0);
    
    total = Mult(strans, leg_run_r());
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(LegR,program,"inPosition","inNormal","inTexCoord");// Right Leg
    
    total = Mult(strans, leg_run_l());
    
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(LegL,program,"inPosition","inNormal","inTexCoord"); // Left Leg
    
    //BODY
    mat4 transB;
    mat4 scale;
    
    glScalef(0.08,0.10,0.10);
    transB = T(x_pos, y_pos + 0.5, z_pos);
    scale = S(0.5, 0.5, 0.5);
    total = Mult(transB, scale);
    
    glBindTexture(GL_TEXTURE_2D, tex3);
    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(Body,program,"inPosition","inNormal","inTexCoord"); // Body
    
    //HEAD
    if (duck) {
        transB = T(x_pos, y_pos+1, z_pos-0.9); //fixa bra med coord för banan
    }
    else{
    transB = T(x_pos, y_pos + 1.35, z_pos);
    }
    scale = S(0.2, 0.3, 0.3);
    total = Mult(transB, scale);
    
    glBindTexture(GL_TEXTURE_2D, tex2);
    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(Body,program,"inPosition","inNormal","inTexCoord"); // Head
    
    
    //ARMS
    scale = S(0.5, 0.5, 0.5);
    
    total = Mult(arm_run_r(), scale);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(ArmR,program,"inPosition","inNormal","inTexCoord"); // Right arm
   
    total = Mult(arm_run_l(), scale);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(ArmL,program,"inPosition","inNormal","inTexCoord"); // Left arm
    
    
	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Player Test");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);


	glutMainLoop();
	exit(0);
}
