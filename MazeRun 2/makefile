CC1 += -std=c++11 -Wall -w

CC2 += -std=c99 -Wall -w

LDFLAGS += -DGL_GLEXT_PROTOTYPES -lm -framework OpenGL -framework Cocoa -I/opt/X11/include

OBJECTS = main.o ./Game/game.o ./Maze/maze.o ./Player/player.o  ./MG/MicroGlut.o ./MG/GL_utilities.o ./MG/VectorUtils3.o ./MG/loadobj.o ./MG/LoadTGA.o ./MG/simplefont.o 

#Huvudmal
mazerun: $(OBJECTS) Makefile
	g++ $(CC1) -o mazerun $(OBJECTS) $(LDFLAGS)

#Delmal
main.o: main.cpp
	g++ $(CC1) -c main.cpp

game.o: ./Game/game.cpp ./Game/game.hpp
	g++ $(CC1) -c ./Game/game.cpp

maze.o: ./Maze/maze.cpp ./Maze/maze.hpp
	g++ $(CC1) -c ./Maze/maze.cpp

player.o: ./Player/player.cpp ./Player/player.hpp
	g++ $(CC1) -c ./Player/player.cpp

MicroGlut.o: ./MG/MicroGlut.m ./MG/Microglut.h
	gcc $(CC2) -c ./MG/MicroGlut.m

GL_utilities.o: ./MG/GL_utilities.c ./MG/GL_utilities.h
	gcc $(CC2) -c ./MG/GL_utilities.c

VectorUtils3.o: ./MG/VectorUtils3.c ./MG/VectorUtils3.h
	gcc $(CC2) -c ./MG/VectorUtils3.c

loadobj.o: ./MG/loadobj.c ./MG/loadobj.h
	gcc $(CC2) -c ./MG/loadobj.c

LoadTGA.o: ./MG/LoadTGA.c ./MG/LoadTGA.h
	gcc $(CC2) -c ./MG/LoadTGA.c

simplefont.o: ./MG/simplefont.c ./MG/simplefont.h
	gcc $(CC2) -c ./MG/simplefont.c

clean:
	@ \rm -rf *.o *.gch core

zap: clean
	@ \rm -rf mazerun *~

