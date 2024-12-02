//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
#include <unistd.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "sgonzales.h"
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <cstddef>
#include "game.h"
#define MAX_BULLETS 10000 
//#define MAX_ENEMIES 100 
#define MAX_HEALTH 20
const float shootInterval = 0.1f;
int playerHealth = MAX_HEALTH;
int maxHealth = playerHealth;
float collisionThreshold = 10.0f;
float collisionCarrotThreshold = 35.0f;
float collisionTomatoThreshold = 35.0f;
float collisionLettuceThreshold = 50.0f;
//CARROTS
float carrotX = 100.0f;
float carrotY = 100.0f;
float carrotX1 = 100.0f;
float carrotY1 = 100.0f;
float carrotX2 = 100.0f;
float carrotY2 = 100.0f;
float carrotX3 = 100.0f;
float carrotY3 = 100.0f;
float carrotX4 = 100.0f;
float carrotY4 = 100.0f;
float carrotX5 = 100.0f;
float carrotY5 = 100.0f;
//////////////////////
//TOMATO
float tomatoX = 0.0f;
float tomatoY = 0.0f;
float tomatoX1 = 0.0f;
float tomatoY1 = 0.0f;
float tomatoX2 = 0.0f;
float tomatoY2 = 0.0f;
float tomatoX3 = 0.0f;
float tomatoY3 = 0.0f;
float tomatoX4 = 0.0f;
float tomatoY4 = 0.0f;
float tomatoX5 = 0.0f;
float tomatoY5 = 0.0f;
float tomatoX6 = 0.0f;
float tomatoY6 = 0.0f;
/////////////////////
float brockX = 0.0f;
float brockY = 0.0f;
//EGGPLANTS / LETTUCE
float eggplantX = 100.0f; 
float eggplantY = 200.0f;
float eggplantX1 = 100.0f; 
float eggplantY1 = 150.0f;
float eggplantX2 = 100.0f; 
float eggplantY2 = 150.0f;
float eggplantX3 = 100.0f; 
float eggplantY3 = 150.0f;
float eggplantX4 = 100.0f; 
float eggplantY4 = 150.0f;

float lettuceX = 150.0f; 
float lettuceY = 100.0f;
float lettuceX1 = 150.0f; 
float lettuceY1 = 100.0f;
float lettuceX2 = 150.0f; 
float lettuceY2 = 100.0f;
float lettuceX3 = 150.0f; 
float lettuceY3 = 100.0f;
//////////////////////////
float bossX = 500.0f;
float bossY = 200.0f;

float slowdown = 0.05f;
float lastShotTime = 0.0f;
extern Ball bal;
//TOMATOES
int tomatoHealth = 10;
int tomatoHealth1 = 10;
int tomatoHealth2 = 10;
int tomatoHealth3 = 10;
int tomatoHealth4 = 10;
int tomatoHealth5 = 10;
int tomatoHealth6 = 10;
////////////////////////
//CARROTS
int carrotHealth = 5;
int carrotHealth1 = 5;
int carrotHealth2 = 5;
int carrotHealth3 = 5;
int carrotHealth4 = 5;
int carrotHealth5 = 5;
//
// LETTUCE AND EGGPLANT
int lettuceHealth = 25;
int lettuceHealth1 = 25;
int lettuceHealth2 = 25;
int lettuceHealth3 = 25;

int eggplantHealth = 40;
int eggplantHealth1 = 40;
int eggplantHealth2 = 40;
int eggplantHealth3 = 40;
int eggplantHealth4 = 40;
///////////////////////
int playerScore = 0;
int bossHealth = 350;
extern int xres;
extern int yres;
struct Projectile {
    float x, y; 
    float dx, dy;  
    float speed;  
};

Enemy enemies[MAX_ENEMIES]; 

int enemyCount = 0;

Projectile projectiles[MAX_BULLETS];  
int bulletCount = 0;
Projectile enemyProjectiles[MAX_BULLETS];
Projectile enemyGreenProjectiles[MAX_BULLETS];
Projectile enemyOrangeProjectiles[MAX_BULLETS];
Projectile enemyPurpleProjectiles[MAX_BULLETS];

void shootEnemyBullet(float tomatoX, float tomatoY, float playerX, float playerY)
{

    if (bulletCount < MAX_BULLETS) { 
        float dx = playerX - tomatoX;
        float dy = playerY - tomatoY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance; 
        dy /= distance;
        Projectile newProjectile;
        newProjectile.x = tomatoX;  
        newProjectile.y = tomatoY;
        newProjectile.dx = dx;   
        newProjectile.dy = dy;
        newProjectile.speed = 5.0f;
        enemyProjectiles[bulletCount] = newProjectile;
        bulletCount++;  
    }
}

bool meleeCollision(float brockX, float brockY, float brockSize, float enemyX,float enemySize, float enemyY) {
    float dx = brockX - enemyX;
    float dy = brockY - enemyY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (brockSize / 2 + enemySize / 2);
}

bool checkCollision(float bulletX, float bulletY, float tomatoX, float tomatoY, float threshold) {
    float dx = bulletX - tomatoX;
    float dy = bulletY - tomatoY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= threshold; 
}
void shootGreenEnemyBullet(float tomatoX, float tomatoY, float playerX, float playerY)
{

    if (bulletCount < MAX_BULLETS) {
        float dx = playerX - tomatoX;
        float dy = playerY - tomatoY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance;
        dy /= distance;
        Projectile newProjectile;
        newProjectile.x = tomatoX;
        newProjectile.y = tomatoY;
        newProjectile.dx = dx;
        newProjectile.dy = dy;
        newProjectile.speed = 5.0f;
        enemyGreenProjectiles[bulletCount] = newProjectile;
        bulletCount++;
    }
}
void shootPurpleEnemyBullet(float tomatoX, float tomatoY, float playerX, float playerY)
{

    if (bulletCount < MAX_BULLETS) {
        float dx = playerX - tomatoX;
        float dy = playerY - tomatoY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance;
        dy /= distance;
        Projectile newProjectile;
        newProjectile.x = tomatoX;
        newProjectile.y = tomatoY;
        newProjectile.dx = dx;
        newProjectile.dy = dy;
        newProjectile.speed = 5.0f;
        enemyPurpleProjectiles[bulletCount] = newProjectile;
        bulletCount++;
    }
}


