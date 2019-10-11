//
//  cannon.hpp
//  CS360
//
//  Created by Ayush Kumar on 28/08/2019.
//  Copyright © 2019 Ayush Kumar. All rights reserved.
//

#ifndef cannon_hpp
#define cannon_hpp

#include <stdio.h>

class Cannon {
private:
    bool bot;
    
    float health;
    float pos[3];
    float rot[3];
    //rot is the rotation transform of the cannon about x,y,z axes
    float initial_muzzle_point[3];
    float muzzle_angle[2];
    float muzzle_speed;
    //muzzle_angle[0] is the muzzle angle about x-axis
    //muzzle_angle[1] is the muzzle angle about z-axis
    float color[4];
    float arena[4];
    float shadow[4];
    //(arena[0], arena[1]) is the top left corner
    //(arena[2], arena[3]) is the bottom right corner
    
public:
    Cannon();
    void clampPositionToArena();
    void setArena(const float &x1, const float &y1, const float &x2, const float &y2);
    void setColor(const float &r, const float &g, const float &b);
    float getMuzzleSpeed();
    const float* getInitialMuzzlePoint();
    const float* getMuzzleAngle();
    const float* getPos();
    const float* getRot();
    const float* getColor();
    const float* getShadow();
    void translate(const float &dx, const float &dy, const float &dz);
    void rotate(const float &dthetax, const float &dthetay, const float &dthetaz);
    void setPos(const float &x, const float &y, const float &z);
    void setRot(const float &thetax, const float &thetay, const float &thetaz);
    void setBot();
    void clampMuzzle();
    inline void lowerMuzzle() {muzzle_angle[0]--; clampMuzzle();}
    inline void liftMuzzle() {muzzle_angle[0]++; clampMuzzle();}
    inline void rotateMuzzleLeft() {muzzle_angle[1]++; clampMuzzle();}
    inline void rotateMuzzleRight() {muzzle_angle[1]--; clampMuzzle();}
    inline void increaseMuzzleSpeed() {muzzle_speed += 0.0002; clampMuzzle();}
    inline void decreaseMuzzleSpeed() {muzzle_speed -= 0.0002; clampMuzzle();}
    inline bool isBot() {return bot;}
    void hitPoints();
    float* applyCannonTransform(float vx, float vy, float vz);
    float* applyMuzzleTransform(float vx, float vy, float vz);
    void render();
    void renderAim();
    void renderCannonSquare();
    void renderArena();
    void renderHealth();
    void makeBotMovements();
};


#endif /* cannon_hpp */
