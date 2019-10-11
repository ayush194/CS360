//
//  cannon.cpp
//  CS360
//
//  Created by Ayush Kumar on 28/08/2019.
//  Copyright © 2019 Ayush Kumar. All rights reserved.
//

#include "gl_includes.h"
#include "cannon.hpp"
#include "missile.hpp"
#include <iostream>
#include <algorithm>
#include <list>
#include <cmath>

extern bool GAME_OVER;
extern std::list<Missile*> missiles;
extern Cannon* cannon2;

Cannon::Cannon() {
        //initialize the cannon with 100 health
        bot = false;
        health = 100;
        muzzle_speed = 0.0070;
        //initialize the cannon at (0, 0, 0.2), the center of the cannon
        //is at the back end of the muzzle
        pos[0] = pos[1] = 0.0f; pos[2] = 0.2f;
        //initilize the initial rotation transform applied to the cannon
        rot[0] = rot[1] = rot[2] = 0.0f;
        //initialize with a random muzzle angle
        muzzle_angle[0] = -30.0f; muzzle_angle[1] = 0.0f;
        //initial_muzzle_point stores the point in space where it was initially located
        initial_muzzle_point[0] = 0.0f;
        initial_muzzle_point[1] = 0.0f;
        initial_muzzle_point[2] = 0.5f;
        //give a random color to the canoon
        color[0] = color[1] = color[2] = 0.0f; color[3] = 1.0;
        setArena(0.0, 0.0, 2.0, 1.0);
        shadow[0] = shadow[1] = -0.2f; shadow[2] = shadow[3] = 0.2f;
    }
void Cannon::clampPositionToArena() {
    pos[0] = std::max(pos[0], arena[0]);
    pos[0] = std::min(pos[0], arena[2]);
    pos[1] = std::max(pos[1], arena[1]);
    pos[1] = std::min(pos[1], arena[3]);
}
void Cannon::setArena(const float &x1, const float &y1, const float &x2, const float &y2) {
    arena[0] = x1; arena[1] = y1; arena[2] = x2; arena[3] = y2;
}
void Cannon::setColor(const float &r, const float &g, const float &b) {
    color[0] = r; color[1] = g; color[2] = b;
}
float Cannon::getMuzzleSpeed() {
    return muzzle_speed;
}
const float* Cannon::getInitialMuzzlePoint() {
    //return the initial muzzle point i.e.
    //muzzle point without any transformations applied to the muzzle
    return initial_muzzle_point;
}
const float* Cannon::getMuzzleAngle() {
    return muzzle_angle;
}
const float* Cannon::getPos() {
    return pos;
}
const float* Cannon::getRot() {
    return rot;
}
const float* Cannon::getColor() {
    return color;
}
const float* Cannon::getShadow() {
    return shadow;
}
void Cannon::translate(const float &dx, const float &dy, const float &dz) {
    pos[0] += dx; pos[1] += dy; pos[2] += dz;
    clampPositionToArena();
}
void Cannon::rotate(const float &dthetax, const float &dthetay, const float &dthetaz) {
    rot[0] += dthetax; rot[1] += dthetay; rot[2] += dthetaz;
}
void Cannon::setPos(const float &x, const float &y, const float &z) {
    pos[0] = x; pos[1] = y; pos[2] = z;
    clampPositionToArena();
    //translate(x - pos[0], y - pos[1], z - pos[2]);
}
void Cannon::setRot(const float &thetax, const float &thetay, const float &thetaz) {
    rot[0] = thetax; rot[1] = thetay; rot[2] = thetaz;
    //rotate(thetax - rot[0], thetay - rot[1], thetaz - rot[2]);
}
void Cannon::setBot() {
    bot = true;
}
void Cannon::clampMuzzle() {
    muzzle_angle[0] = std::max(muzzle_angle[0], -90.0f);
    muzzle_angle[0] = std::min(muzzle_angle[0], 0.0f);
    muzzle_angle[1] = std::max(muzzle_angle[1], -45.0f);
    muzzle_angle[1] = std::min(muzzle_angle[1], 45.0f);
    muzzle_speed = std::max(muzzle_speed, 0.0065f);
    muzzle_speed = std::min(muzzle_speed, 0.0075f);
}
//void Cannon::lowerMuzzle() {muzzle_angle[0]--; clampMuzzle();}
//void Cannon::liftMuzzle() {muzzle_angle[0]++; clampMuzzle();}
//void Cannon::rotateMuzzleLeft() {muzzle_angle[1]++; clampMuzzle();}
//void Cannon::rotateMuzzleRight() {muzzle_angle[1]--; clampMuzzle();}
void Cannon::hitPoints() {
    health -= 20;
    if (health < 0) {
        health = 0;
        GAME_OVER = true;
    }
    renderHealth();
    
}
float* Cannon::applyCannonTransform(float vx, float vy, float vz) {
    glLoadIdentity();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0], 1.0, 0.0, 0.0);
    glRotatef(rot[1], 0.0, 1.0, 0.0);
    glRotatef(rot[2], 0.0, 0.0, 1.0);
    
    float* v = (float*)malloc(16 * sizeof(float));
    memset(v, 0, 16 * sizeof(v[0]));
    v[0] = vx; v[1] = vy; v[2] = vz; v[3] = 1;
    glMultMatrixf(v);
    float* v_transformed = (float*)malloc(16 * sizeof(float));
    glGetFloatv(GL_MODELVIEW_MATRIX, v_transformed);
    return v_transformed;
}
float* Cannon::applyMuzzleTransform(float vx, float vy, float vz) {
    glLoadIdentity();
    //glTranslatef(pos[0], pos[1], pos[2]);
    //glRotatef(rot[0], 1.0, 0.0, 0.0);
    //glRotatef(rot[1], 0.0, 1.0, 0.0);
    //glRotatef(rot[2], 0.0, 0.0, 1.0);
    glRotatef(muzzle_angle[1], 0.0, 0.0, 1.0);
    glRotatef(muzzle_angle[0], 1.0, 0.0, 0.0);
    
    float* v = (float*)malloc(16 * sizeof(float));
    memset(v, 0, 16 * sizeof(v[0]));
    v[0] = vx; v[1] = vy; v[2] = vz; v[3] = 1;
    glMultMatrixf(v);
    float* v_transformed = (float*)malloc(16 * sizeof(float));
    glGetFloatv(GL_MODELVIEW_MATRIX, v_transformed);
    return v_transformed;
}

