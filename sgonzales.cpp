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
#define MAX_ENEMIES 10 
#define MAX_HEALTH 20
const float shootInterval = 0.1f;
int playerHealth = MAX_HEALTH;
int maxHealth = playerHealth;
float collisionThreshold = 10.0f;
float collisionCarrotThreshold = 35.0f;
float collisionTomatoThreshold = 35.0f;
float collisionLettuceThreshold = 50.0f;
float carrotX = 100.0f;
float carrotY = 100.0f;
float tomatoX = 0.0f;
float tomatoY = 0.0f;
float brockX = 0.0f;
float brockY = 0.0f;
float slowdown = 0.05f;
float lastShotTime = 0.0f;
extern Ball bal;
int tomatoHealth = 5;
int carrotHealth = 5;
int lettuceHealth = 5;
int eggplantHealth = 5;
int playerScore = 0;
extern int xres;
extern int yres;
struct Projectile {
    float x, y; 
    float dx, dy;  
    float speed;  
};

struct Enemy {
    float x, y;
    float size;
    int health;
    bool active; 
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
    float size = 7.0f;
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
    static int timer = time(NULL) + 2;
    if (carrotHealth <= 0) {
        enemies[0].active = false;
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 2.5f;
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
    } else {
        carrotX = playerX;
        carrotY = playerY;
        if (distance <= collisionThreshold && playerHealth > 0) {
            playerHealth -= 1;
            printf("Player health: %d\n", playerHealth);
        }
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

    if (time(NULL) > timer) {
        shootOrangeEnemyBullet(carrotX, carrotY, playerX, playerY);
        timer = time(NULL) + 2;
    }
    updateOrangeEnemyBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawOrangeEnemyBullet(enemyOrangeProjectiles[i]);
    }
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
                    bal.pos[0], bal.pos[1], 20.0f) || checkCollision(enemyPurpleProjectiles[i].x, 
                        enemyPurpleProjectiles[i].y, bal.pos[0], bal.pos[1], 20.0f)) {
            playerHealth -= 1;
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


void drawLettuce(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (lettuceHealth <= 0) {
        enemies[2].active = false;
        return;
    }

    glPushMatrix();
    static float lettuceX = playerX + 150.0f; 
    static float lettuceY = playerY + 150.0f;
    enemies[2].x = lettuceX;
    enemies[2].y = lettuceY;
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

void drawEggplant(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    if (eggplantHealth <= 0) {
        enemies[3].active = false;
        return;
    }

    glPushMatrix();
    static float eggplantX = playerX + 200.0f; 
    static float eggplantY = playerY + 200.0f;
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
void drawEggplant1(float playerX, float playerY,int stage)
{
    
    if(stage == 5) {
        static int hp = 5;
        static int timer = time(NULL) + 2;
        if (hp <= 0) {
            return;
        }

        glPushMatrix();
        static float eggplantX = playerX + 200.0f; 
        static float eggplantY = playerY + 200.0f;
        static float radius = 15.0f;
        enemies[4].x = eggplantX;
        enemies[4].y = eggplantY;
        enemies[4].size = 40.0f;
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
                hp -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", enemies[4].health);
                printf("Player Score: %d\n", playerScore);
                removeBullet(i);
                break;
            }
        }
        if (meleeCollision(eggplantX, eggplantY, 25.0f, brockX, 40.0f, brockY)) {
            if(hp > 0) {
                hp -= 1;
                playerScore += 10;
                printf("Eggplant health: %d\n", hp);
                printf("Melee collision detected!\n");
                printf("Player Score: %d\n", playerScore);
            }
        }
    }
}
