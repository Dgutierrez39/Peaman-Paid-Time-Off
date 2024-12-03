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

#define MAX_ENEMIES 100 
//#define AR 1
//#define SHOTGUN 2


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

const double BULLET_LIFESPAN = 5.0;
const int MAX_BULLETS = 99;

extern float collisionThreshold;
extern float collisionCarrotThreshold;
extern float collisionTomatoThreshold;
extern float collisionLettuceThreshold;
extern float carrotX, carrotY;
extern float tomatoX, tomatoY;
extern float brockX, brockY;
extern float eggplantX, eggplantY;
extern float eggplantX1, eggplantY1;
extern float lettuceX, lettuceY;
extern float slowdown;
extern float lastShotTime;
extern int tomatoHealth, carrotHealth, lettuceHealth;
extern int eggplantHealth, eggplantHealth1;

struct Enemy {
    float x, y;
    float size;
    int health;
    bool active; 
};

class Gun {
public:
    string name;        //Gun name 
    float bulletSpeed;  //Velocity
    double cooldown;    //Time in between shots
    int ammoCapacity;   //Max ammo 
    int currentAmmo;    //Current ammo count
    float bulletSize;   //Size of bullets
    int spreadCount;    //Number of bullets in spread
    float spreadAngle;  //Angle between spread bullets
    bool isReloading;
    struct timespec reloadStartTime;
    bool purchased; //Flag to see if gun has been purchased

    Gun(string gunName, float speed, double cd, int capacity, float size, int spread, float angle);

    //Determines if the gun can shoot based on cooldown
    bool canShoot(struct timespec &lastShotTime);

    //To purchase a gun
    bool purchase(int &playerScore, int cost);
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

void shopGuns(unsigned char key, int &playerScore);
//void renderShop(int xres, int yres);
void renderShop(int xres, int yres, vector<Gun>& guns);
void fire_bullet(int mx, int my);
void update_bullets(unsigned char map[16][31][30], int row, int col, float offx, float offy, float tile[2], int stage);
//void update_bullets();
void display_gun_info();
void reload();
void update_reload();
void render_bullets();
void CarrotCollision(Game &ga);
extern int playerScore;
extern bool openShop;

void Carrot1Collision(Game &ga);
void Carrot2Collision(Game &ga);
void Carrot3Collision(Game &ga);
void Carrot4Collision(Game &ga);
void Carrot5Collision(Game &ga);

void LettuceCollision(Game &ga);
void Lettuce1Collision(Game &ga);
void Lettuce2Collision(Game &ga);
void Lettuce3Collision(Game &ga);

void TomatoCollision(Game &ga);
void Tomato1Collision(Game &ga);
void Tomato2Collision(Game &ga);
void Tomato3Collision(Game &ga);
void Tomato4Collision(Game &ga);
void Tomato5Collision(Game &ga);
void Tomato6Collision(Game &ga);

void EggplantCollision(Game &ga);
void Eggplant1Collision(Game &ga);
void Eggplant2Collision(Game &ga);
void Eggplant3Collision(Game &ga);
void Eggplant4Collision(Game &ga);

void BossCollision(Game &ga);

//extern vector<Enemy> enemyList;
//void EnemyCollision(Game &ga, vector<Enemy> &enemyList,
//        float collisionThreshold);
//void EnemyCollision(Game &ga, vector<Enemy> &enemies, 
//        float collisionThreshold);
//bool checkCollision(float bulletX, float bulletY, float enemyX, float enemyY, 
  //      float threshold);

extern int currentGunIndex;
extern vector<Gun> guns;

#endif // GAME_H
