
all : lab1-1 lab1-2 lab1-3 lab1-4 lab1-5 lab1-52 lab1-6

init=  -Wall -std=c99

fw= -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include MicroGlut.m GL_utilities.c 



lab1-1: lab1-1.c GL_utilities.c 
	gcc $(init) -o lab1-1 $(fw) lab1-1.c  

lab1-2: lab1-2.c GL_utilities.c 
	gcc $(init) -o lab1-2 $(fw) lab1-2.c  

lab1-3: lab1-3.c GL_utilities.c 
	gcc $(init) -o lab1-3 $(fw) lab1-3.c 
 
lab1-4: lab1-4.c GL_utilities.c 
	gcc $(init) -o lab1-4 $(fw) lab1-4.c 

lab1-5: lab1-5.c GL_utilities.c 
	gcc $(init) -o lab1-5 $(fw) lab1-5.c  

lab1-52: lab1-52.c GL_utilities.c 
	gcc $(init) -o lab1-52 $(fw) lab1-52.c  

lab1-6: lab1-6.c GL_utilities.c 
	gcc $(init) -o lab1-6 $(fw) loadobj.c lab1-6.c   

clean :
	rm lab1-1 lab1-2 lab1-3 lab1-4 lab1-5 lab1-52 lab1-6

