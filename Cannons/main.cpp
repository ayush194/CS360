

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "cannon.hpp"
#include "missile.hpp"
#include "gl_includes.h"

/*
 This is a simple inefficient Cannon game, where the user player
 fights a bot cannon in a serious battle of Cannon Balls
 Controls:
     Move the cannon in its arena with 'up', 'down', 'left', 'right' keys
     Change the muzzle angle with 'a', 'w', 's', 'd' keys
     Decrease/Increase the muzzle velocity with 'c', 'v' keys
     Shoot the cannon with the 'f' key
 Fight the Bot till death !
 */

bool GAME_OVER = false;
float *WIND_V;

Cannon* cannon1;
Cannon* cannon2;
std::list<Missile*> missiles;

void renderAxes() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);
    
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);
    
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 100.0);
    glEnd();
}

void renderGrid() {
    glLoadIdentity();
    glBegin(GL_LINES);
    for(int x = -50; x <= 50; x++) {
        glVertex3f(-x, -50, 0);
        glVertex3f(-x, 50, 0);
    }
    for(int y = -50; y <= 50; y++) {
        glVertex3f(-50, -y, 0);
        glVertex3f(50, -y, 0);
    }
    glEnd();
}

void renderScene(void){
    if (GAME_OVER) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.2, 0.8, 0.5);
    
    //renderAxes();
    renderGrid();
    
    cannon2->makeBotMovements();
    cannon1->render();
    cannon2->render();
    
    for(auto it = missiles.begin(); it != missiles.end(); it++) {
        if ((*it)->hasHitGround()) {
            missiles.erase(it);
        } else {
            (*it)->render();
        }
    }
    //during each render, iterate over the missiles and render them each
    //while doing so, check if any of them has hit the ground and update their position
    
    
    glFlush();
    //glutSwapBuffers();
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch(key) {
        case 'a' : {
            std::cout << "w pressed" << std::endl;
            cannon1->liftMuzzle();
            renderScene();
            break;
        }
        case 'w' : {
            std::cout << "a pressed" << std::endl;
            cannon1->rotateMuzzleLeft();
            renderScene();
            break;
        }
        case 's' : {
            std::cout << "d pressed" << std::endl;
            cannon1->rotateMuzzleRight();
            renderScene();
            break;
        }
        case 'd' : {
            std::cout << "s pressed" << std::endl;
            cannon1->lowerMuzzle();
            renderScene();
            break;
        }
        case 'f' : {
            std::cout << "f pressed" << std::endl;
            //fire a missile from the cannon 1
            //the user should have an option to control the speed of the missile but
            //for the moment all missiles will have the same speed initially
            Missile* new_missile = new Missile(cannon1);
            missiles.push_back(new_missile);
            renderScene();
            break;
        }
        case 'v' : {
            std::cout << "v pressed" << std::endl;
            //increase the muzzle speed of the cannon
            cannon1->increaseMuzzleSpeed();
            renderScene();
            break;
        }
        case 'c' : {
            std::cout << "c pressed" << std::endl;
            //decrease the muzzle speed of the cannon
            cannon1->decreaseMuzzleSpeed();
            renderScene();
            break;
        }
    }
}

void keyboardSpecialHandler(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP : {
            std::cout << "up pressed" << std::endl;
            cannon1->translate(-0.1, 0.0, 0.0);
            renderScene();
            break;
        }
        case GLUT_KEY_LEFT : {
            std::cout << "left pressed" << std::endl;
            cannon1->translate(0.0, -0.1, 0.0);
            renderScene();
            break;
        }
        case GLUT_KEY_RIGHT : {
            std::cout << "right pressed" << std::endl;
            cannon1->translate(0.0, 0.1, 0.0);
            renderScene();
            break;
        }
        case GLUT_KEY_DOWN : {
            std::cout << "down pressed" << std::endl;
            cannon1->translate(0.1, 0.0, 0.0);
            renderScene();
            break;
        }
    }
}

void changeSize(int x, int y){
    if (y == 0 || x == 0) return;
    x = y = std::min(x, y);
    GLfloat aspect = (GLfloat)(x/y);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0, 1, 0);
    //gluLookAt(0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0, 0, 1);
    //gluLookAt(0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0, 0, 1);
    //gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0);
    gluPerspective(45.0f,aspect,0.1f, 10000.0f);
    gluLookAt(10.0, 3.5, 5.0, 0.0, 3.5, 0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, x, y);
    //glMatrixMode(GL_MODELVIEW);
}

void init() {
    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat direction[] = { 1.0, 1.0, 1.0, 0.0 };
    
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 3);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT0, GL_POSITION, direction);
    
    glEnable(GL_LIGHTING);                // so the renderer considers light
    glEnable(GL_LIGHT0);                  // turn LIGHT0 on
    glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
    //glShadeModel(GL_FLAT);
}

int main (int argc, char **argv){
    srand(time(NULL));
    WIND_V = (float*)malloc(3 * sizeof(float));
    
    //assign random wind directions
    WIND_V[0] = (rand()%60 - 30.0f) / 100000.0f;
    WIND_V[1] = (rand()%60 - 30.0f) / 100000.0f;
    WIND_V[2] = (rand()%60 - 30.0f) / 100000.0f;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Cannons");
    
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0,0.0,0.0,0.0);
    
    cannon1 = new Cannon();
    cannon2 = new Cannon();
    cannon1->setColor(1.0, 0.0, 0.0);
    cannon1->setArena(0.0, 0.0, 3.0, 2.0);
    cannon1->translate(1.5, 1.0, 0.0);
    
    cannon2->setColor(0.0, 0.0, 1.0);
    cannon2->setArena(0.0, 5.0, 3.0, 7.0);
    cannon2->translate(1.5, 6.0, 0.0);
    cannon2->setRot(0.0, 0.0, 180);
    cannon2->setBot();
    //Assign  the function used in events
    //event handlers for keyboard events
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(keyboardSpecialHandler);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    
    glutIdleFunc(renderScene);                    //comment for static scene
    
    init();
    //Let start glut loop
    glutMainLoop();
    
    return 0;
}



//By default, the camera is placed at the origin (0, 0, 0) of the world coordinate system and facing the -ve z-axis
//So if you do not apply any projection matrix (i.e no gluLookAt, then this is the assumed camera),
