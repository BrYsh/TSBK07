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
    
   
    
}

void Player::update(){
    

    
    
}

mat4 Player::total_pos(){
    
    return Mult(total,T(0.0,0.0,strafe));
    
}