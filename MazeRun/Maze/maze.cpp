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
    
 
    
}



void Maze::generate_transform(){
    
    //current
    rot = Ry(-global_dir*M_PI/2);
    trans = T(0 ,0 ,0 );
    total = Mult(trans,rot);
    
    //left
    left->rot = Ry(-left->global_dir*M_PI/2);
    left->trans = T(length - width ,0 ,0 );
    left->total = Mult(left->trans,left->rot);
    
    //Right
    right->rot = Ry(-right->global_dir*M_PI/2);
    right->trans = T(length -1,0 ,width-1);
    right->total = Mult(right->trans,right->rot);
    
}


//          |
//    x^    |
//          |
//rot ->    |