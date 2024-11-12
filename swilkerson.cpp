//Shane Wilkerson

#include "fonts.h"
#include <iostream>
#include "game.h"

using namespace std;

extern Global g;
extern Ball bal;
extern Game ga;
extern Bullet myBullet;

int shane_show = 0;

enum GunType {
    AR,       
    Shotgun,  
    Pistol    
};

class Gun {
protected:
    float bulletSpeed;
    int numBullets;  
    float spreadAngle;  
    virtual void shoot() = 0;
    virtual ~Gun() {}
};

class AR : public Gun {
public:
    AR() {
        bulletSpeed = 10.0f;  
        numBullets = 1;       
        spreadAngle = 0.0f;   
    }
    void shoot() override {
        cout << "AR shooting with bullet speed: " << bulletSpeed << ", spread: " << spreadAngle << endl;
    }
};

class Shotgun : public Gun {
public:
    Shotgun() {
        bulletSpeed = 6.0f;   
        numBullets = 3;       
        spreadAngle = 15.0f;  
    }

    void shoot() override {
        cout << "Shotgun shooting with bullet speed: " << bulletSpeed << ", spread: " << spreadAngle << endl;
    }
};

class Pistol : public Gun {
public:
    Pistol() {
        bulletSpeed = 7.0f;   
        numBullets = 1;       
        spreadAngle = 0.0f;   
    }

    void shoot() override {
        cout << "Pistol shooting with bullet speed: " << bulletSpeed << ", spread: " << spreadAngle << endl;
    }
};

Gun* currentGun = nullptr;

void show_my_featureSW(int x, int y)
{
    //draw a rectangle
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Shane");


}



//Draw bullets -- will go in render function
/*
void drawbullets()
{
    for (int i=0; i<g.nbullets; i++) {
        Bullet *b = &g.barr[i];
        //Log("draw bullet...\n");
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0],      b->pos[1]);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]);
        glVertex2f(b->pos[0],      b->pos[1]-1.0f);
        glVertex2f(b->pos[0],      b->pos[1]+1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
    }

}*/
/*
void show_my_feature(int x, int y)
{
    //draw a rectangle 
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Shane");


}
*/
