//
//  missile.cpp
//  CS360
//
//  Created by Ayush Kumar on 28/08/2019.
//  Copyright © 2019 Ayush Kumar. All rights reserved.
//

#include "missile.hpp"
#include "cannon.hpp"
#include "gl_includes.h"
#include <iostream>
#include <string>
#include <cmath>

extern Cannon* cannon1;
extern Cannon* cannon2;
extern float* WIND_V;

Missile::Missile(Cannon* par) {
        parent = par;
        v = par->getMuzzleSpeed();
        const float *tmp = par->getColor();
        color[0] = tmp[0]; color[1] = tmp[1]; color[2] = tmp[2]; color[3] = tmp[3];
        const float* muzzle_angle = par->getMuzzleAngle();
        muzzle_angle_at_fire[0] = muzzle_angle[0];
        muzzle_angle_at_fire[1] = muzzle_angle[1];
        const float* pos = par->getPos();
        cannon_pos_at_fire[0] = pos[0];
        cannon_pos_at_fire[1] = pos[1];
        cannon_pos_at_fire[2] = pos[2];
        const float* rot = par->getRot();
        cannon_rot_at_fire[0] = rot[0];
        cannon_rot_at_fire[1] = rot[1];
        cannon_rot_at_fire[2] = rot[2];
        //const float* initial_muzzle_point = parent->getInitialMuzzlePoint();
        //float* tmp = parent->transformVector(initial_muzzle_point[0], initial_muzzle_point[0], initial_muzzle_point[0]);
        //muzzle_point_at_fire[0] = tmp[0];
        //muzzle_point_at_fire[0] = tmp[1];
        //muzzle_point_at_fire[0] = tmp[2];
        v_0[0] = -v * sin(-muzzle_angle_at_fire[0] * M_PI/180.0f) * sin(muzzle_angle_at_fire[1] * M_PI/180.0f);
        v_0[1] = v * sin(-muzzle_angle_at_fire[0] * M_PI/180.0f) * cos(muzzle_angle_at_fire[1] * M_PI/180.0f);
        v_0[2] = v * cos(-muzzle_angle_at_fire[0] * M_PI/180.0f);
        t_0 = glutGet(GLUT_ELAPSED_TIME);
        //fire_time = std::chrono::system_clock::now();
    }
