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
    
    //Rnadom lenght
    length = rand() % 200 + 50;
    
    //Add boosters
    choose_booster();
    booster_pos();
    
    //Add obstacles
    choose_obstacle();
    obstacle_pos();
    b_collision = false;
    
}


void Maze::choose_booster(){
    
    boost = rand() % 3;
   // printf("booster %d \n", boost );
    //return boost;
}

void Maze::booster_pos(){
    int x = length - 34;
    
    //printf("booster x %d \n", x);
    //printf("booster length %d \n", length );
    
    if (x < 10) {
        
        boost = 5;
        
    }
    
    else {
    boost_x_pos = rand() % x + 18;
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
    
   // printf("boosterx %d \n", boost_x_pos );
    //printf("boosterz %d \n", boost_z_pos );
    }
}

void Maze::choose_obstacle(){
    
    obstacle = rand() % 3;
    //printf("obstacle %d \n", obstacle );
    
}

void Maze::obstacle_pos(){
    
    int x = length - 25;
    //printf("obstacle x %d \n", x);
    //printf("obstacle length %d \n", length );
    
    if (x < 10){
        
        obstacle = 2;
    }
    
    else {
        
    obstacle_x_pos = rand() % x + 5; // kan inte lägg hinder vid sväng
    int diff_x = abs(obstacle_x_pos - boost_x_pos);
    
    if (diff_x < 10){
        
        boost = 5;
        
    }
    
   // printf("obstaclex %d \n", obstacle_x_pos );
    }
}



void Maze::generate_transform(GLfloat x,GLfloat z){
    
    
    xpos_n = x;
    zpos_n = z;
    //printf ("New GT \n Tree: %d Global dir = %d \n", tree, global_dir);
    //printf ("xpos_n: %4.0f zpos_n %4.0f \n\n", xpos_n,zpos_n);
    rot = Ry(-global_dir*M_PI/2);
    trans = T(xpos_n ,0 ,zpos_n );
    total = Mult(trans,rot);
    
    if (tree < 2) {
        
        GLfloat next_xl = xpos_n + (length-(width-1) )*cos(-global_dir*M_PI_2) +sin(global_dir*M_PI_2);
        GLfloat next_zl = zpos_n + (length-(width-1) )*sin(-global_dir*M_PI_2) +cos(global_dir*M_PI_2);
        
        GLfloat next_xr = xpos_n + (length )*cos(-global_dir*M_PI_2) + (width-2)*sin(global_dir*M_PI_2);
        GLfloat next_zr = zpos_n + (length )*sin(-global_dir*M_PI_2) + (width-2)*cos(global_dir*M_PI_2);
        
        
        left->generate_transform( next_xl,next_zl );
        right->generate_transform( next_xr,next_zr );
    }
    
    
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
    
    if (tree < 2) {
        left->update_turn(world_angle_s,xpos,zpos);
        right->update_turn(world_angle_s,xpos,zpos);
    }
    

    
}

void Maze::update_terain(GLfloat x,GLfloat z){
    

    
    xpos_n += x;
    zpos_n += z;
    
    
    
    
    if (tree < 2) {
        left->update_terain(x,z);
        right->update_terain(x,z);
    }
    
    
}


mat4 Maze::get_total(){
    
    return total;//Mult(global_trans,total);
    
}

void Maze::update_tree(){
 
    tree--;
    
    if (tree < 1) {
        left->update_tree();
        right->update_tree();
    }
    
    
}

void Maze::new_path(){

    if (tree == 0){
        left->new_path();
        right->new_path();
    }
    else if (tree == 1) {
        left = new Maze( ((global_dir+1)%4+4)%4,tree+1);
        right = new Maze(((global_dir-1)%4+4)%4,tree+1);
    }
    
}

void Maze::get_all_pos(){
    
    if (tree < 2) {
        left->get_all_pos();
        right->get_all_pos();
    }
    
}


void Maze::null_global_dir(char turn){
    
    int pol = 0;
    
    if (turn == 'r') {
        pol = 1;
    }
    else{
        pol = -1;
    }
    
    global_dir = ((global_dir+pol)%4+4)%4;
    
    if (tree < 2) {
        left->null_global_dir(turn);
        right->null_global_dir(turn);
    }
    
}


















