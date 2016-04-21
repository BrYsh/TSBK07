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
#include <stdlib.h>
#include "../MG/MicroGlut.h"
#include "../MG/GL_utilities.h"
#include "../MG/VectorUtils3.h"
#include "../MG/loadobj.h"
#include "../MG/LoadTGA.h"
#include "../Maze/maze.hpp"
#include "../Player/player.hpp"


class Game
{
    
public:
    
    Game(int,int);
    //GLfloat height_controll();
    void jump_controll();
    void turn_controll();
    void check_turn_key();

    void world_dir();
    void update();
    void turn_update(std::string);
    void update_maze_strafe();
    mat4 update_camera();
    void update_world();
    void strafe_back(GLfloat pace);
    
    int a;
    int width_, length_;
    int wait_key;
    
    mat4 world_trans;
    mat4 strafe_trans;
    mat4 total;

    
    bool jump;
    bool turn;
    int turn_wait = 0;
    int jump_count;
    int global_dir;
    int turn_steps;
    
    GLfloat world_angle;
    GLfloat ball_speed;
    GLfloat ball_speed_global;
    
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat y_pos_n;
    GLfloat z_pos;
    
    GLfloat x_pos_tot;
    GLfloat y_pos_tot;
    GLfloat z_pos_tot;
    
    GLfloat worldpos_x;
    GLfloat worldpos_z;
    
    GLfloat worldpos_x_tot;
    GLfloat worldpos_z_tot;
    
    GLfloat strafe;
    GLfloat strafe_max;
    GLfloat offset;
    
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
    
    Player* player_;
    

    
};

#endif /* game_hpp */
