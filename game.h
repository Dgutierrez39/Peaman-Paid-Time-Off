#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cstring>
#include <ctime>
#include <cmath>
#include "fonts.h"
#include "log.h"
#include "sgonzales.h"
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;


#define AR 1
#define SHOTGUN 2


extern const float shotgunSpread;  
extern struct timespec lastShotAR; 
extern struct timespec lastShotShotgun;  
extern const double arCooldown;  
extern const double shotgunCooldown;  

typedef double Vec[3];
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

const double BULLET_LIFESPAN = 3.0;
const int MAX_BULLETS = 99;

class Gun {
public:
    string name;        //Gun name (e.g., "AR", "Shotgun")
    float bulletSpeed;  //Speed of bullets fired
    double cooldown;    //Cooldown time between shots
    int ammoCapacity;   //Maximum ammo capacity
    int currentAmmo;    //Current ammo count
    float bulletSize;   //Size of bullets
    int spreadCount;    //Number of bullets in a spread
    float spreadAngle;  //Angle between spread bullets
    bool isReloading;
    struct timespec reloadStartTime;

    Gun(string gunName, float speed, double cd, int capacity, float size, int spread, float angle);

    //Determines if the gun can shoot based on cooldown
    bool canShoot(struct timespec &lastShotTime);
};

class Global {
public:
    int useless_Var;
    unsigned char keys[65536];
    int xres, yres;
    double delay;
    GLuint MenuTexture;
    GLuint silhouetteTexture;
    GLuint PlayTexture;
    int playPress;
    int silhouette;
    int menu;
    int mx, my;

    Global();

};

class Ball {
public:
    Vec vel;
    Vec dir;
    float angle;
    float color[3];
    float pos[2];
    float movement[2];
    double camera[2];

    Ball();

};

class Bullet {
public:
   Vec pos;
   Vec vel;
    float color[3];
    struct timespec time;
    float size;
public:
    Bullet();
};

class Game {
public:
    Ball bal;
    Bullet *barr;
    int nbullets;
    struct timespec bulletTimer;

public:
    Game();

    ~Game();

    // Function to check if a bullet has expired
    void check_bullet_lifetime();







};

extern Global g;
extern Ball bal;
extern Game ga;

void fire_bullet(int mx, int my);
void update_bullets(unsigned char map[16][31][30], int row, int col, float tile[2], int stage);
//void update_bullets();
void display_gun_info();
void reload();
void update_reload();
void render_bullets();
void CarrotCollision(Game &ga);

extern int currentGunIndex;
extern vector<Gun> guns;

#endif // GAME_H