void Cannon::render() {
    //glColor3f(color[0], color[1], color[2]);
    renderArena();
    renderHealth();
    if (!bot) {
        renderAim();
    }
    renderCannonSquare();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glLoadIdentity();
    //glPushMatrix();
    
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0], 1.0, 0.0, 0.0);
    glRotatef(rot[1], 0.0, 1.0, 0.0);
    glRotatef(rot[2], 0.0, 0.0, 1.0);
    //drawing the first wheel
    //glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(-0.1, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(0.4, 0.4, 0.4);
    glutSolidTorus(0.1, 0.4, 20, 20);
    glPopMatrix();
    //drawing the muzzle
    //glColor3f(color[0], color[1], color[2]);
    glPushMatrix();
    glRotatef(muzzle_angle[1], 0.0, 0.0, 1.0);
    glRotatef(muzzle_angle[0], 1.0, 0.0, 0.0);
    gluCylinder(gluNewQuadric(), 0.05, 0.05, 0.5, 10, 10);
    glPopMatrix();
    //drawing the second wheel
    //glLoadIdentity();
    //glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.1, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glScalef(0.4, 0.4, 0.4);
    glutSolidTorus(0.1, 0.4, 20, 20);
    glPopMatrix();
    
    //glPopMatrix();
    
    //also render the projected space in which the projectile will fall
}

