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
//extern float eggplantX;
//extern float eggplantY;
//extern float carrotX;  
//extern float carrotY;
//extern int eggplantHealth;
extern float collisionThreshold;         
extern float collisionCarrotThreshold;   
extern float collisionTomatoThreshold;   
extern float collisionLettuceThreshold;  
//extern int carrotHealth, tomatoHealth, lettuceHealth;
extern int playerScore;

extern float carrotX;
extern float carrotY;
extern float carrotX1;
extern float carrotY1;
extern float carrotX2;
extern float carrotY2;
extern float carrotX3;
extern float carrotY3;
extern float carrotX4;
extern float carrotY4;
extern float carrotX5;
extern float carrotY5;

extern float tomatoX;
extern float tomatoY;
extern float tomatoX1;
extern float tomatoY1;
extern float tomatoX2;
extern float tomatoY2;
extern float tomatoX3;
extern float tomatoY3;
extern float tomatoX4;
extern float tomatoY4;
extern float tomatoX5;
extern float tomatoY5;
extern float tomatoX6;
extern float tomatoY6;

extern float brockX;
extern float brockY;

extern float eggplantX;
extern float eggplantY;
extern float eggplantX1;
extern float eggplantY1;
extern float eggplantX2;
extern float eggplantY2;
extern float eggplantX3;
extern float eggplantY3;
extern float eggplantX4;
extern float eggplantY4;

extern float lettuceX;
extern float lettuceY;
extern float lettuceX1;
extern float lettuceY1;
extern float lettuceX2;
extern float lettuceY2;
extern float lettuceX3;
extern float lettuceY3;

extern float bossX;
extern float bossY;
extern int bossHealth;

// TOMATOES
extern int tomatoHealth;
extern int tomatoHealth1;
extern int tomatoHealth2;
extern int tomatoHealth3;
extern int tomatoHealth4;
extern int tomatoHealth5;
extern int tomatoHealth6;

// CARROTS
extern int carrotHealth;
extern int carrotHealth1;
extern int carrotHealth2;
extern int carrotHealth3;
extern int carrotHealth4;
extern int carrotHealth5;

// LETTUCE
extern int lettuceHealth;
extern int lettuceHealth1;
extern int lettuceHealth2;
extern int lettuceHealth3;

// EGGPLANTS
extern int eggplantHealth;
extern int eggplantHealth1;
extern int eggplantHealth2;
extern int eggplantHealth3;
extern int eggplantHealth4;

using namespace std;

//Last time shot for all guns
struct timespec lastShotAR = {0, 0};  
struct timespec lastShotShotgun = {0, 0};  
struct timespec lastShotPistol = {0, 0};  
struct timespec lastShotLMG = {0, 0};  
struct timespec lastShotSniper = {0, 0};  
struct timespec lastShotRocketLauncher = {0, 0};  
struct timespec lastShotRayGun = {0, 0};

int currentGunIndex = 0;
vector<Gun> guns = {
    //name, bullet speed, cooldown, ammo size, bullet size, spread, angle  
    Gun("Pistol", 15.0, 0.3, 10, 1.5f, 1, 0.0f),
    Gun("Shotgun", 10.0, 1.0, 8, 3.0f, 3, 0.2f),
    Gun("AR", 20.0, 0.3, 15, 3.0f, 1, 0.0f),
    Gun("LMG", 25.0, 0.5, 50, 5.0f, 1, 0.0f),  
    Gun("Sniper", 50.0, 1.0, 5, 10.0f, 1, 0.0f),  
    Gun("Rocket Launcher", 10.0, 5.0, 1, 5.0f, 1, 0.0f),  
    Gun("Ray Gun", 20.0, 0.5, 20, 2.0f, 1, 0.0f)
};


//delay for guns
bool Gun::canShoot(struct timespec &lastShotTime) {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime); //gets time
    double timeDiff = (currentTime.tv_sec - lastShotTime.tv_sec) +
                      (currentTime.tv_nsec - lastShotTime.tv_nsec) / 1e9;
    //gun can shoot again when difference is equal to time elapsed
    return timeDiff >= cooldown;
    //help with time intervals
    //https://gist.github.com/error454/1448313/
    //6850ca7a82bf4edb3c5bafaf97ba73b847a2f2e3
}

