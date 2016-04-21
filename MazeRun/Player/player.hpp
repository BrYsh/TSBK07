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
    void update();
    mat4 total_pos();
    
    mat4 rot;
    mat4 trans;
    mat4 total;
    
    GLfloat strafe;


    
};

#endif /* player_hpp */