void shootBullet(float brockX, float brockY, float playerX, float playerY) 
{
    if (bulletCount < MAX_BULLETS) { 
        float dx = playerX - brockX;
        float dy = playerY - brockY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance; 
        dy /= distance;
        Projectile newProjectile;
        newProjectile.x = brockX;
        newProjectile.y = brockY;
        newProjectile.dx = dx;
        newProjectile.dy = dy;
        newProjectile.speed = 5.0f;
        projectiles[bulletCount] = newProjectile;
        bulletCount++;  
    }

}


void shootOrangeEnemyBullet(float tomatoX, float tomatoY, float playerX, float playerY)
{

    if (bulletCount < MAX_BULLETS) {
        float dx = playerX - tomatoX;
        float dy = playerY - tomatoY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance;
        dy /= distance;
        Projectile newProjectile;
        newProjectile.x = tomatoX;
        newProjectile.y = tomatoY;
        newProjectile.dx = dx;
        newProjectile.dy = dy;
        newProjectile.speed = 5.0f;
        enemyOrangeProjectiles[bulletCount] = newProjectile;
        bulletCount++;
    }
}

void updateBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        projectiles[i].x += projectiles[i].dx * projectiles[i].speed;  
        projectiles[i].y += projectiles[i].dy * projectiles[i].speed;

    }
}

void updateEnemyBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyProjectiles[i].x += enemyProjectiles[i].dx * enemyProjectiles[i].speed;
        enemyProjectiles[i].y += enemyProjectiles[i].dy * enemyProjectiles[i].speed;

    }
}

void updateGreenEnemyBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyGreenProjectiles[i].x += enemyGreenProjectiles[i].dx * enemyGreenProjectiles[i].speed;
        enemyGreenProjectiles[i].y += enemyGreenProjectiles[i].dy * enemyGreenProjectiles[i].speed;

    }
}

void updateOrangeEnemyBullets()
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyOrangeProjectiles[i].x += enemyOrangeProjectiles[i].dx * enemyOrangeProjectiles[i].speed;
        enemyOrangeProjectiles[i].y += enemyOrangeProjectiles[i].dy * enemyOrangeProjectiles[i].speed;

    }
}
void updatePurpleEnemyBullets()
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyPurpleProjectiles[i].x += enemyPurpleProjectiles[i].dx * enemyPurpleProjectiles[i].speed;
        enemyPurpleProjectiles[i].y += enemyPurpleProjectiles[i].dy * enemyPurpleProjectiles[i].speed;

    }
}


void removeBullet(int index) {
    if (index < 0 || index >= bulletCount) {
        return; // Invalid index
    }

    // Shift all bullets after the specified index
    for (int i = index; i < bulletCount - 1; i++) {
        projectiles[i] = projectiles[i + 1];
    }

    // Decrement the bullet count
    --bulletCount;
}
void removeEnemyBullet(int index) {
    if (index < 0 || index >= bulletCount) {
        return; // Invalid index
    }
    for (int i = index; i < bulletCount - 1; i++) {
        //Check for collision with window edges
        if (enemyProjectiles[i].x < 0.0) {
            enemyProjectiles[i].x += (float)g.xres;
        }
        else if (enemyProjectiles[i].x > (float)g.xres) {
            enemyProjectiles[i].x -= (float)g.xres;
        }
        else if (enemyProjectiles[i].y < 0.0) {
            enemyProjectiles[i].y += (float)g.yres;
        }
        else if (enemyProjectiles[i].y > (float)g.yres) {
            enemyProjectiles[i].y -= (float)g.yres;
        }
        else if (enemyGreenProjectiles[i].x < 0.0) {
            enemyGreenProjectiles[i].x += (float)g.xres;
        }
        else if (enemyGreenProjectiles[i].x > (float)g.xres) {
            enemyGreenProjectiles[i].x -= (float)g.xres;
        }
        else if (enemyGreenProjectiles[i].y < 0.0) {
            enemyGreenProjectiles[i].y += (float)g.yres;
        }
        else if (enemyGreenProjectiles[i].y > (float)g.yres) {
            enemyGreenProjectiles[i].y -= (float)g.yres;
        }
        else if (enemyOrangeProjectiles[i].x < 0.0) {
            enemyOrangeProjectiles[i].x += (float)g.xres;
        }
        else if (enemyOrangeProjectiles[i].x > (float)g.xres) {
            enemyOrangeProjectiles[i].x -= (float)g.xres;
        }
        else if (enemyOrangeProjectiles[i].y < 0.0) {
            enemyOrangeProjectiles[i].y += (float)g.yres;
        }
        else if (enemyOrangeProjectiles[i].y > (float)g.yres) {
            enemyOrangeProjectiles[i].y -= (float)g.yres;
        }
        else if (enemyPurpleProjectiles[i].x < 0.0) {
            enemyPurpleProjectiles[i].x += (float)g.xres;
        }
        else if (enemyPurpleProjectiles[i].x > (float)g.xres) {
            enemyPurpleProjectiles[i].x -= (float)g.xres;
        }
        else if (enemyPurpleProjectiles[i].y < 0.0) {
            enemyPurpleProjectiles[i].y += (float)g.yres;
        }
        else if (enemyPurpleProjectiles[i].y > (float)g.yres) {
            enemyPurpleProjectiles[i].y -= (float)g.yres;
        }



    }



    // Shift all bullets after the specified index
    for (int i = index; i < bulletCount - 1; i++) {
        enemyProjectiles[i] = enemyProjectiles[i + 1];
    }
    for (int i = index; i < bulletCount - 1; i++) {
        enemyGreenProjectiles[i] = enemyGreenProjectiles[i + 1];
    }
    for (int i = index; i < bulletCount - 1; i++) {
        enemyOrangeProjectiles[i] = enemyOrangeProjectiles[i + 1];
    }
    for (int i = index; i < bulletCount - 1; i++) {
        enemyPurpleProjectiles[i] = enemyPurpleProjectiles[i + 1];
    }

    // Decrement the bullet count
    --bulletCount;
}

void drawBullet(const Projectile& projectile) 
{
    glColor3f(0.0f, 1.0f, 0.0f); 
    glPushMatrix();
    glTranslatef(projectile.x, projectile.y, 0.0f);  
    float size = 5.0f; 
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2); 
    glVertex2f(size / 2, -size / 2);   
    glVertex2f(size / 2, size / 2);   
    glVertex2f(-size / 2, size / 2); 
    glEnd();
    glPopMatrix();
}

