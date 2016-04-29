#Minimal cmake example for linking opencv
CMAKE_MINIMUM_REQUIRED(VERSION 2.0)
PROJECT(TSBB15G1)

set(sources lab1-1.c MicroGlut.m GL_utilities.c)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -std=c99")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g")

#Set appropriate linking flags.
#set(GCC_COVERAGE_LINK_FLAGS    "-lgcov") 
#set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

#finds the opengl package somewhere on the system
IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT )
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     GLUT_LIBRARY
                     OpenGL_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

#add_subdirectory (bin)
#defines an executable, if needed one cmake script can generate more than one file
#more source files can be added after the first one
ADD_EXECUTABLE(mainfram_3000 ${sources})

#Specifies linking with needed libraries, more libraries can be added.
TARGET_LINK_LIBRARIES(mainfram_3000 ${EXTRA_LIBS})