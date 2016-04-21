all : Player

init=  -Wall -std=c99

fw= -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include MicroGlut.m GL_utilities.c loadobj.c LoadTGA.c VectorUtils3.c

Player: Player.c GL_utilities.c 
	gcc $(init) -o Player $(fw) Player.c  

clean :
	rm Player

