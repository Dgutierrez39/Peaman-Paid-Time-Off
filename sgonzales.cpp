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
#include <GL/glut.h>
#include <cstddef>
#define MAX_BULLETS 10000 
#define MAX_ENEMIES 10 
const float shootInterval = 0.1f;
int playerHealth = 20;
float collisionThreshold = 10.0f;
float collisionCarrotThreshold = 25.0f;
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
int playerScore = 0;
struct Bullet {
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

Bullet bullets[MAX_BULLETS];  
int bulletCount = 0;
Bullet enemyBullets[MAX_BULLETS];
Bullet enemyGreenBullets[MAX_BULLETS];
Bullet enemyOrangeBullets[MAX_BULLETS];
void shootEnemyBullet(float tomatoX, float tomatoY, float playerX, float playerY)
{

    if (bulletCount < MAX_BULLETS) { 
        float dx = playerX - tomatoX;
        float dy = playerY - tomatoY;
        float distance = sqrt(dx * dx + dy * dy);
        dx /= distance; 
        dy /= distance;
        Bullet newBullet;
        newBullet.x = tomatoX;  
        newBullet.y = tomatoY;
        newBullet.dx = dx;   
        newBullet.dy = dy;
        newBullet.speed = 5.0f;
        enemyBullets[bulletCount] = newBullet;
        bulletCount++;  
    }
}
/*
bool meleeColliding(float brockX, float brockY, float brockSize, float enemyX, float enemyY, float enemySize) {
    float dx = brockX - enemyX;
    float dy = brockY - enemyY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (brockSize / 2 + enemySize / 2);
}
*/
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
        Bullet newBullet;
        newBullet.x = tomatoX;
        newBullet.y = tomatoY;
        newBullet.dx = dx;
        newBullet.dy = dy;
        newBullet.speed = 5.0f;
        enemyGreenBullets[bulletCount] = newBullet;
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
        Bullet newBullet;
        newBullet.x = brockX;  
        newBullet.y = brockY;
        newBullet.dx = dx;   
        newBullet.dy = dy;
        newBullet.speed = 10.0f;

        bullets[bulletCount] = newBullet;
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
        Bullet newBullet;
        newBullet.x = tomatoX;
        newBullet.y = tomatoY;
        newBullet.dx = dx;
        newBullet.dy = dy;
        newBullet.speed = 5.0f;
        enemyOrangeBullets[bulletCount] = newBullet;
        bulletCount++;
    }
}

void updateBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        bullets[i].x += bullets[i].dx * bullets[i].speed;  
        bullets[i].y += bullets[i].dy * bullets[i].speed;
    
    }
}

void updateEnemyBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyBullets[i].x += enemyBullets[i].dx * enemyBullets[i].speed;  
        enemyBullets[i].y += enemyBullets[i].dy * enemyBullets[i].speed;
    }
}

void updateGreenEnemyBullets() 
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyGreenBullets[i].x += enemyGreenBullets[i].dx * enemyGreenBullets[i].speed;  
        enemyGreenBullets[i].y += enemyGreenBullets[i].dy * enemyGreenBullets[i].speed;
    }
}

void updateOrangeEnemyBullets()
{
    for (int i = 0; i < bulletCount; ++i) {
        enemyOrangeBullets[i].x += enemyOrangeBullets[i].dx * enemyOrangeBullets[i].speed;
        enemyOrangeBullets[i].y += enemyOrangeBullets[i].dy * enemyOrangeBullets[i].speed;
    }
}

void removeBullet(int index) {
    if (index < 0 || index >= bulletCount) {
        return; // Invalid index
    }

    // Shift all bullets after the specified index
    for (int i = index; i < bulletCount - 1; ++i) {
        bullets[i] = bullets[i + 1];
    }

    // Decrement the bullet count
    --bulletCount;
}

void drawBullet(const Bullet& bullet) 
{
    glColor3f(0.0f, 1.0f, 0.0f); 
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0.0f);  
    float size = 5.0f; 
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2); 
    glVertex2f(size / 2, -size / 2);   
    glVertex2f(size / 2, size / 2);   
    glVertex2f(-size / 2, size / 2); 
    glEnd();
    glPopMatrix();
}

void drawEnemyBullet(const Bullet& bullet) 
{
    glColor3f(1.0f, 0.0f, 0.0f); 
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0.0f);  
    float size = 5.0f; 
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2); 
    glVertex2f(size / 2, -size / 2);   
    glVertex2f(size / 2, size / 2);   
    glVertex2f(-size / 2, size / 2); 
    glEnd();
    glPopMatrix();
}

void drawGreenEnemyBullet(const Bullet& bullet)
{
    glColor3f(0.0f, 0.4f, 0.0f);
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0.0f);
    float size = 5.0f;
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
    glPopMatrix();
}

void drawOrangeEnemyBullet(const Bullet& bullet)
{
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0.0f);
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
    static int health = 5;
    if (health <= 0) {
        return;
    }
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 2.5f;
    //carrotX += (playerX - carrotX) * slowdown;
    //carrotY += (playerY - carrotY) * slowdown;
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
        if (distance <= collisionThreshold && health > 0) {
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
        drawOrangeEnemyBullet(enemyOrangeBullets[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(bullets[i].x, bullets[i].y, tomatoX, tomatoY, collisionCarrotThreshold)) {
            health -= 1;
            printf("Carrot health: %d\n", health);
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
    /*
    for (int i = 0; i < enemyCount; ++i) {
        if (enemies[i].active && meleeColliding(brockX, brockY, size, enemies[i].x, enemies[i].y, enemies[i].size)) {
            enemies[i].health -= 1; 
            if (enemies[i].health <= 0) {
                enemies[i].active = false; 
            }
        }
    }
    */
    if (time(NULL) > timer) {
        shootBullet(brockX, brockY, playerX, playerY);
        shootBullet(brockX, brockY, -playerX, -playerY);
        shootBullet(brockX, brockY, playerX, -playerY);
        shootBullet(brockX, brockY, -playerX, playerY);
        timer = time(NULL) + 2;
    }
    updateBullets();
    for (int i = 0; i < bulletCount; ++i) {
        drawBullet(bullets[i]);
    }

}

void drawTomato(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    static int health = 5;

    if (health <= 0) {
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
        drawEnemyBullet(enemyBullets[i]);
    }

    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(bullets[i].x, bullets[i].y, tomatoX, tomatoY, collisionTomatoThreshold)) {
            health -= 1; 
            printf("Tomato health: %d\n", health);

            removeBullet(i);
            break; 
        }
    }

}


void drawLettuce(float playerX, float playerY)
{
    static int timer = time(NULL) + 2;
    static int health = 5;
    if (health <= 0) {
        return;
    }

    glPushMatrix();
    static float lettuceX = playerX + 150.0f; 
    static float lettuceY = playerY + 150.0f;
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
        drawGreenEnemyBullet(enemyGreenBullets[i]);
    }
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(bullets[i].x, bullets[i].y, tomatoX, tomatoY, collisionLettuceThreshold)) {
            health -= 1;
            printf("Lettuce health: %d\n", health);

            removeBullet(i);
            break;
        }
    }
}