void drawEnemyBullet(const Projectile& projectile) 
{
    glColor3f(1.0f, 0.0f, 0.0f); 
    glPushMatrix();
    glTranslatef(projectile.x, projectile.y, 0.0f);  
    float size = 5.0f; 
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2); 
    glVertex2f(size / 2, -size / 2);   
    glVertex2f(size / 2, size / 2);   
    glVertex2f(-size / 2, size / 2); 
    glEnd();
    glPopMatrix();
}

void drawGreenEnemyBullet(const Projectile& projectile)
{
    glColor3f(0.0f, 0.4f, 0.0f);
    glPushMatrix();
    glTranslatef(projectile.x, projectile.y, 0.0f);
    float size = 5.0f;
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
    glPopMatrix();
}
void drawPurpleEnemyBullet(const Projectile& projectile)
{
    glColor3f(0.5f, 0.0f, 0.5f);
    glPushMatrix();
    glTranslatef(projectile.x, projectile.y, 0.0f);
    float size = 18.0f;
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
    glPopMatrix();
}
void drawOrangeEnemyBullet(const Projectile& projectile)
{
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(projectile.x, projectile.y, 0.0f);
    float size = 5.0f;
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
    glPopMatrix();
}

void drawCarrot(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[0].x = carrotX;
    enemies[0].y = carrotY;
    enemies[0].size = 25.0f;
    float dx = playerX - carrotX;
    float dy = playerY - carrotY;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX += dx * speed;
        carrotY += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX,carrotY,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}

void drawCarrot1(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth1 <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[4].x = carrotX1;
    enemies[4].y = carrotY1;
    enemies[4].size = 25.0f;
    float dx = playerX - carrotX1;
    float dy = playerY - carrotY1;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX1 += dx * speed;
        carrotY1 += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX1,carrotY1,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth1 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth1);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}

void drawCarrot2(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth2 <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[5].x = carrotX2;
    enemies[5].y = carrotY2;
    enemies[5].size = 25.0f;
    float dx = playerX - carrotX2;
    float dy = playerY - carrotY2;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX2 += dx * speed;
        carrotY2 += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX2,carrotY2,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth2 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth2);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
void drawCarrot3(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth3 <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[6].x = carrotX3;
    enemies[6].y = carrotY3;
    enemies[6].size = 25.0f;
    float dx = playerX - carrotX3;
    float dy = playerY - carrotY3;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX3 += dx * speed;
        carrotY3 += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX3,carrotY3,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth3 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth3);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
void drawCarrot4(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth4 <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[7].x = carrotX4;
    enemies[7].y = carrotY4;
    enemies[7].size = 25.0f;
    float dx = playerX - carrotX4;
    float dy = playerY - carrotY4;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX4 += dx * speed;
        carrotY4 += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX4,carrotY4,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth4 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth4);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
void drawCarrot5(float playerX, float playerY) 
{
    //static int timer = time(NULL) + 2;
    if (carrotHealth5 <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 3.0f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    enemies[8].x = carrotX5;
    enemies[8].y = carrotY5;
    enemies[8].size = 25.0f;
    float dx = playerX - carrotX5;
    float dy = playerY - carrotY5;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        carrotX5 += dx * speed;
        carrotY5 += dy * speed;
    } 
    if (distance <= collisionThreshold && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }


    glTranslatef(carrotX5,carrotY5,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-12.0f, -6.0f);
    glVertex2f(-6.0f, -12.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            carrotHealth5 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth5);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}


