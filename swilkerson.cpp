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
#include <vector>
#include <string>

#define PI 3.141592653589793

//From Sky's source file
extern float eggplantX;
extern float eggplantY;
extern float carrotX;  
extern float carrotY;
extern int eggplantHealth;
extern float collisionThreshold;         
extern float collisionCarrotThreshold;   
extern float collisionTomatoThreshold;   
extern float collisionLettuceThreshold;  
extern int carrotHealth, tomatoHealth, lettuceHealth;
extern int playerScore;

using namespace std;

struct timespec lastShotAR = {0, 0};  //Last shot time for AR
struct timespec lastShotShotgun = {0, 0};  //Last shot time for Shotgun
const double arCooldown = 0.2;  
const double shotgunCooldown = 1.0;  

int currentGunIndex = 0;
vector<Gun> guns = {
    Gun("AR", 9.5, 0.2, 30, 30.0f, 1, 0.0f),
    Gun("Shotgun", 8.0, 1.0, 8, 1.5f, 3, 0.2f)
};


//Delay for guns
bool Gun::canShoot(struct timespec &lastShotTime) {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    double timeDiff = (currentTime.tv_sec - lastShotTime.tv_sec) +
                      (currentTime.tv_nsec - lastShotTime.tv_nsec) / 1e9;
    return timeDiff >= cooldown;
}

void fire_bullet(int mx, int my) {
    Gun &currentGun = guns[currentGunIndex];
    struct timespec &lastShotTime = (currentGunIndex == 0) ? lastShotAR : lastShotShotgun; //checks last time shot for any weapon

    if (currentGun.isReloading) {  //Check to see if reloading
        cout << "Reloading, cannot shoot!" << endl;
        return;
    }

    if (!currentGun.canShoot(lastShotTime) || currentGun.currentAmmo <= 0) return;

    for (int i = 0; i < currentGun.spreadCount; ++i) {
        if (ga.nbullets >= MAX_BULLETS) break;

        Bullet new_bullet;
        new_bullet.size = currentGun.bulletSize;  
        new_bullet.pos[0] = bal.pos[0];
        new_bullet.pos[1] = bal.pos[1];

        //cout << new_bullet.pos[0] << " " <<  
        //cout << new_bullet.pos[1] << " " >> 

        float dx = mx - new_bullet.pos[0];
        float dy = new_bullet.pos[1] - my;
//        float magnitude = sqrt(dx * dx + dy * dy);

         float angleOffset = (i - currentGun.spreadCount / 2) * currentGun.spreadAngle;
        float angle = atan2(dy, dx) + angleOffset;
        //float angle = atan2(dy, dx);

        cout << angle << endl;

        new_bullet.vel[0] = cos(angle) * currentGun.bulletSpeed;
        new_bullet.vel[1] = sin(angle) * currentGun.bulletSpeed;
        clock_gettime(CLOCK_REALTIME, &new_bullet.time);
        ga.barr[ga.nbullets++] = new_bullet;
    }

    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    lastShotTime = currentTime;
    currentGun.currentAmmo--;
}

void update_bullets(unsigned char map[16][31][30], int row, int col, float tile[2], int stage) {
    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];

        // Move bullet
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];

        if (b->pos[0] < 0.0) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[0] > (float)g.xres) {
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] < 0.0) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] > (float)g.yres) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        ++i;
/*
        bool collision = false;
        for (int i = 0; i < row; ++i) { 
            for (int j = 0; j < col; ++j) { 
                //Check tile 
                if ((map[stage][i][j] == 'w' || map[stage][i][j] == 'b') &&
                b->pos[0] > tile[0] * j &&
                b->pos[0] < tile[0] * (j + 1) &&
                b->pos[1] > tile[1] * i &&
                b->pos[1] < tile[1] * (i + 1)) {
                //Delete bullet
                memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
                --ga.nbullets;
                collision = true;
                cout << "Bullet hit restricted tile at " << b->pos[0] << ", " << b->pos[1] << "\n";
                break;
            }
        }
        if (collision) break;
        }

        //Increment bullet if no collision occurred
        if (!collision) {
            ++i;
        }*/
    }

    
    ga.check_bullet_lifetime();
}

//Bullet update 
/*void update_bullets() {

    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];

        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];

        if (b->pos[0] < 0.0) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[0] > (float)g.xres) {
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] < 0.0) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] > (float)g.yres) {
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        ++i;


            
    }

    //for (int i = 0; i < ga.nbullets; ++i) {
    //    Bullet &bullet = ga.barr[i];
    //    bullet.pos[0] += bullet.vel[0];
    //    bullet.pos[1] += bullet.vel[1];
    //    } 


    ga.check_bullet_lifetime(); 
}*/