//function to see if gun is purchased
bool Gun::purchase(int &playerScore, int cost) {
    if (playerScore >= cost && !purchased) {
        playerScore -= cost; //subtracts cost from score when purchased
        purchased = true;
        return true;
    }
    return false;
}

void fire_bullet(int mx, int my) {
    Gun &currentGun = guns[currentGunIndex];
    struct timespec &lastShotTime = (currentGunIndex == 0) ? lastShotAR : 
        lastShotShotgun; //checks last time shot for any weapon

    if (currentGun.isReloading) {  //Check to see if reloading
        cout << "Reloading, cannot shoot!" << endl;
        return;
    }

    if (!currentGun.canShoot(lastShotTime) || currentGun.currentAmmo <= 0) 
        return; //cannot shoot before cooldown or when ammo is 0

    for (int i = 0; i < currentGun.spreadCount; ++i) {
        if (ga.nbullets >= MAX_BULLETS) break;

        Bullet new_bullet; //new bullet created 
        new_bullet.size = currentGun.bulletSize;  
        new_bullet.pos[0] = bal.pos[0];
        new_bullet.pos[1] = bal.pos[1];


        float dx = mx - new_bullet.pos[0];
        float dy = new_bullet.pos[1] - my;

        float angleOffset = (i - currentGun.spreadCount / 2) * //calculation for shotgun
                            currentGun.spreadAngle;
        float angle = atan2(dy, dx) + angleOffset; //tangent inverse 
        //https://www.geeksforgeeks.org/atan2-function-in-c-stl/

        //give bullet velocity in x and y directions
        new_bullet.vel[0] = cos(angle) * currentGun.bulletSpeed; 
        new_bullet.vel[1] = sin(angle) * currentGun.bulletSpeed;
        clock_gettime(CLOCK_REALTIME, &new_bullet.time);
        ga.barr[ga.nbullets++] = new_bullet; //add to bullet array
    }

    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    lastShotTime = currentTime;
    currentGun.currentAmmo--;
}

//function to call in physics 
void update_bullets() {
    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];

        //move bullet
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
        
        //deletes bullets if they go off the edge of the screen
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

    
    ga.check_bullet_lifetime();
}

void render_bullets() {

    for (int i = 0; i < ga.nbullets; i++) {
        Bullet *b = &ga.barr[i];
        glColor3f(1.0, 1.0, 1.0);  
        //Went from GL_POINTS to GL_POLYGON to draw filled shape
        glBegin(GL_POLYGON);  
       // https://stackoverflow.com/questions/46258919/drawing-circle-with
       // -gl-polygon-radius-out-of-scale
        int numSegments = 16; //determines smoothness
        for (int j = 0; j < numSegments; j++) {
            //            2 radians in circle * pi * current segment / 16
            float angle = 2.0f * 3.14159265359f * float(j) / float(numSegments); 
            float x = b->pos[0] + cos(angle) * b->size;  
            float y = b->pos[1] + sin(angle) * b->size;  
            glVertex2f(x, y);
        }
        glEnd();
    }
}

void display_gun_info() {
    //get current gun
    Gun &currentGun = guns[currentGunIndex];

    Rect r;
    r.bot = 25;
    r.left = g.xres - 250;
    r.center = 0;


    char gun_info[128];
    if (currentGun.isReloading) {
        //Print reloading
        sprintf(gun_info, "%s: %d/%d - Reloading", currentGun.name.c_str(),
                currentGun.currentAmmo, currentGun.ammoCapacity);
    } else {

        sprintf(gun_info, "%s: %d/%d", currentGun.name.c_str(),
                currentGun.currentAmmo, currentGun.ammoCapacity);
    }

    unsigned int text_color = 0xFFFFFFFF;

    glEnable(GL_TEXTURE_2D);
    ggprint8b(&r, 16, text_color, gun_info);

     Rect r2;
    r2.bot = r.bot;                 
    r2.left = r.left + 250;         
    r2.center = 0;

    ggprint8b(&r2, 16, text_color, "Press 'O' to shop");

    glDisable(GL_TEXTURE_2D);
}

