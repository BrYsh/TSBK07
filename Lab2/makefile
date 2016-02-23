
all : lab2-1 lab2-2 lab2-3 lab2-4 lab2-5 lab2-6 

init=  -Wall -std=c99

fw= -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include MicroGlut.m GL_utilities.c loadobj.c LoadTGA.c



lab2-1: lab2-1.c GL_utilities.c 
	gcc $(init) -o lab2-1 $(fw) lab2-1.c  

lab2-2: lab2-2.c GL_utilities.c 
	gcc $(init) -o lab2-2 $(fw) lab2-2.c  

lab2-3: lab2-3.c GL_utilities.c 
	gcc $(init) -o lab2-3 $(fw) lab2-3.c VectorUtils3.c

lab2-4: lab2-4.c GL_utilities.c 
	gcc $(init) -o lab2-4 $(fw) lab2-4.c VectorUtils3.c

lab2-5: lab2-5.c GL_utilities.c 
	gcc $(init) -o lab2-5 $(fw) lab2-5.c VectorUtils3.c

lab2-6: lab2-6.c GL_utilities.c 
	gcc $(init) -o lab2-6 $(fw) lab2-6.c VectorUtils3.c



clean :
	rm lab2-1 lab2-2 lab2-3 lab2-4 lab2-5 lab 2-6

