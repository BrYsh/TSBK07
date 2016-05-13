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
    if (t < 2){
    left = new Maze( ((dir+1)%4+4)%4,t+1);
    right = new Maze(((dir-1)%4+4)%4,t+1);
    }
    
    //Add boosters
    choose_booster();
    booster_pos();

    //Add obstacles
    choose_obstacle();
    obstacle_pos();
    
}

void Maze::choose_booster(){

    boost = rand() % 11;
    printf("booster %d \n", boost );
    //return boost;
}

void Maze::booster_pos(){

    boost_x_pos = rand() % 17 + 18;
    boost_z_pos = rand() % 3;
    if (boost_z_pos == 0) {
        boost_z_pos = 5;
    }
    if (boost_z_pos == 1) {
        boost_z_pos = 8;
    }
    if (boost_z_pos == 2) {
        boost_z_pos = 11;
    }
    
    printf("boosterx %d \n", boost_x_pos );
    printf("boosterz %d \n", boost_z_pos );
    
}

void Maze::choose_obstacle(){
    
    obstacle = rand() % 2;
    printf("obstacle %d \n", obstacle );
    
}

void Maze::obstacle_pos(){
    
    obstacle_x_pos = rand() % 10 + 10; // kan inte lägg hinder vid sväng
    int diff_x = obstacle_x_pos - boost_x_pos;
    
    if (diff_x < 4){
    
        boost_x_pos -= 9;
    }
    
    printf("obstaclex %d \n", obstacle_x_pos );
}



void Maze::generate_transform(){
    
    //current
    xpos_n = 0;
    zpos_n = -width/2;
    rot = Ry(-global_dir*M_PI/2);
    trans = T(xpos_n ,0 ,zpos_n );
    total = Mult(trans,rot);
    
    //left
    left->xpos_n = length - width +2;
    left->zpos_n = -width/2 + 2;
    left->rot = Ry(-left->global_dir*M_PI/2);
    left->trans = T(left->xpos_n ,0 ,left->zpos_n );
    left->total = Mult(left->trans,left->rot);
    
    //Right
    right->xpos_n = length +1;
    right->zpos_n = width - 3 -width/2;
    right->rot = Ry(-right->global_dir*M_PI/2);
    right->trans = T(right->xpos_n,0 ,right->zpos_n);
    right->total = Mult(right->trans,right->rot);
    
}

void Maze::update_pos(mat4 Ti){
    
    if (tree < 2) {
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
    
    if (tree < 1) {
        left->update_turn(world_angle_s,xpos,zpos);
        right->update_turn(world_angle_s,xpos,zpos);
    }
    

    
}


mat4 Maze::get_total(){
    
    return total;//Mult(global_trans,total);
    
}



