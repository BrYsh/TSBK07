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
    void turn_controll();
    void check_turn_key();
//    void update_speed_dir();
//    void jump_check();
    void ball_dir();
    void update();
    void turn_update(std::string);
    mat4 update_camera();
//    void display(void);
    
    int a;
    int width_, height_;
    
    mat4 strans;
    mat4 total;
    
    

    
    bool jump;
    bool turn;
    int turn_wait = 0;
    int jump_count;
    int global_dir;
    
    GLfloat ball_angle;
    GLfloat ball_speed;
    GLfloat ball_speed_global;
    
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat z_pos;
    
    GLfloat x_pos_tot;
    GLfloat y_pos_tot;
    GLfloat z_pos_tot;
    
    GLfloat strafe;
    
    GLfloat x_pos_c;
    GLfloat y_pos_c;
    GLfloat z_pos_c;
    
    GLfloat turn_angle;
    GLfloat pol_dir;
    
    GLfloat camera_control_x;
    GLfloat camera_control_y;
    GLfloat camera_control_z;
    
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