void drawBrock(float playerX, float playerY) 
{
    static int timer = time(NULL) + 2;
    glColor3f(0.0f, 1.0f, 0.0f);  //green color
    glPushMatrix();
    float radius = 50.0f;
    float size = 25.0f;
    float speed = 5.0f;
    float angularSpeed = 0.05f;
    static float angle = 0.0f;
    brockX = playerX + radius * cos(angle);
    brockY = playerY + radius * sin(angle);
    angle += angularSpeed;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
    float dx = playerX - brockX;
    float dy = playerY - brockY;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;

        brockX += dx * speed;
        brockY += dy * speed;
    } else {
        brockX = playerX;
        brockY = playerY;
    }
    glTranslatef(brockX,brockY,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2); 
    glVertex2f(size / 2, -size / 2); 
    glVertex2f(size / 2, size / 2);   
    glVertex2f(-size / 2, size / 2);  
    glEnd();
    glPopMatrix();

    // Collision
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(enemyOrangeProjectiles[i].x, enemyOrangeProjectiles[i].y,
                    bal.pos[0], bal.pos[1], 20.0f) || checkCollision(enemyGreenProjectiles[i].x,
                        enemyGreenProjectiles[i].y,bal.pos[0], bal.pos[1], 20.0f) ||
                checkCollision(enemyProjectiles[i].x, enemyProjectiles[i].y,
                    bal.pos[0], bal.pos[1], 20.0f)){
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
            removeEnemyBullet(i);
            break;
        }
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(enemyPurpleProjectiles[i].x, enemyPurpleProjectiles[i].y,bal.pos[0], bal.pos[1], 20.0f)) {
            playerHealth -= 5;
            printf("Player health: %d\n", playerHealth);
            removeEnemyBullet(i);
            break;
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[0].x, enemies[0].size, enemies[0].y)) {
        if(carrotHealth > 0) {
            carrotHealth -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[1].x, enemies[1].size, enemies[1].y)) {
        if(tomatoHealth > 0) {
            tomatoHealth -= 1;
            playerScore += 10;
            printf("Tomato health: %d\n", tomatoHealth);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[2].x, enemies[2].size, enemies[2].y)) {
        if(lettuceHealth > 0) {
            lettuceHealth -= 1;
            playerScore += 10;
            printf("Lettuce health: %d\n", lettuceHealth);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[3].x, enemies[3].size, enemies[3].y)) {
        if(eggplantHealth > 0) {
            eggplantHealth -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[4].x, enemies[4].size, enemies[4].y)) {
        if(carrotHealth1 > 0) {
            carrotHealth1 -= 1;
            playerScore += 10;
            printf("Carrot health: %d\n", carrotHealth1);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[5].x, enemies[5].size, enemies[5].y)) {
        if(carrotHealth2 > 0) {
            carrotHealth2 -= 1;
            playerScore += 10;
            printf("carrot health: %d\n", carrotHealth2);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[6].x, enemies[6].size, enemies[6].y)) {
        if(carrotHealth3 > 0) {
            carrotHealth3 -= 1;
            playerScore += 10;
            printf("carrot health: %d\n", carrotHealth3);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[7].x, enemies[7].size, enemies[7].y)) {
        if(carrotHealth4 > 0) {
            carrotHealth4 -= 1;
            playerScore += 10;
            printf("carrot health: %d\n", carrotHealth4);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[8].x, enemies[8].size, enemies[8].y)) {
        if(carrotHealth5 > 0) {
            carrotHealth5 -= 1;
            playerScore += 10;
            printf("carrot health: %d\n", carrotHealth5);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[9].x, enemies[9].size, enemies[9].y)) {
        if(tomatoHealth1 > 0) {
            tomatoHealth1 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth1);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[10].x, enemies[10].size, enemies[10].y)) {
        if (tomatoHealth2 > 0) {
            tomatoHealth2 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth2);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[11].x, enemies[11].size, enemies[11].y)) {
        if (tomatoHealth3 > 0) {
            tomatoHealth3 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth3);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[12].x, enemies[12].size, enemies[12].y)) {
        if (tomatoHealth4 > 0) {
            tomatoHealth4 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth4);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[13].x, enemies[13].size, enemies[13].y)) {
        if (tomatoHealth5 > 0) {
            tomatoHealth5 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth5);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[14].x, enemies[14].size, enemies[14].y)) {
        if (tomatoHealth6 > 0) {
            tomatoHealth6 -= 1;
            playerScore += 10;
            printf("tomato health: %d\n", tomatoHealth6);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[15].x, enemies[15].size, enemies[15].y)) {
        if(lettuceHealth1 > 0) {
            lettuceHealth1 -= 1;
            playerScore += 10;
            printf("lettuce health: %d\n", lettuceHealth1);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[16].x, enemies[16].size, enemies[16].y)) {
        if (lettuceHealth2 > 0) {
            lettuceHealth2 -= 1;
            playerScore += 10;
            printf("lettuce health: %d\n", lettuceHealth2);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[17].x, enemies[17].size, enemies[17].y)) {
        if (lettuceHealth3 > 0) {
            lettuceHealth3 -= 1;
            playerScore += 10;
            printf("lettuce health: %d\n", lettuceHealth3);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
    if (meleeCollision(brockX, brockY, 25.0f, enemies[18].x, enemies[18].size, enemies[18].y)) {
        if (eggplantHealth1 > 0) {
            eggplantHealth1 -= 1;
            playerScore += 10;
            printf("eggplant health: %d\n", eggplantHealth1);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[19].x, enemies[19].size, enemies[19].y)) {
        if (eggplantHealth2 > 0) {
            eggplantHealth2 -= 1;
            playerScore += 10;
            printf("eggplant health: %d\n", eggplantHealth2);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[20].x, enemies[20].size, enemies[20].y)) {
        if (eggplantHealth3 > 0) {
            eggplantHealth3 -= 1;
            playerScore += 10;
            printf("eggplant health: %d\n", eggplantHealth3);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (meleeCollision(brockX, brockY, 25.0f, enemies[21].x, enemies[21].size, enemies[21].y)) {
        if (eggplantHealth4 > 0) {
            eggplantHealth4 -= 1;
            playerScore += 10;
            printf("eggplant health: %d\n", eggplantHealth4);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }

    if (time(NULL) > timer) {
        shootBullet(brockX, brockY, playerX, playerY);
        shootBullet(brockX, brockY, -playerX, -playerY);
        shootBullet(brockX, brockY, playerX, -playerY);
        shootBullet(brockX, brockY, -playerX, playerY);
        timer = time(NULL) + 2;
    }
    updateBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawBullet(projectiles[i]);
    }

}

void drawTomato(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[1].x = tomatoX;
    enemies[1].y = tomatoY;
    enemies[1].size = 35.0f;

    if (tomatoHealth <= 0) {
        enemies[1].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX;
    float dy = playerY - tomatoY;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX += dx * speed;
        tomatoY += dy * speed;
    } else {
        tomatoX = playerX;
        tomatoY = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX, tomatoY, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX, tomatoY, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionTomatoThreshold)) {
            tomatoHealth -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}

void drawTomato1(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[9].x = tomatoX1;
    enemies[9].y = tomatoY1;
    enemies[9].size = 35.0f;

    if (tomatoHealth1 <= 0) {
        enemies[9].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX1;
    float dy = playerY - tomatoY1;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX1 += dx * speed;
        tomatoY1 += dy * speed;
    } else {
        tomatoX1 = playerX;
        tomatoY1 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX1, tomatoY1, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX1, tomatoY1, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX1, tomatoY1, collisionTomatoThreshold)) {
            tomatoHealth1 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth1);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}
void drawTomato2(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[10].x = tomatoX;
    enemies[10].y = tomatoY;
    enemies[10].size = 35.0f;

    if (tomatoHealth2 <= 0) {
        enemies[10].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX2;
    float dy = playerY - tomatoY2;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX2 += dx * speed;
        tomatoY2 += dy * speed;
    } else {
        tomatoX2 = playerX;
        tomatoY2 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX2, tomatoY2, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX2, tomatoY2, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX2, tomatoY2, collisionTomatoThreshold)) {
            tomatoHealth2 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth2);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}
void drawTomato3(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[11].x = tomatoX;
    enemies[11].y = tomatoY;
    enemies[11].size = 35.0f;

    if (tomatoHealth3 <= 0) {
        enemies[11].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX3;
    float dy = playerY - tomatoY3;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX3 += dx * speed;
        tomatoY3 += dy * speed;
    } else {
        tomatoX3 = playerX;
        tomatoY3 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX3, tomatoY3, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX3, tomatoY3, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX3, tomatoY3, collisionTomatoThreshold)) {
            tomatoHealth3 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth3);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}
void drawTomato4(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[12].x = tomatoX4;
    enemies[12].y = tomatoY4;
    enemies[12].size = 35.0f;

    if (tomatoHealth4 <= 0) {
        enemies[12].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX4;
    float dy = playerY - tomatoY4;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX4 += dx * speed;
        tomatoY4 += dy * speed;
    } else {
        tomatoX4 = playerX;
        tomatoY4 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX4, tomatoY4, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX4, tomatoY4, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX4, tomatoY4, collisionTomatoThreshold)) {
            tomatoHealth4 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth4);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}