void reload() {
    Gun &currentGun = guns[currentGunIndex];
    //make sure gun is not currently reloading
    if (currentGun.isReloading) {
        return;
    }

    currentGun.isReloading = true;
    clock_gettime(CLOCK_REALTIME, &currentGun.reloadStartTime);
    //Record reload start time
    cout << "Reloading " << currentGun.name << "..." << endl;
}

void update_reload() {
    Gun &currentGun = guns[currentGunIndex];

    //if reloading, check time
    if (currentGun.isReloading) {
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);  //Get time

        //calculate time since reload
        double timeDiff = (currentTime.tv_sec -
        currentGun.reloadStartTime.tv_sec) +
        (currentTime.tv_nsec - currentGun.reloadStartTime.tv_nsec) / 1e9;

        //reload after 2 sec
        if (timeDiff >= 2.0) {
            currentGun.currentAmmo = currentGun.ammoCapacity;  //Refill ammo
            currentGun.isReloading = false;  //Set reloading to back to false
            cout << "Reloaded " << currentGun.name << "!" << endl;
        }
    }
}

void renderShop(int xres, int yres, vector<Gun>& guns) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Helps create translucent background
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); //Black with 50% opacity
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, yres);
        glVertex2i(xres, yres);
        glVertex2i(xres, 0);
    glEnd();

    //Shop title
    Rect title;
    title.bot = yres - 50;
    title.left = xres / 2 - 100;
    title.center = 1;

    unsigned int text_color = 0xFFFFFFFF;

    glEnable(GL_TEXTURE_2D);
    ggprint16(&title, 32, text_color, "The Shop");

    //Gun list
    const char* gunNames[] = {
        "Shotgun(2) - $20 - press 'z' to purchase",
        "AR(3) - $30 - press 'x' to purchase",
        "LMG(4) - $40 - press 'c' to purchase",
        "Sniper(5) - $50 - press 'v' to purchase",
        "Rocket Launcher(6) - $1000000 - press 'n' to purchase",
        "Ray Gun(7) - $1000000 - press 'm' to purchase"
    };

    Rect item;
    item.bot = yres - 100;
    item.left = xres / 4;
    item.center = 0;

    for (size_t i = 0; i < 6; ++i) {
        string displayText = gunNames[i];
        //convert guns to string

        //purchased status if the gun is already bought
        if (guns[i + 1].purchased) {
            displayText += " (Purchased)";
        }

        ggprint16(&item, 32, text_color, displayText.c_str());
        item.bot -= 50;
    }

    glDisable(GL_BLEND);
}

