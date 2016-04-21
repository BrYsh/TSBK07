//
//  game.cpp
//  
//
//  Created by Andreas Brorsson on 2016-04-08.
//
//

#include "game.hpp"
#include <iostream>
#include <string>

//Model* GenerateTerrain(char,int,int);

GLfloat height_controll(GLint length, GLint width, GLfloat xi, GLfloat yi,GLfloat offset,Maze* maze);

Game::Game(int w, int h){
    
    a = 1;
    width_ = h;
    length_ = w;
    offset = width_/2;
    std::cout << "width_ = " << width_ << "  || length_ = " << length_ << std::endl;
    
    jump = false;
    turn = false;
    turn_steps = 8;
    jump_count = 0;
    world_angle = 0;
    ball_speed_global = 0.00;
    ball_speed = ball_speed_global;
    
    x_pos = 0.0;
    y_pos = 0.0;
    y_pos_n = y_pos;
    z_pos = 0.0;
    
    x_pos_c = -10.0;
    y_pos_c = 3;
    z_pos_c = 0;
    
    x_pos = 0;
    z_pos = 0;
    
    x_pos_c = x_pos - 7*cos(world_angle);
    z_pos_c = z_pos - 7*sin(world_angle);
    
    strafe = 0;
    strafe_max = 3;
    
    global_dir = 0;
    
    world_trans = T(x_pos, y_pos, z_pos);
    
    maze = new Maze(0,0);
    current_track = maze->track;
    
    left_t = maze->left->track;
    leftleft_t = maze->left->left->track;
    leftright_t = maze->left->right->track;
    
    right_t = maze->right->track;
    rightright_t = maze->right->right->track;
    rightleft_t = maze->right->left->track;
    
    player_ = new Player();
    
}

void Game::update(){
    
    
    player_->update();
    
    //std::cout << "to middle turn = " << length_ + x_pos - (width_-2)/2 << std::endl;
    

    
    if (jump) {
        jump_controll();
        check_turn_key();
    }
    else{
        y_pos_n = height_controll(maze->length,maze->width,x_pos,z_pos,offset,maze);
    }
    if (turn) {
        turn_controll();
    }
    if (!(turn||jump)) {
        world_dir();
        x_pos += -ball_speed*cos(world_angle);
        z_pos += -ball_speed*sin(world_angle);
    }
    
    
    world_trans = T(-ball_speed, -(y_pos_n-y_pos), 0);
    y_pos = y_pos_n;
    maze->update_pos(world_trans);

    
    
    total = world_trans;

}


mat4 Game::update_camera(){
    
    return lookAt(-25,10,strafe,
                  0.0,1.0,strafe,
                  0.0,1.0,0.0);
}



GLfloat height_controll(GLint length, GLint width, GLfloat xi, GLfloat zi,GLfloat offset,Maze* maze){
    
    GLuint w = length; // maze->length;
    GLuint h = width; // maze->width;
    GLfloat x_pos = -xi;
    GLfloat z_pos = -zi + offset;
    
    if( x_pos > w || x_pos < 0 || z_pos > h || z_pos < 0 ){
        return 10.0;
    }

    int x = floor(x_pos);
    int z = floor(z_pos);
    GLfloat dx = x_pos - (GLfloat)x;
    GLfloat dz = z_pos - (GLfloat)z;
    GLfloat w11 = (1.0-dx)*(1.0-dz);
    GLfloat w12 = dx*(1.0-dz);
    GLfloat w21 = (1.0-dx)*dz;
    GLfloat w22 = dx*dz;
    
    GLfloat y11,y12,y21,y22;
    y11 = maze->track->vertexArray[(x + z * w)*3 + 1];
    y21 = maze->track->vertexArray[((x+1) + z * w)*3 + 1];
    y12 = maze->track->vertexArray[(x + (z+1) * w)*3 + 1];
    y22 = maze->track->vertexArray[((x+1) + (z+1) * w)*3 + 1];
    return (w11*y11 + w12*y12 + w21*y21 + w22*y22);
    
}

void Game::world_dir(){
    
    if (jump) {
        if (!turn) {
            check_turn_key();
        }
        return;
    }
    if (turn) {
        return;
    }

    if (glutKeyIsDown(' ')) {
        jump = true;
        jump_count = 0;
    }
    
    check_turn_key();
    if (turn) {
        return;
    }
    
    if(glutKeyIsDown(GLUT_KEY_LEFT)) {
        if (strafe > -strafe_max) {
            strafe -= 0.1;
        }
        else
        strafe = -strafe_max;
    }
    else if(glutKeyIsDown(GLUT_KEY_RIGHT)) {
        if (strafe < strafe_max) {
            strafe += 0.1;
        }
        else
        strafe = strafe_max;
    }
    else{
        strafe_back(0.1);
    }
    player_->strafe = strafe;
    if(glutKeyIsDown(GLUT_KEY_UP)) {
        if (ball_speed_global < 0.72) {
            ball_speed_global +=0.01;
            ball_speed = ball_speed_global;
        }
    }
    else if(glutKeyIsDown(GLUT_KEY_DOWN)) {
        if (ball_speed_global > 0.00) {
            ball_speed_global -= 0.01;
            ball_speed = ball_speed_global;
        }
    }
    else if(glutKeyIsDown('p')) {
            ball_speed_global = 0.0;
            ball_speed = 0;
    }
    
}

void Game::check_turn_key(){
    
    GLfloat to_centrum = length_ + x_pos - (width_-2)/2;

    
    if (glutKeyIsDown('c') && wait_key < 1) {
        turn = true;
        pol_dir = -1.0;
    }
    if (glutKeyIsDown('z') && wait_key < 1) {
        turn = true;
        pol_dir = 1.0;
        }
    
    if (turn) {
        if (to_centrum < width_/2) {
            if (to_centrum > 0){
            std::cout << "Wait to turn = " << to_centrum  << std::endl;
               // wait_turn_pos = true;
                strafe_back(strafe_max/(turn_steps-1)); //<-- hur många delar rotare
            }
            else if(to_centrum > -2.0){
                std::cout << "Turn Direct!! = " << to_centrum  << std::endl;
                // strafe -> 0 each turn
            }
            else{
                //dead by too late;
            }
        }
        turn_angle = M_PI_2/turn_steps;
        ball_speed = 0;
        global_dir = ((global_dir-1)%4+4)%4;
        world_angle += -turn_angle*pol_dir;
        maze->update_turn(turn_angle*pol_dir,x_pos,z_pos);
        wait_key = turn_steps-1;
        return;

    }
    
    
}

void Game::jump_controll(){
    
//    if (jump_count > 30) {
//        y_pos -= 0.1*jump_count/15;
//    }
//    else{
//        y_pos += 0.1*cos(jump_count*M_PI_2/60);
//    }
//    GLfloat control = height_controll();
//    jump_count++;
//    if (y_pos < control) {
//        jump = false;
//        y_pos = control;
//    }
    
}

void Game::turn_controll(){
    
    if (wait_key < 1){
        turn = false;
        ball_speed = ball_speed_global;
    }
    else{
        wait_key--;
        world_angle += -turn_angle*pol_dir;
        maze->update_turn(turn_angle*pol_dir,x_pos,z_pos);
        std::cout << " ||  Angle = " << world_angle << std::endl;
    }
    
}

void Game::strafe_back(GLfloat pace){
    if (strafe > pace) {
        strafe -= pace;
    }
    else if(strafe<-pace){
        strafe += pace;
    }
    else{
        strafe =0;
    }
}

void turn_update(std::string turn_dir){
    

    
    
}







