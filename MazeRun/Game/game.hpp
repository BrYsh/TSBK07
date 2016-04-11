//
//  game.hpp
//  
//
//  Created by Andreas Brorsson on 2016-04-08.
//
//

#ifndef game_hpp
#define game_hpp

#include <iostream>
#include <string>
#include <stdio.h>
#include "../MG/MicroGlut.h"
#include "../MG/GL_utilities.h"
#include "../MG/VectorUtils3.h"
#include "../MG/loadobj.h"
#include "../MG/LoadTGA.h"
#include "../Maze/maze.hpp"


class Game
{
    
public:
    
    Game();
    //Model* GenerateTerrain(char type);
    GLfloat height_controll();
    void jump_controll();
//    void update_speed_dir();
//    void jump_check();
    void ball_dir();
    void update();
    mat4 update_camera();
//    void display(void);
    
    int a;
    int width_, height_;
    
    

    
    bool jump;
    bool turn;
    int jump_count;
    GLfloat ball_angle;
    GLfloat ball_speed;
    
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat z_pos;
    
    GLfloat x_pos_c;
    GLfloat y_pos_c;
    GLfloat z_pos_c;
    
    mat4 strans;
    GLfloat camera_control;
    
    Maze* maze;
    Model* current_track;
    
    Model* left_t;
    Model* leftleft_t;
    Model* leftright_t;
    
    Model* right_t;
    Model* rightright_t;
    Model* rightleft_t;
    

    

    
};

#endif /* game_hpp */
