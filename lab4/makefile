all : lab4-1 lab4-2 lab4-3 

init=  -Wall -std=c99

fw= -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include MicroGlut.m GL_utilities.c loadobj.c LoadTGA.c VectorUtils3.c

lab4-1: lab4-1.c GL_utilities.c 
	gcc $(init) -o lab4-1 $(fw) lab4-1.c  

lab4-2: lab4-2.c GL_utilities.c 
	gcc $(init) -o lab4-2 $(fw) lab4-2.c
 
lab4-3: lab4-3.c GL_utilities.c 
	gcc $(init) -o lab4-3 $(fw) lab4-3.c  

clean :
	rm lab4-1 lab4-2 lab4-3 

