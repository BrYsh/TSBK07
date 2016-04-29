//
//  maze.cpp
//  
//
//  Created by Andreas Brorsson on 2016-04-07.
//
//

#include "maze.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


Maze::Maze(int dir, int t){ //Konstruktor
    
    global_dir = dir;
    rot = Ry(dir*M_PI_2/2);
    tree = t;
    if (t < 3){
    left = new Maze( ((dir+1)%4+4)%4,t+1);
    right = new Maze(((dir-1)%4+4)%4,t+1);
    }
    
 
    
}



void Maze::generate_transform(GLfloat x,GLfloat z){
    
    //current
    xpos_n = x;
    zpos_n = z;// + width/2;
    
    rot = Ry(-global_dir*M_PI/2);
    trans = T(xpos_n ,0 ,zpos_n );
    total = Mult(trans,rot);
    
    GLfloat next_xl = xpos_n + (length-width+2)*cos(-global_dir*M_PI_2) + (-width/2+2+width/4)*sin(-global_dir*M_PI_2);
    GLfloat next_zl = zpos_n + (length-width+2)*sin(-global_dir*M_PI_2) + (-width/2+2+width/2)*cos(-global_dir*M_PI_2);
    
    GLfloat next_xr = xpos_n + (length +1)*cos(-global_dir*M_PI_2) + (width-3)*sin(global_dir*M_PI_2);
    GLfloat next_zr = zpos_n + (length +1)*sin(-global_dir*M_PI_2) + (width-3)*cos(-global_dir*M_PI_2);
    
    if (tree < 3) {
        left->generate_transform( next_xl,next_zl );
        right->generate_transform( next_xr,next_zr );
    }
    
    
}

void Maze::update_pos(mat4 Ti){
    
    if (tree < 3) {
        left->update_pos(Ti);
        right->update_pos(Ti);
    }
    
    total = Mult(Ti,total);//Mult(Ry(global_dir*M_PI_2),T);
    
}

void Maze::update_turn(GLfloat world_angle_s,GLfloat xpos,GLfloat zpos){
    
    total = Mult(total,Ry(global_dir*M_PI/2));
    total = Mult(total,T(-xpos,0,-zpos));
    total = Mult(total,T(-xpos_n,0,-zpos_n));
    
    total = Mult(total,Ry(world_angle_s));
    
    total = Mult(total,T(xpos_n,0,zpos_n));
    total = Mult(total,T(xpos,0,zpos));
    total = Mult(total,Ry(-global_dir*M_PI/2));
    
    if (tree < 2) {
        left->update_turn(world_angle_s,xpos,zpos);
        right->update_turn(world_angle_s,xpos,zpos);
    }
    

    
}


mat4 Maze::get_total(){
    
    return total;//Mult(global_trans,total);
    
}