void drawTomato5(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[13].x = tomatoX5;
    enemies[13].y = tomatoY5;
    enemies[13].size = 35.0f;

    if (tomatoHealth5 <= 0) {
        enemies[13].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX5;
    float dy = playerY - tomatoY5;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX5 += dx * speed;
        tomatoY5 += dy * speed;
    } else {
        tomatoX5 = playerX;
        tomatoY5 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX5, tomatoY5, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX5, tomatoY5, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX5, tomatoY5, collisionTomatoThreshold)) {
            tomatoHealth5 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth5);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}
void drawTomato6(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    enemies[14].x = tomatoX6;
    enemies[14].y = tomatoY6;
    enemies[14].size = 35.0f;

    if (tomatoHealth6 <= 0) {
        enemies[14].active = false;
        return;
    }
    glColor3f(1.0f, 0.0f, 0.0f);  
    glPushMatrix();
    float speed = 2.0f;
    float dx = playerX - tomatoX6;
    float dy = playerY - tomatoY6;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        tomatoX6 += dx * speed;
        tomatoY6 += dy * speed;
    } else {
        tomatoX6 = playerX;
        tomatoY6 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }
    glTranslatef(tomatoX6, tomatoY6, 0.0f);
    float radius = 15.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    // information for code found at : 
    // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * (3.1415926f / 180);
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
    //trying to draw a stem
    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 4, radius / 2);
    glVertex2f(0.0f, radius + 5.0f);
    glVertex2f(radius / 4, radius / 2);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-7.0f, 5.0f);  
    glVertex2f(-5.0f, 5.0f); 
    glVertex2f(-5.0f, 7.0f);   
    glVertex2f(-7.0f, 7.0f);  
    glEnd();

    //Right eye
    glBegin(GL_QUADS);
    glVertex2f(5.0f, 5.0f);   
    glVertex2f(7.0f, 5.0f);   
    glVertex2f(7.0f, 7.0f);  
    glVertex2f(5.0f, 7.0f);   
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4.0f, -2.0f);
    glVertex2f(-2.0f, -4.0f);
    glVertex2f(2.0f, -4.0f);
    glVertex2f(4.0f, -2.0f);
    glEnd();

    glPopMatrix();


    if (time(NULL) > timer) {
        shootEnemyBullet(tomatoX6, tomatoY6, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawEnemyBullet(enemyProjectiles[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX6, tomatoY6, collisionTomatoThreshold)) {
            tomatoHealth6 -= 1;
            playerScore += 10; 
            printf("Tomato health: %d\n", tomatoHealth6);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break; 
        }
    }

}

