//Shane Wilkerson

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"
#include "sgonzales.h"
#include "game.h"

using namespace std;

void fire_bullet(int mx, int my) {
    if (ga.nbullets < MAX_BULLETS) {
        Bullet new_bullet;

        //Set the bullet's initial position to the player's position
        new_bullet.pos[0] = bal.pos[0];
        new_bullet.pos[1] = bal.pos[1];

        //Calculate the direction from player to mouse (g.mx, g.my)
        float dx = mx - new_bullet.pos[0];
        float dy = new_bullet.pos[1] - my;
        float magnitude = sqrt(dx * dx + dy * dy);

        //set the bullet's velocity
        new_bullet.vel[0] = dx / magnitude * 9.5;  //adjust speed here
        new_bullet.vel[1] = dy / magnitude * 9.5;

        //current time for the bullet's creation
        clock_gettime(CLOCK_REALTIME, &new_bullet.time);

        //add the new bullet to the array
        ga.barr[ga.nbullets++] = new_bullet;
    }
}

void update_bullets() {
    //move each bullet based on its velocity
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet &bullet = ga.barr[i];
        bullet.pos[0] += bullet.vel[0];  //Update X position
        bullet.pos[1] += bullet.vel[1];  //Update Y position
    }

    //remove expired bullets
    ga.check_bullet_lifetime();
}
int currentWeapon = 1;

int shane_show = 0;

void show_gun(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "AR");
}

/*
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
*/
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
