//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
#include <iostream>
#include "sgonzales.h"
#include <cmath>
int health = 4;
float collisionThreshold = 10.0f;
float carrotX = 0.0f;
float carrotY = 0.0f;
float brockX = 0.0f;
float brockY = 0.0f;
float slowdown = 0.05f;
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

void drawBrock(float playerX, float playerY) {
    glColor3f(0.0f, 1.0f, 0.0f);  //green color
    glPushMatrix();
    float radius = 50.0f;
    float size = 25.0f;
    float speed = 5.0f;
    float angularSpeed = 0.05f;
    static float angle = 0.0f;
    angle += angularSpeed;
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
    glVertex2f(-size / 2, -size / 2); // Bottom-left corner
    glVertex2f(size / 2, -size / 2);  // Bottom-right corner
    glVertex2f(size / 2, size / 2);   // Top-right corner
    glVertex2f(-size / 2, size / 2);  // Top-left corner
    glEnd();
    glPopMatrix();
}

