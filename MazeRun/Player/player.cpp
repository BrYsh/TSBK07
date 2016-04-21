//
//  player.cpp
//  
//
//  Created by Andreas Brorsson on 2016-04-14.
//
//

#include "player.hpp"


Player::Player(){
    
    
    rot = Ry(0);
    trans = T(0.0,0.0,0.0);
    total = IdentityMatrix();
    total = Mult(trans,total);
    strafe = 0;
    
    scale_arm_body = S(0.5,0.5,0.5);
    scale_head = S(0.2,0.3,0.3);
    
    x_pos = 0;
    y_pos = 0;
    z_pos = 0;
    
    leg_state_l = 10;
    leg_state_r =0;
    leg_angle = Rx(0);
    leg_trans = T(0, 0, 0);
    leg_total_l = Mult(leg_trans,leg_angle);
    leg_total_r = Mult(leg_trans,leg_angle);
    
    //initiate Left Arm things
    arm_state_l = 0;
    arm_angle_l = Rx(0);
    arm_trans_l = T(0, 0, 0);
    arm_total_l = Mult(arm_trans_l,arm_angle_l);
    
    //initiate Right Arm things
    arm_state_r = 10;
    arm_angle = Rx(0);
    arm_trans = T(0, 0, 0);
    arm_total_r = Mult(arm_trans,arm_angle);
    
    body_total = Mult(T(x_pos,y_pos+0.5,z_pos),scale_arm_body);
    head_total = Mult(T(x_pos,y_pos+1.35,z_pos),scale_head);
    
    
   
    
}

void Player::update(bool jump_in , bool duck_in){
    
    jump = jump_in;
    duck = duck_in;

    leg_run_l();
    leg_run_r();
    arm_run_l();
    arm_run_r();
    
//    if (duck) {
//        transB = T(x_pos, y_pos+1, z_pos-0.9); //fixa bra med coord för banan
//    }
//    else{
//        transB = T(x_pos, y_pos + 1.35, z_pos);
//    }
    
    
    
}

mat4 Player::total_pos(){
    
    return Mult(total,T(0.0,0.0,strafe));
    //std::cout << strafe << std::endl;
    
}

void Player::leg_run_l(){
    if (!jump && !duck){
        if (leg_state_l < 10){
            leg_state_l++;
            leg_angle = Rx(M_PI/7);
            leg_trans = T(0, 0, -0.36);
        }
        else{
            leg_state_l++;
            leg_angle = Rx(-M_PI/7);
            leg_trans = T(0, 0, 0.36);
            if (leg_state_l == 20) {
                leg_state_l = 0;
            }
        }
    }
    else{
        leg_angle = Rx(0);
        leg_trans = T(0, 0,0);
    }
    leg_total_l = Mult(Ry(M_PI_2),Mult(leg_trans, leg_angle));
    return;
}

void Player::leg_run_r(){
    if (!jump && !duck){
        if (leg_state_r < 10){
            leg_state_r++;
            leg_angle = Rx(M_PI/7);
            leg_trans = T(0, 0, -0.36);
        }
        else{
            leg_state_r++;
            leg_angle = Rx(-M_PI/7);
            leg_trans = T(0, 0, 0.36);
            if (leg_state_r == 20) {
                leg_state_r = 0;
            }
        }
    }
    else{
        leg_angle = Rx(0);
        leg_trans = T(0, 0, 0);
        
    }
    leg_total_r = Mult(Ry(M_PI_2),Mult(leg_trans, leg_angle));
    return;
}

void Player::arm_run_l(){
    if(!duck){
        if (arm_state_l < 10){
            arm_state_l++;
            arm_angle_l = Rz(0);
            arm_trans_l = T(x_pos+0.5, y_pos+1.2, z_pos);
        }
        
        else{
            arm_state_l++;
            arm_angle_l = Rz(-M_PI/4);
            arm_trans_l = T(x_pos+0.5, y_pos+1, z_pos);
            
            
            if (arm_state_l == 20) {
                arm_state_l = 0;
            }
        }
    }
    else{
        arm_angle_l = Rx(0);
        arm_trans_l = T(x_pos+0.5, y_pos+1.2, z_pos);
    }
    arm_total_l = Mult(Mult(Ry(-M_PI_2),Mult(arm_trans_l, arm_angle_l)),scale_arm_body);
    return ;
}

void Player::arm_run_r(){
    if(!duck){
        if (arm_state_r < 10){
            arm_state_r++;
            arm_angle = Rz(0);
            arm_trans = T(x_pos-0.5, y_pos+1.2, z_pos);
        }
        else{
            arm_state_r++;
            arm_angle = Rz(M_PI_2/2);
            arm_trans = T(x_pos-0.5, y_pos+1, z_pos);
            
            if (arm_state_r == 20) {
                arm_state_r = 0;
            }
        }
    }
    else{
        arm_angle = Rx(0);
        arm_trans = T(x_pos-0.5, y_pos+1.2, z_pos);
    }
    arm_total_r = Mult(Mult(Ry(-M_PI_2),Mult(arm_trans, arm_angle)),scale_arm_body);
    return ;
    
}
