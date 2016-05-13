#Minimal cmake example for linking opencv
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(TSBB15G1)

set(sources main.cpp ./Game/game.cpp ./Maze/maze.cpp ./Player/player.cpp ./MG/MicroGlut.m ./MG/GL_utilities.c ./MG/VectorUtils3.c ./MG/loadobj.c ./MG/LoadTGA.c ./MG/simplefont.c)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -w")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g")

#Set appropriate linking flags.
#set(GCC_COVERAGE_LINK_FLAGS    "-lgcov") 
#set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

#finds the opengl package somewhere on the system
IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     OpenGL_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

#add_subdirectory (bin)
#defines an executable, if needed one cmake script can generate more than one file
#more source files can be added after the first one
ADD_EXECUTABLE(mr ${sources})

#Specifies linking with needed libraries, more libraries can be added.
TARGET_LINK_LIBRARIES(mr ${EXTRA_LIBS})