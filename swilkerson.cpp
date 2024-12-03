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
    Gun("Pistol", 15.0, 0.3, 10, 1.5f, 1, 0.0f),
    Gun("Shotgun", 10.0, 1.0, 8, 3.0f, 3, 0.2f),
    Gun("AR", 20.0, 0.3, 15, 2.0f, 1, 0.0f),
    Gun("LMG", 25.0, 0.5, 50, 2.5f, 1, 0.0f),  
    Gun("Sniper", 50.0, 1.0, 5, 3.5f, 1, 0.0f),  
    Gun("Rocket Launcher", 10.0, 5.0, 1, 5.0f, 1, 0.0f),  
    Gun("Ray Gun", 20.0, 0.5, 20, 2.0f, 1, 0.0f)
};


//Delay for guns
bool Gun::canShoot(struct timespec &lastShotTime) {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    double timeDiff = (currentTime.tv_sec - lastShotTime.tv_sec) +
                      (currentTime.tv_nsec - lastShotTime.tv_nsec) / 1e9;
    return timeDiff >= cooldown;
}

bool Gun::purchase(int &playerScore, int cost) {
    if (playerScore >= cost && !purchased) {
        playerScore -= cost;
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
        return;

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

         float angleOffset = (i - currentGun.spreadCount / 2) * 
             currentGun.spreadAngle;
        float angle = atan2(dy, dx) + angleOffset;
       // float angle = atan2(dy, dx);

        // + angleOffset;
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

void update_bullets(unsigned char map[16][31][30], int row, int col, 
        float offx, float offy, float tile[2], int stage) {
    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];

        //Move bullet
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
       // ++i;

        //bool collision = false;
        for (int z = 0; z < col; ++z) { 
            for (int j = 0; j < row; ++j) { 
                //Check tile 
                if ((map[stage][z][j] == 'w' || map[stage][z][j] == 'b') &&
                b->pos[0] > tile[0] * j &&
                b->pos[0] < ((2*offx)+(tile[0]*j)) &&
                b->pos[1] > ((2*offy) + (tile[1]*z)) &&
                b->pos[1] < tile[1] * z) {
                //Delete bullet
                memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
                --ga.nbullets;
             //   collision = true;
                cout << "Bullet hit restricted tile at " << b->pos[0] << 
                    ", " << b->pos[1] << "\n";
                //eak;
            }
        }
       // if (collision) break;
        }
        i++;

        //Increment bullet if no collision occurred
       // if (!collision) {
       //     ++i;
      //  }
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

    for (int i = 0; i < ga.nbullets; i++) {
        Bullet *b = &ga.barr[i];
        glColor3f(1.0, 1.0, 1.0);  

        glBegin(GL_POLYGON);  //Went from GL_POINTS to GL_POLYGON to draw bullet
        int numSegments = 16; //Number of segments for a bullet
        for (int j = 0; j < numSegments; j++) {
            float angle = 2.0f * 3.14159265359f * float(j) / float(numSegments); 
            float x = b->pos[0] + cos(angle) * b->size;  
            float y = b->pos[1] + sin(angle) * b->size;  
            glVertex2f(x, y);
        }
        glEnd();
    }

  /*  for (int i=0; i<ga.nbullets; i++) {
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
    }*/
    
      
}
/*
bool checkCollisionWithEnemy(float bulletX, float bulletY, const Enemy &enemy, float threshold) {
    float dx = bulletX - enemy.x;
    float dy = bulletY - enemy.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold;
}

void EnemyCollision(Game &ga, vector<Enemy> &enemyList, float collisionThreshold) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet *b = &ga.barr[i];
        for (Enemy &enemy : enemyList) {
            if (enemy.active) {
                float dx = b->pos[0] - enemy.x;
                float dy = b->pos[1] - enemy.y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance <= collisionThreshold) {
                    //Bullet hit the enemy
                    enemy.health -= 1;
                    if (enemy.health <= 0) {
                        enemy.active = false; 
                        cout << "Enemy eliminated!" << endl;
                    }
                    
                    playerScore += 10;

                    
                    memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
                    --ga.nbullets;
                    --i; 
                    break; //Exit the loop 
                }
            }
        }
    }
}
*/
/*
void EnemyCollision(Game &ga, vector<Enemy> &enemies, float threshold) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];

        // Loop through each enemy
        for (auto &enemy : enemies) {
            if (enemy.active && checkCollisionWithEnemy(b->pos[0], b->pos[1], enemy, threshold)) {
                enemy.health -= 1;  // Decrease enemy health
                playerScore += 10;  // Increase score
                cout << "Bullet hit an enemy!" << endl;

                // Remove the bullet if it hits an enemy
                memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
                --ga.nbullets;

                // If the enemy's health reaches 0, deactivate the enemy
                if (enemy.health <= 0) {
                    enemy.active = false;
                    cout << "Enemy defeated!" << endl;
                }

                break;  // Exit the loop once the bullet hits an enemy
            }
        }
    }
}
*/

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
            carrotHealth -= 1;
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
            carrotHealth1 -= 1;
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
            carrotHealth2 -= 1;
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
            carrotHealth3 -= 1;
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
            carrotHealth4 -= 1;
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
            carrotHealth5 -= 1;
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
            lettuceHealth -= 1;
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
            lettuceHealth1 -= 1;
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
            lettuceHealth2 -= 1;
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
            lettuceHealth3 -= 1;
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
            tomatoHealth -= 1;
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
            tomatoHealth1 -= 1;
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
            tomatoHealth2 -= 1;
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
            tomatoHealth3 -= 1;
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
            tomatoHealth4 -= 1;
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
            tomatoHealth5 -= 1;
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
            tomatoHealth6 -= 1;
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
            eggplantHealth -= 1;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
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
            eggplantHealth1 -= 1;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
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
            eggplantHealth2 -= 1;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
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
            eggplantHealth3 -= 1;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
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
            eggplantHealth4 -= 1;
            playerScore += 10;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

void BossCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];
        if (checkCollisionB(b->pos[0], b->pos[1], bossX, bossY, collisionThreshold)) {
            bossHealth -= 1;
            playerScore += 20;
            cout << "Bullet hit the enemy!" << endl;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets - 1], sizeof(Bullet));
            --ga.nbullets;
            break;
        }
    }
}