void render_bullets() {
    for (int i=0; i<ga.nbullets; i++) {
            Bullet *b = &ga.barr[i];
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
    /*
    for (int i = 0; i < ga.nbullets; i++) {
        Bullet *b = &ga.barr[i];
        // Use the bullet's color
        glColor3f(b->color[0], b->color[1], b->color[2]);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0], b->pos[1]);
        glVertex2f(b->pos[0] - 1.0f, b->pos[1]);
        glVertex2f(b->pos[0] + 1.0f, b->pos[1]);
        glVertex2f(b->pos[0], b->pos[1] - 1.0f);
        glVertex2f(b->pos[0], b->pos[1] + 1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b->pos[0] - 1.0f, b->pos[1] - 1.0f);
        glVertex2f(b->pos[0] - 1.0f, b->pos[1] + 1.0f);
        glVertex2f(b->pos[0] + 1.0f, b->pos[1] - 1.0f);
        glVertex2f(b->pos[0] + 1.0f, b->pos[1] + 1.0f);
        glEnd();
    }*/
}

bool checkCollisionCarrot(float bulletX, float bulletY, float carrotX, float carrotY, float threshold) {
    float dx = bulletX - carrotX;
    float dy = bulletY - carrotY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold;
}

void CarrotCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];  

        if (checkCollisionCarrot(b->pos[0], b->pos[1], carrotX, carrotY, collisionCarrotThreshold)) {
            carrotHealth -= 1;  
            playerScore += 10;  
           // printf("Carrot health: %d\n", carrotHealth);
           // printf("Player Score: %d\n", playerScore);
            cout << "Bullet hit the carrot!" << endl;

            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;  

            break;  
        }
    }
}


/*
bool checkCollisionEggplant(float bulletX, float bulletY, float eggplantX, float eggplantY, float threshold) {
    float dx = bulletX - eggplantX;
    float dy = bulletY - eggplantY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold;
}

void CollisionEggplant(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];  

        if (checkCollisionEggplant(b->pos[0], b->pos[1], eggplantX, eggplantY, collisionThreshold)) {
            //Bullet hit eggplant
            eggplantHealth -= 1;  
            //playerScore += 10;    
            printf("Eggplant health: %d\n", eggplantHealth);
            //printf("Player Score: %d\n", playerScore);

            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;  

            break;  
        }
    }
}*/

void display_gun_info() {
    Gun &currentGun = guns[currentGunIndex];

    Rect r;
    r.bot = 25;               
    r.left = g.xres - 250;    
    r.center = 0;             

    
    char gun_info[128]; 
    if (currentGun.isReloading) {
        //Print reloading 
        sprintf(gun_info, "%s: %d/%d - Reloading", currentGun.name.c_str(), currentGun.currentAmmo, currentGun.ammoCapacity);
    } else {
        
        sprintf(gun_info, "%s: %d/%d", currentGun.name.c_str(), currentGun.currentAmmo, currentGun.ammoCapacity);
    }

    unsigned int text_color = 0xFFFFFFFF;

    glEnable(GL_TEXTURE_2D);
    ggprint8b(&r, 16, text_color, gun_info);
    glDisable(GL_TEXTURE_2D);
}

void reload() {
Gun &currentGun = guns[currentGunIndex];

    //Start reloading 
    if (currentGun.isReloading) {
        return;  
    }

    currentGun.isReloading = true;  
    clock_gettime(CLOCK_REALTIME, &currentGun.reloadStartTime);  //Record reload start time
    cout << "Reloading " << currentGun.name << "..." << endl;
}

void update_reload() {
    Gun &currentGun = guns[currentGunIndex];

    //If reloading, check time
    if (currentGun.isReloading) {
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);  //Get time

        //Calculate time since reload 
        double timeDiff = (currentTime.tv_sec - currentGun.reloadStartTime.tv_sec) +
                          (currentTime.tv_nsec - currentGun.reloadStartTime.tv_nsec) / 1e9;

        //Reload after 2 sec
        if (timeDiff >= 2.0) {
            currentGun.currentAmmo = currentGun.ammoCapacity;  //Refill ammo
            currentGun.isReloading = false;  //Set reloading to back to false
            cout << "Reloaded " << currentGun.name << "!" << endl;
        }
    }
}

int shane_show = 0;

void show_gun(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "AR");
}

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