Missile::~Missile() {}
int Missile::deltaT() {
    return glutGet(GLUT_ELAPSED_TIME) - t_0;
}
float* Missile::getCurrentPos() {
    //float delta_t = std::chrono::system_clock::now() - fire_time;
    //gives the current position in the cannon's frame
    float* p_t = (float*)malloc(3 * sizeof(float));
    p_t[0] = (WIND_V[0] + v_0[0]) * deltaT();
    p_t[1] = ((parent->isBot() ? -WIND_V[1] : WIND_V[1]) + v_0[1]) * deltaT();
    p_t[2] = (WIND_V[2] + v_0[2] * deltaT()) - 0.5 * 0.0000098 * deltaT() * deltaT();
    return p_t;
}
float* Missile::getCurrentGlobalPos() {
    float* p_t = getCurrentPos();
    //std::cout << "p_t local " << p_t[0] << ' ' << p_t[1] << ' ' << p_t[2] << std::endl;
    glLoadIdentity();
    //Apply all the cannon transformations
    glTranslatef(cannon_pos_at_fire[0], cannon_pos_at_fire[1], cannon_pos_at_fire[2]);
    glRotatef(cannon_rot_at_fire[0], 1.0, 0.0, 0.0);
    glRotatef(cannon_rot_at_fire[1], 0.0, 1.0, 0.0);
    glRotatef(cannon_rot_at_fire[2], 0.0, 0.0, 1.0);
    
    glTranslatef(p_t[0], p_t[1], p_t[2]);
    //Apply all the muzzle transformations
    glRotatef(muzzle_angle_at_fire[1], 0.0, 0.0, 1.0);
    glRotatef(muzzle_angle_at_fire[0], 1.0, 0.0, 0.0);
    
    //Apply all the missile transformations
    //note that the missile transformations below are all in the parent's frame
    //glTranslatef(p_t[0], p_t[1], p_t[2]);
    const float* initial_muzzle_point = parent->getInitialMuzzlePoint();
    glTranslatef(initial_muzzle_point[0], initial_muzzle_point[1], initial_muzzle_point[2]);
    
    float* v = (float*)malloc(16 * sizeof(float));
    memset(v, 0, 16 * sizeof(v[0]));
    v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f; v[3] = 1;
    glMultMatrixf(v);
    float* v_transformed = (float*)malloc(16 * sizeof(float));
    glGetFloatv(GL_MODELVIEW_MATRIX, v_transformed);
    return v_transformed;
}
bool Missile::hasHitGround() {
    float* p_t = getCurrentPos();
    std::cout << "p_t local " << p_t[0] << ' ' << p_t[1] << ' ' << p_t[2] << std::endl;
    std::cout << "muzzle point at fire " << muzzle_point_at_fire[2] << std::endl;
    if (p_t[2] < -(0.2 + 0.5 * cos(-muzzle_angle_at_fire[0] * M_PI/180.0f))) {
        //the missile has hit the ground
        //give both tanks hit points
        for(Cannon* cannon : {cannon1, cannon2}) {
            const float* cannon_shadow = cannon->getShadow();
            const float* cannon_pos = cannon->getPos();
            float hit_range[] = {cannon_shadow[0] + cannon_pos[0],
                cannon_shadow[1] + cannon_pos[1],
                cannon_shadow[2] + cannon_pos[0],
                cannon_shadow[3] + cannon_pos[1]
            };
            p_t = getCurrentGlobalPos();
            std::cout << "p_t global " << p_t[0] << ' ' << p_t[1] << ' ' << p_t[2] << std::endl;
            std::cout << "hit_range " << hit_range[0] << ' ' << hit_range[1] << ' ' << hit_range[2] << ' ' << hit_range[3] << std::endl;
            //float* top_left = cannon->applyCannonTransform(shadow[0], shadow[1], 0.0);
            //float* bottom_right = cannon->applyCannonTransform(shadow[2], shadow[3], 0.0);
            
            if (hit_range[0] < p_t[0] && p_t[0] < hit_range[2] && hit_range[1] < p_t[1] && p_t[1] < hit_range[3]) {
                //std::cout << "cannon hit" << std::endl;
                cannon->hitPoints();
            }
        }
        return true;
    }
    return false;
}

void Missile::render() {
    //std::cout << "rendering missile " << std::endl;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    //float delta_t =std::chrono::system_clock::now() - fire_time;
    //std::cout << "time is " << glutGet(GLUT_ELAPSED_TIME) << std::endl;
    float* p_t = getCurrentPos();
    
    glColor3f(color[0], color[1], color[2]);
    glLoadIdentity();
    //Apply all the cannon transformations
    glTranslatef(cannon_pos_at_fire[0], cannon_pos_at_fire[1], cannon_pos_at_fire[2]);
    glRotatef(cannon_rot_at_fire[0], 1.0, 0.0, 0.0);
    glRotatef(cannon_rot_at_fire[1], 0.0, 1.0, 0.0);
    glRotatef(cannon_rot_at_fire[2], 0.0, 0.0, 1.0);
    
    glTranslatef(p_t[0], p_t[1], p_t[2]);
    //Apply all the muzzle transformations
    glRotatef(muzzle_angle_at_fire[1], 0.0, 0.0, 1.0);
    glRotatef(muzzle_angle_at_fire[0], 1.0, 0.0, 0.0);
    
    //Apply all the missile transformations
    //note that the missile transformations below are all in the parent's frame
    //glTranslatef(p_t[0], p_t[1], p_t[2]);
    const float* initial_muzzle_point = parent->getInitialMuzzlePoint();
    glTranslatef(initial_muzzle_point[0], initial_muzzle_point[1], initial_muzzle_point[2]);
    glutWireSphere(0.05, 20, 10);
}