void drawLettuce(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (lettuceHealth <= 0) {
        enemies[2].active = false;
        return;
    }

    glPushMatrix();
    //lettuceX = playerX + 150.0f; 
    //lettuceY = playerY + 150.0f;
    enemies[2].y = lettuceY;
    enemies[2].x = lettuceX;
    enemies[2].size = 40.0f;
    float speed = 1.5f;

    float dx = playerX - lettuceX;
    float dy = playerY - lettuceY;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        lettuceX += dx * speed;
        lettuceY += dy * speed;
    } else {
        lettuceX = playerX;
        lettuceY = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(lettuceX, lettuceY, 0.0f);

    //draw square lettuce
    glColor3f(0.5f, 1.0f, 0.5f);  
    float size = 20.0f;  
    glBegin(GL_QUADS);
    glVertex2f(-size, -size);  
    glVertex2f(size, -size);   
    glVertex2f(size, size);    
    glVertex2f(-size, size);   
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 10.0f);
    glVertex2f(-6.0f, 10.0f);
    glVertex2f(-6.0f, 14.0f);
    glVertex2f(-10.0f, 14.0f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(6.0f, 10.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(10.0f, 14.0f);
    glVertex2f(6.0f, 14.0f);
    glEnd();
    glPopMatrix();

    if (time(NULL) > timer) {
        shootGreenEnemyBullet(lettuceX, lettuceY, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateGreenEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawGreenEnemyBullet(enemyGreenProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, tomatoX, tomatoY, collisionLettuceThreshold)) {
            lettuceHealth -= 1;
            playerScore += 10;
            printf("Lettuce health: %d\n", lettuceHealth);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break;
        }
    }
}
void drawLettuce1(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (lettuceHealth1 <= 0) {
        enemies[15].active = false;
        return;
    }

    glPushMatrix();
    //lettuceX = playerX + 150.0f; 
    //lettuceY = playerY + 150.0f;
    enemies[15].y = lettuceY1;
    enemies[15].x = lettuceX1;
    enemies[15].size = 40.0f;
    float speed = 1.5f;

    float dx = playerX - lettuceX1;
    float dy = playerY - lettuceY1;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        lettuceX1 += dx * speed;
        lettuceY1 += dy * speed;
    } else {
        lettuceX1 = playerX;
        lettuceY1 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(lettuceX1, lettuceY1, 0.0f);

    //draw square lettuce
    glColor3f(0.5f, 1.0f, 0.5f);  
    float size = 20.0f;  
    glBegin(GL_QUADS);
    glVertex2f(-size, -size);  
    glVertex2f(size, -size);   
    glVertex2f(size, size);    
    glVertex2f(-size, size);   
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 10.0f);
    glVertex2f(-6.0f, 10.0f);
    glVertex2f(-6.0f, 14.0f);
    glVertex2f(-10.0f, 14.0f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(6.0f, 10.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(10.0f, 14.0f);
    glVertex2f(6.0f, 14.0f);
    glEnd();
    glPopMatrix();

    if (time(NULL) > timer) {
        shootGreenEnemyBullet(lettuceX1, lettuceY1, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateGreenEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawGreenEnemyBullet(enemyGreenProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, lettuceX1, lettuceY1, collisionLettuceThreshold)) {
            lettuceHealth1 -= 1;
            playerScore += 10;
            printf("Lettuce health: %d\n", lettuceHealth1);
            printf("Player Score: %d\n", playerScore);

            removeBullet(i);
            break;
        }
    }
}
void drawLettuce2(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (lettuceHealth2 <= 0) {
        enemies[16].active = false;
        return;
    }

    glPushMatrix();
    //lettuceX = playerX + 150.0f; 
    //lettuceY = playerY + 150.0f;
    enemies[16].y = lettuceY2;
    enemies[16].x = lettuceX2;
    enemies[16].size = 40.0f;
    float speed = 1.5f;

    float dx = playerX - lettuceX2;
    float dy = playerY - lettuceY2;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        lettuceX2 += dx * speed;
        lettuceY2 += dy * speed;
    } else {
        lettuceX2 = playerX;
        lettuceY2 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(lettuceX2, lettuceY2, 0.0f);

    //draw square lettuce
    glColor3f(0.5f, 1.0f, 0.5f);  
    float size = 20.0f;  
    glBegin(GL_QUADS);
    glVertex2f(-size, -size);  
    glVertex2f(size, -size);   
    glVertex2f(size, size);    
    glVertex2f(-size, size);   
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 10.0f);
    glVertex2f(-6.0f, 10.0f);
    glVertex2f(-6.0f, 14.0f);
    glVertex2f(-10.0f, 14.0f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(6.0f, 10.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(10.0f, 14.0f);
    glVertex2f(6.0f, 14.0f);
    glEnd();
    glPopMatrix();

    if (time(NULL) > timer) {
        shootGreenEnemyBullet(lettuceX2, lettuceY2, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateGreenEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawGreenEnemyBullet(enemyGreenProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, lettuceX2, lettuceY2, collisionLettuceThreshold)) {
            lettuceHealth2 -= 1;
            playerScore += 10;
            printf("Lettuce health: %d\n", lettuceHealth2);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
void drawLettuce3(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (lettuceHealth3 <= 0) {
        enemies[17].active = false;
        return;
    }

    glPushMatrix();
    //lettuceX = playerX + 150.0f; 
    //lettuceY = playerY + 150.0f;
    enemies[16].y = lettuceY3;
    enemies[16].x = lettuceX3;
    enemies[16].size = 40.0f;
    float speed = 1.5f;

    float dx = playerX - lettuceX3;
    float dy = playerY - lettuceY3;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        lettuceX3 += dx * speed;
        lettuceY3 += dy * speed;
    } else {
        lettuceX3 = playerX;
        lettuceY3 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(lettuceX3, lettuceY3, 0.0f);

    //draw square lettuce
    glColor3f(0.5f, 1.0f, 0.5f);  
    float size = 20.0f;  
    glBegin(GL_QUADS);
    glVertex2f(-size, -size);  
    glVertex2f(size, -size);   
    glVertex2f(size, size);    
    glVertex2f(-size, size);   
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 10.0f);
    glVertex2f(-6.0f, 10.0f);
    glVertex2f(-6.0f, 14.0f);
    glVertex2f(-10.0f, 14.0f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(6.0f, 10.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(10.0f, 14.0f);
    glVertex2f(6.0f, 14.0f);
    glEnd();
    glPopMatrix();

    if (time(NULL) > timer) {
        shootGreenEnemyBullet(lettuceX3, lettuceY3, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updateGreenEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawGreenEnemyBullet(enemyGreenProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, lettuceX3, lettuceY3, collisionLettuceThreshold)) {
            lettuceHealth3 -= 1;
            playerScore += 10;
            printf("Lettuce health: %d\n", lettuceHealth3);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
/*
*/
void drawEggplant(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (eggplantHealth <= 0) {
        enemies[3].active = false;
        return;
    }

    glPushMatrix();
    static float radius = 15.0f;
    enemies[3].x = eggplantX;
    enemies[3].y = eggplantY;
    enemies[3].size = 40.0f;
    float speed = 1.0f;

    float dx = playerX - eggplantX;
    float dy = playerY - eggplantY;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        eggplantX += dx * speed;
        eggplantY += dy * speed;
    } else {
        eggplantX = playerX;
        eggplantY = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(eggplantX, eggplantY, 0.0f);

    //draw eggplant
    glColor3f(0.5f, 0.0f, 0.5f); 
    float size = 25.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(size * cos(theta), size * sin(theta));
    }
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 2, radius);
    glVertex2f(0.0f, radius + 15.0f);
    glVertex2f(radius / 2, radius);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        shootPurpleEnemyBullet(eggplantX, eggplantY, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updatePurpleEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, eggplantX, eggplantY, collisionLettuceThreshold)) {
            eggplantHealth -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
}
void drawEggplant1(float playerX, float playerY)
{
    //if (stage == 5) {
    static int timer = time(NULL) + 2;
    if (eggplantHealth1 <= 0) {
        enemies[4].active = false;
        return;
    }

    glPushMatrix();
    static float radius = 15.0f;
    enemies[4].x = eggplantX;
    enemies[4].y = eggplantY;
    enemies[4].size = 40.0f;
    float speed = 1.0f;

    float dx = playerX - eggplantX1;
    float dy = playerY - eggplantY1;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        eggplantX1 += dx * speed;
        eggplantY1 += dy * speed;
    } else {
        eggplantX1 = playerX;
        eggplantY1 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 5;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(eggplantX1, eggplantY1, 0.0f);

    //draw eggplant
    glColor3f(0.5f, 0.0f, 0.5f); 
    float size = 25.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(size * cos(theta), size * sin(theta));
    }
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 2, radius);
    glVertex2f(0.0f, radius + 15.0f);
    glVertex2f(radius / 2, radius);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        shootPurpleEnemyBullet(eggplantX1, eggplantY1, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updatePurpleEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, eggplantX1, eggplantY1, collisionLettuceThreshold)) {
            eggplantHealth1 -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth1);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
        if (meleeCollision(eggplantX1, eggplantY1, 25.0f, brockX, 40.0f, brockY)) {
            if(eggplantHealth1 > 0) {
                eggplantHealth1 -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", eggplantHealth1);
                printf("Melee collision detected!\n");
                printf("Player Score: %d\n", playerScore);
            }
        }
    }
    // }
}
void drawEggplant2(float playerX, float playerY)
{
    //if (stage == 5) {
    static int timer = time(NULL) + 2;
    if (eggplantHealth2 <= 0) {
        enemies[17].active = false;
        return;
    }

    glPushMatrix();
    static float radius = 15.0f;
    enemies[17].x = eggplantX2;
    enemies[17].y = eggplantY2;
    enemies[17].size = 40.0f;
    float speed = 1.0f;

    float dx = playerX - eggplantX2;
    float dy = playerY - eggplantY2;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        eggplantX2 += dx * speed;
        eggplantY2 += dy * speed;
    } else {
        eggplantX2 = playerX;
        eggplantY2 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 5;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(eggplantX2, eggplantY2, 0.0f);

    //draw eggplant
    glColor3f(0.5f, 0.0f, 0.5f); 
    float size = 25.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(size * cos(theta), size * sin(theta));
    }
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 2, radius);
    glVertex2f(0.0f, radius + 15.0f);
    glVertex2f(radius / 2, radius);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        shootPurpleEnemyBullet(eggplantX2, eggplantY2, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updatePurpleEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, eggplantX2, eggplantY2, collisionLettuceThreshold)) {
            eggplantHealth1 -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth1);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
        if (meleeCollision(eggplantX2, eggplantY2, 25.0f, brockX, 40.0f, brockY)) {
            if(eggplantHealth2 > 0) {
                eggplantHealth2 -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", eggplantHealth2);
                printf("Melee collision detected!\n");
                printf("Player Score: %d\n", playerScore);
            }
        }
    }
    // }
}
void drawEggplant3(float playerX, float playerY)
{
    //if (stage == 5) {
    static int timer = time(NULL) + 2;
    if (eggplantHealth3 <= 0) {
        enemies[18].active = false;
        return;
    }

    glPushMatrix();
    static float radius = 15.0f;
    enemies[18].x = eggplantX3;
    enemies[18].y = eggplantY3;
    enemies[18].size = 40.0f;
    float speed = 1.0f;

    float dx = playerX - eggplantX3;
    float dy = playerY - eggplantY3;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        eggplantX3 += dx * speed;
        eggplantY3 += dy * speed;
    } else {
        eggplantX3 = playerX;
        eggplantY3 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 5;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(eggplantX3, eggplantY3, 0.0f);

    //draw eggplant
    glColor3f(0.5f, 0.0f, 0.5f); 
    float size = 25.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(size * cos(theta), size * sin(theta));
    }
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 2, radius);
    glVertex2f(0.0f, radius + 15.0f);
    glVertex2f(radius / 2, radius);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        shootPurpleEnemyBullet(eggplantX3, eggplantY3, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updatePurpleEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, eggplantX3, eggplantY3, collisionLettuceThreshold)) {
            eggplantHealth3 -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth3);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
        if (meleeCollision(eggplantX3, eggplantY3, 25.0f, brockX, 40.0f, brockY)) {
            if(eggplantHealth3 > 0) {
                eggplantHealth3 -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", eggplantHealth3);
                printf("Melee collision detected!\n");
                printf("Player Score: %d\n", playerScore);
            }
        }
    }
    // }
}
void drawEggplant4(float playerX, float playerY)
{
    //if (stage == 5) {
    static int timer = time(NULL) + 2;
    if (eggplantHealth4 <= 0) {
        enemies[19].active = false;
        return;
    }

    glPushMatrix();
    static float radius = 15.0f;
    enemies[19].x = eggplantX4;
    enemies[19].y = eggplantY4;
    enemies[19].size = 40.0f;
    float speed = 1.0f;

    float dx = playerX - eggplantX4;
    float dy = playerY - eggplantY4;
    float distance = sqrt(dx * dx + dy * dy);


    if (distance > speed) {
        dx /= distance;
        dy /= distance;
        eggplantX1 += dx * speed;
        eggplantY1 += dy * speed;
    } else {
        eggplantX4 = playerX;
        eggplantY4 = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 5;
            printf("Player health: %d\n", playerHealth);
        }

    }


    glTranslatef(eggplantX4, eggplantY4, 0.0f);

    //draw eggplant
    glColor3f(0.5f, 0.0f, 0.5f); 
    float size = 25.0f;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(size * cos(theta), size * sin(theta));
    }
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-radius / 2, radius);
    glVertex2f(0.0f, radius + 15.0f);
    glVertex2f(radius / 2, radius);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        shootPurpleEnemyBullet(eggplantX4, eggplantY4, playerX, playerY);
        timer = time(NULL) + 1.5;
    }
    updatePurpleEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, eggplantX4, eggplantY4, collisionLettuceThreshold)) {
            eggplantHealth4 -= 1;
            playerScore += 10;
            printf("Eggplant health: %d\n", eggplantHealth4);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
        if (meleeCollision(eggplantX4, eggplantY4, 25.0f, brockX, 40.0f, brockY)) {
            if(eggplantHealth4 > 0) {
                eggplantHealth4 -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", eggplantHealth4);
                printf("Melee collision detected!\n");
                printf("Player Score: %d\n", playerScore);
            }
        }
    }
    // }
}

