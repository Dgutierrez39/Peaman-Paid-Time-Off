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
const float shootInterval = 0.1f;
int health = 20;
float collisionThreshold = 10.0f;
float carrotX = 0.0f;
float carrotY = 0.0f;
float brockX = 0.0f;
float brockY = 0.0f;
float slowdown = 0.05f;
float lastShotTime = 0.0f;

struct Bullet {
    float x, y; 
    float dx, dy;  
    float speed;  
};

Bullet bullets[MAX_BULLETS];  
int bulletCount = 0;

void shootBullet(float brockX, float brockY, float playerX, float playerY) {
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


void updateBullets() {
    for (int i = 0; i < bulletCount; ++i) {
        bullets[i].x += bullets[i].dx * bullets[i].speed;  
        bullets[i].y += bullets[i].dy * bullets[i].speed;
    }
}

void drawBullet(const Bullet& bullet) {
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

void drawCarrot(float playerX, float playerY) {
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    float speed = 5.0f;
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
            health -= 1;
            printf("Player health: %d\n", health);
        }
    }
    glTranslatef(carrotX,carrotY,0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();
    glPopMatrix();
}

void SimpleTimer(int timeinterval)
  {
       int startTime, currentTime, difference;
       startTime = time(NULL);

       do
       {
            currentTime = time(NULL); 
            difference = currentTime - startTime;
       }
       while (difference < timeinterval);
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

