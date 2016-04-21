//
//  player.hpp
//  
//
//  Created by Andreas Brorsson on 2016-04-14.
//
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include "../MG/MicroGlut.h"
#include "../MG/GL_utilities.h"
#include "../MG/VectorUtils3.h"
#include "../MG/loadobj.h"
#include "../MG/LoadTGA.h"





class Player
{
    
public:
    
    Player();
    void update(bool, bool);
    mat4 total_pos();
    
    mat4 rot;
    mat4 trans;
    mat4 total;
    
    bool jump;
    bool duck;
    
    GLfloat leg_state_l;
    GLfloat leg_state_r;
    mat4 leg_angle;
    mat4 leg_trans;
    mat4 leg_total_l;
    mat4 leg_total_r;
    
    GLfloat arm_state_l;
    GLfloat arm_state_r;
    mat4 arm_angle;
    mat4 arm_angle_l;
    mat4 arm_trans;
    mat4 arm_trans_l;
    mat4 arm_total_l;
    mat4 arm_total_r;
    
    mat4 body_total;
    mat4 head_total;
    
    mat4 scale_arm_body;
    mat4 scale_head;

    GLfloat strafe;

    void leg_run_l();
    void leg_run_r();
    void arm_run_l();
    void arm_run_r();
    
    GLfloat x_pos;
    GLfloat y_pos;
    GLfloat z_pos;
    


    
};

#endif /* player_hpp */