void drawBoss(float playerX, float playerY)
{
    static float direction = 1.0f;
    static int timer = time(NULL) + 2;
    static int timer1 = time(NULL) + 3;
    static int timer2 = time(NULL) + 4;
    static int timer3 = time(NULL) + 5;
    static int movementTimer = time(NULL) + 5;
    static float speed = 2.0f;
    static float moveSpeed = 150.0f; 
    static float minY = 100.0f;   
    static float maxY = 300.0f;   
    if (bossHealth <= 0) {
        enemies[2].active = false;
        return;
    }
    glPushMatrix();
    //lettuceX = playerX + 150.0f; 
    //lettuceY = playerY + 150.0f;
    enemies[2].y = bossY;
    enemies[2].x = bossX;
    enemies[2].size = 80.0f;
    bossY += speed * direction;
    if (bossY >= maxY || bossY <= minY) {
        direction *= -1; 
    }
    float dx = abs(playerX - bossX);
    float dy = abs(playerY - bossY);
    if (dx <= enemies[2].size && dy <= enemies[2].size && playerHealth > 0) {
        playerHealth -= 1;
        printf("Player health: %d\n", playerHealth);
    }

    glTranslatef(bossX, bossY, 0.0f);

    glColor3f(1.0f, 0.85f, 0.4f);  
    static float bodyWidth = 60.0f;  
    static float bodyHeight = 100.0f;  

    glBegin(GL_QUADS);
    glVertex2f(-bodyWidth, -bodyHeight);
    glVertex2f(bodyWidth, -bodyHeight);
    glVertex2f(bodyWidth, bodyHeight);
    glVertex2f(-bodyWidth, bodyHeight);
    glEnd();

    // PINEAPPLE STEM
    glColor3f(0.0f, 0.8f, 0.0f); 
    static float leafWidth = 40.0f;
    static float leafHeight = 40.0f;
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, bodyHeight); 
    glVertex2f(-leafWidth, bodyHeight + leafHeight);
    glVertex2f(leafWidth, bodyHeight + leafHeight);
    glEnd();

    //more stems
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, bodyHeight + 20.0f);
    glVertex2f(-leafWidth / 1.5f, bodyHeight + leafHeight + 20.0f);
    glVertex2f(leafWidth / 1.5f, bodyHeight + leafHeight + 20.0f);
    glEnd();

    //left eye
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_QUADS);
    glVertex2f(-20.0f, 20.0f);
    glVertex2f(-10.0f, 20.0f);
    glVertex2f(-10.0f, 30.0f);
    glVertex2f(-20.0f, 30.0f);
    glEnd();

    // Right eye
    glBegin(GL_QUADS);
    glVertex2f(10.0f, 20.0f);
    glVertex2f(20.0f, 20.0f);
    glVertex2f(20.0f, 30.0f);
    glVertex2f(10.0f, 30.0f);
    glEnd();

    //mouth 
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-15.0f, 10.0f);
    glVertex2f(0.0f, 5.0f);
    glVertex2f(15.0f, 10.0f);
    glEnd();

    glPopMatrix();

    if (time(NULL) > timer) {
        const int bulletCount = 5;
        const float angleIncrement = 30.0f;
        float baseAngle = atan2(playerY - bossY, playerX - bossX) * 180.0f / 3.14159265358979323846f;
        for (int i = 0; i < bulletCount; ++i) {
            float spreadAngle = baseAngle - (angleIncrement * (bulletCount / 2)) + (i * angleIncrement);
            float radianAngle = spreadAngle * M_PI / 180.0f;
            float deltax = cos(radianAngle);
            float deltay = sin(radianAngle);
            //shootGreenEnemyBullet(bossX, bossY, bossX + deltax * 100, bossY + deltay * 100);
            shootPurpleEnemyBullet(bossX, bossY,bossX + deltax * 100, bossY + deltay * 100);
            //shootOrangeEnemyBullet(bossX, bossY,bossX + deltax * 100, bossY + deltay * 100);
            //shootEnemyBullet(bossX, bossY,bossX + deltax * 100, bossY + deltay * 100);
        }
        timer = time(NULL) + 1.5;
    }

    if (time(NULL) > movementTimer) {
        bossX += moveSpeed * ((rand() % 2 == 0) ? 1 : -1); 
        bossY += moveSpeed * ((rand() % 2 == 0) ? 1 : -1); 
        if (bossX < 0) 
            bossX = 300;
        if (bossX > 600) 
            bossX = 300;
        if (bossY < 0) 
            bossY = 200;
        if (bossY > 400) 
            bossY = 200;         
        movementTimer = time(NULL) + 2;
    } 
    // updateGreenEnemyBullets();
    updatePurpleEnemyBullets();
    // updateOrangeEnemyBullets();
    // updateEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        //   drawGreenEnemyBullet(enemyGreenProjectiles[i]);
        drawPurpleEnemyBullet(enemyPurpleProjectiles[i]);
        // drawOrangeEnemyBullet(enemyOrangeProjectiles[i]);
        // drawEnemyBullet(enemyProjectiles[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(projectiles[i].x, projectiles[i].y, bossX, bossY, 80.0f)) {
            bossHealth -= 1;
            playerScore += 10;
            printf("Boss health: %d\n", bossHealth);
            printf("Player Score: %d\n", playerScore);
            removeBullet(i);
            break;
        }
    }
    if (bossHealth < 100) {
        if (time(NULL) > timer3) {      
            const int bulletCount = 20;
            const float angleIncrement = 30.0f;
            float baseAngle = atan2(playerY - bossY, playerX - bossX) * 180.0f / 3.14159265358979323846f;
            for (int i = 0; i < bulletCount; ++i) {          
                float spreadAngle = baseAngle - (angleIncrement * (bulletCount / 2)) + (i * angleIncrement);
                float radianAngle = spreadAngle * M_PI / 180.0f;
                float deltax = cos(radianAngle);
                float deltay = sin(radianAngle);
                shootGreenEnemyBullet(bossX, bossY,bossX + deltax * 50, bossY + deltay * 50);
            }
            timer3 = time(NULL) + 5;
        }

        updateGreenEnemyBullets();

        for (int i = 0; i < bulletCount; ++i) {
            drawGreenEnemyBullet(enemyGreenProjectiles[i]);
        }
        if (time(NULL) > timer2) {      
            const int bulletCount = 10;
            const float angleIncrement = 30.0f;
            float baseAngle = atan2(playerY - bossY, playerX - bossX) * 180.0f / 3.14159265358979323846f;
            for (int i = 0; i < bulletCount; ++i) {          
                float spreadAngle = baseAngle - (angleIncrement * (bulletCount / 2)) + (i * angleIncrement);
                float radianAngle = spreadAngle * M_PI / 180.0f;
                float deltax = cos(radianAngle);
                float deltay = sin(radianAngle);
                shootOrangeEnemyBullet(bossX, bossY,bossX + deltax * 75, bossY + deltay * 75);
            }
            timer2 = time(NULL) + 4;
        }

        updateOrangeEnemyBullets();

        for (int i = 0; i < bulletCount; ++i) {
            drawOrangeEnemyBullet(enemyOrangeProjectiles[i]);
        }


    }
    if (bossHealth < 50) {
        if (time(NULL) > timer1) {      
            const int bulletCount = 15;
            const float angleIncrement = 30.0f;
            float baseAngle = atan2(playerY - bossY, playerX - bossX) * 180.0f / 3.14159265358979323846f;
            for (int i = 0; i < bulletCount; ++i) {          
                float spreadAngle = baseAngle - (angleIncrement * (bulletCount / 2)) + (i * angleIncrement);
                float radianAngle = spreadAngle * M_PI / 180.0f;
                float deltax = cos(radianAngle);
                float deltay = sin(radianAngle);
                shootGreenEnemyBullet(bossX, bossY,bossX + deltax * 100, bossY + deltay * 100);
            }
            timer1 = time(NULL) + 3;
        }

        updateEnemyBullets();

        for (int i = 0; i < bulletCount; ++i) {
            drawEnemyBullet(enemyProjectiles[i]);
        }
    }

    if (meleeCollision(bossX, bossY, 25.0f, brockX, 120.0f, brockY)) {
        if(bossHealth > 0) {
            bossHealth -= 1;
            playerScore += 10;
            printf("boss health: %d\n", bossHealth);
            printf("Melee collision detected!\n");
            printf("Player Score: %d\n", playerScore);
        }
    }
}