void shopGuns(unsigned char key, int &playerScore) {
    if (!openShop) return;

    int cost = 0;

    switch (key) {
        case 'z':  //shotgun
            cost = 20; //price 
            if (!guns[1].purchased && guns[1].purchase(playerScore, cost)) {
                guns[1].purchased = true;  //mark shotgun sold
                printf("Bought Shotgun! Remaining score: %d\n", playerScore);
            } else if (guns[1].purchased) {
                printf("Shotgun already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'x':  //AR
            cost = 30; //price 
            if (!guns[2].purchased && guns[2].purchase(playerScore, cost)) {
                guns[2].purchased = true;  
                printf("Bought AR! Remaining score: %d\n", playerScore);
            } else if (guns[2].purchased) {
                printf("AR already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'c':  //LMG
            cost = 40; 
            if (!guns[3].purchased && guns[3].purchase(playerScore, cost)) {
                guns[3].purchased = true;  
                printf("Bought Sniper! Remaining score: %d\n", playerScore);
            } else if (guns[3].purchased) {
                printf("Sniper already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'v':  //Sniper
            cost = 50; 
            if (!guns[4].purchased && guns[4].purchase(playerScore, cost)) {
                guns[4].purchased = true;  
                printf("Bought LMG! Remaining score: %d\n", playerScore);
            } else if (guns[4].purchased) {
                printf("LMG already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
         case 'n':  //Rocket Launcher
            cost = 1000000; 
            if (!guns[5].purchased && guns[5].purchase(playerScore, cost)) {
                guns[5].purchased = true;  
                printf("Bought Rocket Launcher! Remaining score: %d\n",
                        playerScore);
            } else if (guns[5].purchased) {
                printf("Rocket Launcher already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'm':  //Ray gun
            cost = 1000000; 
            if (!guns[6].purchased && guns[6].purchase(playerScore, cost)) {
                guns[6].purchased = true;  
                printf("Bought Ray Gun! Remaining score: %d\n", playerScore);
            } else if (guns[6].purchased) {
                printf("Ray Gun already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
    }
}



bool checkCollisionB(float bulletX, float bulletY, float enemyX, float enemyY, float threshold) {
    float dx = bulletX - enemyX;
    float dy = bulletY - enemyY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold;
}

void CarrotCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX, carrotY, collisionCarrotThreshold)) {
            carrotHealth -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Carrot1Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX1, carrotY1, collisionCarrotThreshold)) {
            carrotHealth1 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Carrot2Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX2, carrotY2, collisionCarrotThreshold)) {
            carrotHealth2 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Carrot3Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX3, carrotY3, collisionCarrotThreshold)) {
            carrotHealth3 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Carrot4Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX4, carrotY4, collisionCarrotThreshold)) {
            carrotHealth4 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Carrot5Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], carrotX5, carrotY5, collisionCarrotThreshold)) {
            carrotHealth5 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void LettuceCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], lettuceX, lettuceY, collisionLettuceThreshold)) {
            lettuceHealth -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Lettuce1Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], lettuceX1, lettuceY1, collisionLettuceThreshold)) {
            lettuceHealth1 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Lettuce2Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], lettuceX2, lettuceY2, collisionLettuceThreshold)) {
            lettuceHealth2 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Lettuce3Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], lettuceX3, lettuceY3, collisionLettuceThreshold)) {
            lettuceHealth3 -= 5;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void TomatoCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX, tomatoY, collisionTomatoThreshold)) {
            tomatoHealth -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato1Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX1, tomatoY1, collisionTomatoThreshold)) {
            tomatoHealth1 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato2Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX2, tomatoY2, collisionTomatoThreshold)) {
            tomatoHealth2 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato3Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX3, tomatoY3, collisionTomatoThreshold)) {
            tomatoHealth3 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato4Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX4, tomatoY4, collisionTomatoThreshold)) {
            tomatoHealth4 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato5Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX5, tomatoY5, collisionTomatoThreshold)) {
            tomatoHealth5 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Tomato6Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], tomatoX6, tomatoY6, collisionTomatoThreshold)) {
            tomatoHealth6 -= 3;
            playerScore += 10;
            cout << "Bullet hit the tomatoe!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void EggplantCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], eggplantX, eggplantY, collisionThreshold)) {
            eggplantHealth -= 5;
            playerScore += 10;
            cout << "Bullet hit the eggplant!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Eggplant1Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], eggplantX1, eggplantY1, collisionThreshold)) {
            eggplantHealth1 -= 5;
            playerScore += 10;
            cout << "Bullet hit the eggplant1!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Eggplant2Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], eggplantX2, eggplantY2, collisionThreshold)) {
            eggplantHealth2 -= 5;
            playerScore += 10;
            cout << "Bullet hit the eggplant2!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Eggplant3Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], eggplantX3, eggplantY3, collisionThreshold)) {
            eggplantHealth3 -= 5;
            playerScore += 10;
            cout << "Bullet hit the eggplant3!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void Eggplant4Collision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], eggplantX4, eggplantY4, collisionThreshold)) {
            eggplantHealth4 -= 5;
            playerScore += 10;
            cout << "Bullet hit the eggplant4!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void BossCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], bossX, bossY, 80.0f)) {
            bossHealth -= 10;
            playerScore += 20;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}