void Cannon::renderAim() {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    float* muzzle_point = applyMuzzleTransform(initial_muzzle_point[0], initial_muzzle_point[1], initial_muzzle_point[2]);
    float v_0[3];
    v_0[0] = -muzzle_speed * sin(-muzzle_angle[0] * M_PI/180.0f) * sin(muzzle_angle[1] * M_PI/180.0f);
    v_0[1] = muzzle_speed * sin(-muzzle_angle[0] * M_PI/180.0f) * cos(muzzle_angle[1] * M_PI/180.0f);
    v_0[2] = muzzle_speed * cos(-muzzle_angle[0] * M_PI/180.0f);
    float delta_t = (v_0[2] + sqrt(v_0[2] * v_0[2] + 2 * 0.0000098 * (muzzle_point[2] + 0.2))) / 0.0000098;
    //glColor3f(0.0f, 1.0f, 0.0f);
    //apply cannon transform
    glLoadIdentity();
    glTranslatef(pos[0], pos[1], 0.0);
    glRotatef(rot[0], 1.0, 0.0, 0.0);
    glRotatef(rot[1], 0.0, 1.0, 0.0);
    glRotatef(rot[2], 0.0, 0.0, 1.0);
    int num_segments = 20;
    float aim_radius = 0.2;
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
        
        float x = aim_radius * cosf(theta);//calculate the x component
        float y = aim_radius * sinf(theta);//calculate the y component
        
        glVertex3f(x + muzzle_point[0] + v_0[0] * delta_t, y + muzzle_point[1] + v_0[1] * delta_t, 0.0f);//output vertex
        
    }
    glEnd();
}

void Cannon::renderCannonSquare() {
    float color4[] = {color[0], color[1], color[2], 0.3};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color4);
    //glColor4f(color[0], color[1], color[2], 0.3);
    glLoadIdentity();
    glTranslatef(pos[0], pos[1], 0.0);
    glRotatef(rot[0], 1.0, 0.0, 0.0);
    glRotatef(rot[1], 0.0, 1.0, 0.0);
    glRotatef(rot[2], 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(shadow[0], shadow[1], 0.0f);
    glVertex3f(shadow[0], shadow[3], 0.0f);
    glVertex3f(shadow[2], shadow[3], 0.0f);
    glVertex3f(shadow[2], shadow[1], 0.0f);
    glEnd();
}

void Cannon::renderArena() {
    float color4[] = {0.9f, 0.9f, 0.9f, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color4);
    glLoadIdentity();
    //glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(arena[0], arena[1], -0.001);
    glVertex3f(arena[0], arena[3], -0.001);
    glVertex3f(arena[2], arena[3], -0.001);
    glVertex3f(arena[2], arena[1], -0.001);
    glEnd();
}

void Cannon::renderHealth() {
    //glutStrokeCharacter(GLUT_STROKE_ROMAN, 'H');
    //render a cuboid in the cell below the arena
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    //apply cannon transform
    glLoadIdentity();
    glTranslatef(pos[0], pos[1], 2.0);
    glRotatef(rot[0], 1.0, 0.0, 0.0);
    glRotatef(rot[1], 0.0, 1.0, 0.0);
    glRotatef(rot[2], 0.0, 0.0, 1.0);
    glScalef(1.0f, 10.0 * health/100, 1.0f);
    glTranslatef(0.05f, 0.05f, 0.05f);
    glutSolidCube(0.1);
}
void Cannon::makeBotMovements() {
    int r = rand() % 200;
    std::cout << "rand " << r << std::endl;
    if (0 <= r && r <= 10) {
        translate(-0.1, 0.0, 0.0);
    } else if (20 <= r && r <= 30) {
        translate(0.1, 0.0, 0.0);
    } else if (40 <= r && r <= 50) {
        translate(0.0, -0.1, 0.0);
    } else if (60 <= r && r <= 70) {
        translate(0.0, 0.1, 0.0);
    } else if (80 <= r && r <= 120){
        Missile* new_missile = new Missile(cannon2);
        missiles.push_back(new_missile);
    } else if (140 <= r && r <= 150) {
        liftMuzzle();
    } else if (160 <= r && r <= 170) {
        lowerMuzzle();
    } else if (180 <= r && r <= 185) {
        increaseMuzzleSpeed();
    } else if (190 <= r && r <= 195) {
        decreaseMuzzleSpeed();
    }
}