/*
bool checkCollisionCarrot(float bulletX, float bulletY, float carrotX, 
        float carrotY, float threshold) {
    float dx = bulletX - carrotX;
    float dy = bulletY - carrotY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold;
}

void CarrotCollision(Game &ga) {
    for (int i = 0; i < ga.nbullets; ++i) {
        Bullet* b = &ga.barr[i];  

        if (checkCollisionCarrot(b->pos[0], b->pos[1], carrotX, carrotY, 
                    collisionCarrotThreshold)) {
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
*/

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
        sprintf(gun_info, "%s: %d/%d - Reloading", currentGun.name.c_str(), 
                currentGun.currentAmmo, currentGun.ammoCapacity);
    } else {
        
        sprintf(gun_info, "%s: %d/%d", currentGun.name.c_str(), 
                currentGun.currentAmmo, currentGun.ammoCapacity);
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
    clock_gettime(CLOCK_REALTIME, &currentGun.reloadStartTime);  
    //Record reload start time
    cout << "Reloading " << currentGun.name << "..." << endl;
}

void update_reload() {
    Gun &currentGun = guns[currentGunIndex];

    //If reloading, check time
    if (currentGun.isReloading) {
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);  //Get time

        //Calculate time since reload 
        double timeDiff = (currentTime.tv_sec - 
        currentGun.reloadStartTime.tv_sec) +
        (currentTime.tv_nsec - currentGun.reloadStartTime.tv_nsec) / 1e9;

        //Reload after 2 sec
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
        "Shotgun - $20 - press 'z' to purchase",
        "AR - $30 - press 'x' to purchase",
        "LMG - $50 - press 'c' to purchase",
        "Sniper - $40 - press 'v' to purchase",
        "Rocket Launcher - $60 - press 'n' to purchase",
        "Ray Gun - $70 - press 'm' to purchase"
    };

    Rect item;
    item.bot = yres - 100;
    item.left = xres / 4;
    item.center = 0;

    for (size_t i = 0; i < 6; ++i) {
        string displayText = gunNames[i]; 
        //Convert guns to string

        //Purchased status if the gun is already bought
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
        case 'z':  // Shotgun
            cost = 20; // Price for Shotgun
            if (!guns[1].purchased && guns[1].purchase(playerScore, cost)) {
                guns[1].purchased = true;  // Mark Shotgun as purchased
                printf("Bought Shotgun! Remaining score: %d\n", playerScore);
            } else if (guns[1].purchased) {
                printf("Shotgun already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'x':  // AR
            cost = 30; // Price for AR
            if (!guns[2].purchased && guns[2].purchase(playerScore, cost)) {
                guns[2].purchased = true;  // Mark AR as purchased
                printf("Bought AR! Remaining score: %d\n", playerScore);
            } else if (guns[2].purchased) {
                printf("AR already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'c':  // LMG
            cost = 40; // Price for Sniper
            if (!guns[3].purchased && guns[3].purchase(playerScore, cost)) {
                guns[3].purchased = true;  // Mark Sniper as purchased
                printf("Bought Sniper! Remaining score: %d\n", playerScore);
            } else if (guns[3].purchased) {
                printf("Sniper already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'v':  // Sniper
            cost = 50; // Price for LMG
            if (!guns[4].purchased && guns[4].purchase(playerScore, cost)) {
                guns[4].purchased = true;  // Mark LMG as purchased
                printf("Bought LMG! Remaining score: %d\n", playerScore);
            } else if (guns[4].purchased) {
                printf("LMG already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'n':  // Rocket Launcher
            cost = 60; // Price for Rocket Launcher
            if (!guns[5].purchased && guns[5].purchase(playerScore, cost)) {
                guns[5].purchased = true;  // Mark Rocket Launcher as purchased
                printf("Bought Rocket Launcher! Remaining score: %d\n", 
                        playerScore);
            } else if (guns[5].purchased) {
                printf("Rocket Launcher already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
        case 'm':  // Ray Gun
            cost = 70; // Price for Ray Gun
            if (!guns[6].purchased && guns[6].purchase(playerScore, cost)) {
                guns[6].purchased = true;  // Mark Ray Gun as purchased
                printf("Bought Ray Gun! Remaining score: %d\n", playerScore);
            } else if (guns[6].purchased) {
                printf("Ray Gun already purchased!\n");
            } else {
                printf("Not enough points!\n");
            }
            break;
    }
}




