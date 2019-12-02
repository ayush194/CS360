//
//  missile.hpp
//  CS360
//
//  Created by Ayush Kumar on 28/08/2019.
//  Copyright © 2019 Ayush Kumar. All rights reserved.
//

#ifndef missile_hpp
#define missile_hpp

#include <stdio.h>
#include "cannon.hpp"

class Missile {
private:
    Cannon* parent;
    float v;
    float muzzle_point_at_fire[3];
    float muzzle_angle_at_fire[2];
    float cannon_pos_at_fire[3];
    float cannon_rot_at_fire[3];
    float v_0[3];
    //float p_0[3];
    float color[4];
    int t_0;
    //std::chrono::duration<double, std::milli> fire_time;
public:
    Missile(Cannon* par);
    ~Missile();
    int deltaT();
    float* getCurrentPos();
    float* getCurrentGlobalPos();
    bool hasHitGround();
    void render();
};



#endif /* missile_hpp */
