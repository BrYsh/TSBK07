//
//  maze.hpp
//  
//
//  Created by Andreas Brorsson on 2016-04-07.
//
//

#ifndef maze_hpp
#define maze_hpp

#include <stdio.h>
#include <string>

#include "../MG/MicroGlut.h"
#include "../MG/GL_utilities.h"
#include "../MG/VectorUtils3.h"
#include "../MG/loadobj.h"
#include "../MG/LoadTGA.h"

class Maze
{
    
public:

    //Path left_P;
    //Path right_P;
    Maze(int dir,int t);
    Model* track;
    Maze* left;
    Maze* right;
    int tree;
    int global_dir;//  -> = 0  ^ = 1, <- = 2, ner = 3
    mat4 rot;
    mat4 trans;
    mat4 total;
    int length;
    int width;
    
    GLfloat xpos_n;
    GLfloat zpos_n;
    
    void generate_transform(GLfloat, GLfloat);
    void update_pos(mat4);
    mat4 get_total();
    void update_turn(GLfloat,GLfloat,GLfloat);
    
    mat4 global_trans;
    GLfloat global_angle;
    
    
private:
    
};




#endif /* maze_hpp */
