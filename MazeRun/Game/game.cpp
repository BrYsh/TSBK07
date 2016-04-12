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

Game::Game(){
    
    a = 1;
    width_ = 50;
    height_ = 9;
    
    jump = false;
    turn = false;
    jump_count = 0;
    ball_angle = 0;
    ball_speed_global = 0.1;
    ball_speed = ball_speed_global;
    
    x_pos = 0.0;
    y_pos = 0.0;
    z_pos = 0.0;
    
    x_pos_c = 0;
    y_pos_c = 0;
    z_pos_c = 0;
    
        x_pos_c = x_pos_tot - 7*cos(ball_angle);
        z_pos_c = z_pos_tot - 7*sin(ball_angle);
    
    strafe = 0;
    
    global_dir = 0;
    
    strans = T(x_pos, y_pos, z_pos);
    
    maze = new Maze(0,0);
    current_track = maze->track;
    
    left_t = maze->left->track;
    leftleft_t = maze->left->left->track;
    leftright_t = maze->left->right->track;
    
    right_t = maze->right->track;
    rightright_t = maze->right->right->track;
    rightleft_t = maze->right->left->track;
    
    
}

void Game::update(){
    
    ball_dir();
    
    x_pos = x_pos + ball_speed*cos(ball_angle);
    z_pos = z_pos + ball_speed*sin(ball_angle);
    
    x_pos_tot = x_pos + strafe*sin(-ball_angle);
    z_pos_tot = z_pos + strafe*cos(-ball_angle);
    
    if (jump) {
        jump_controll();
    }
    else{
        y_pos = height_controll();
    }
    if (turn) {
        turn_controll();
    }
    
    strans = T(x_pos_tot, y_pos, z_pos_tot);
//    x_pos_c = x_pos_tot - 7*cos(ball_angle);
//    z_pos_c = z_pos_tot - 7*sin(ball_angle);
    
    camera_control_x = x_pos_tot - 7*cos(ball_angle);
    camera_control_z = z_pos_tot - 7*sin(ball_angle);

    camera_control_y = height_controll() + 3;

    
    GLfloat dist = x_pos_c - camera_control_x;
    x_pos_c -= dist/2.0*0.25;
    
    dist = z_pos_c - camera_control_z;
    z_pos_c -= dist/2.0*0.25;
    
    dist = y_pos_c - camera_control_y;
    y_pos_c -= dist/2.0*0.25;
    
    std::cout << strafe << std::endl;
    
    total = strans;
    
}


mat4 Game::update_camera(){
    
    return lookAt(x_pos_c,y_pos_c,z_pos_c,
                  x_pos_tot,y_pos+3,z_pos_tot,
                  0.0,1.0,0.0);
}



GLfloat Game::height_controll(){
    
    GLuint w = maze->length;//tex->width;
    GLuint h = maze->width;//tex->height;      VIktigt att w och h är samma som terrain
    
    if( x_pos > w || x_pos < 0 || z_pos > h || z_pos < 0 ){
        return 3.0;
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

void Game::ball_dir(){
    
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
        if (strafe > -2) {
            strafe -= 0.1;
        }
        else
        strafe = -2;
    }
    else if(glutKeyIsDown(GLUT_KEY_RIGHT)) {
        if (strafe < 2) {
            strafe += 0.1;
        }
        else
        strafe = 2;
    }
    else{
        if (strafe > 0) {
            strafe -= 0.1;
        }
        else if(strafe<0)
        strafe += 0.1;
        else
            strafe =0;
    }
    if(glutKeyIsDown(GLUT_KEY_UP)) {
        if (ball_speed < 0.72) {
            ball_speed += 0.01;
        }
    }
    else if(glutKeyIsDown(GLUT_KEY_DOWN)) {
        if (ball_speed > 0.02) {
            ball_speed -= 0.01;
        }
    }
    
}

void Game::check_turn_key(){
    
    if (glutKeyIsDown('c')) {
        turn = true;
        pol_dir = 1.0;
        turn_angle = 1.0;
        global_dir = ((global_dir+1)%4+4)%4;
        return;
    }
    if (glutKeyIsDown('z')) {
        turn = true;
        pol_dir = -1.0;
        turn_angle = 1.0;
        global_dir = ((global_dir-1)%4+4)%4;
        return;
    }
    
}

void Game::jump_controll(){
    
    if (jump_count > 30) {
        y_pos -= 0.1*jump_count/15;
    }
    else{
        y_pos += 0.1*cos(jump_count*M_PI_2/60);
    }
    GLfloat control = height_controll();
    jump_count++;
    if (y_pos < control) {
        jump = false;
        y_pos = control;
    }
    
}

void Game::turn_controll(){
    
    if (turn_wait > 0) {
        turn_wait--;
        return;
    }
    
    GLfloat div= 12;
    
    if (turn_angle <= div){
        ball_angle += M_PI_2 / div  * pol_dir;
        turn_angle += 1.0;
        ball_speed = ball_speed_global/2;
    }
    else{
        turn = false;
        turn_wait = 15;
        ball_speed = ball_speed_global;
        ball_angle = global_dir * M_PI_2;
    }
    
}

void turn_update(std::string turn_dir){
    
    // Global dir (klar)
    // Byta hieghtcontroll <- auto när man byter current
    // Ta bort trädet som inte svänges till
    // Current = turn_maze
    // skapa nya träd
    
    
}




