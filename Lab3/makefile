
all : lab3-1 

init=  -Wall -std=c99

fw= -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include MicroGlut.m GL_utilities.c loadobj.c LoadTGA.c VectorUtils3.c



lab3-1: lab3-1.c GL_utilities.c 
	gcc $(init) -o lab3-1 $(fw) lab3-1.c  

sky: sky.c GL_utilities.c
	gcc $(init) -o sky $(fw) sky.c 



clean :
	rm lab3-1 sky

