//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
//
#include <iostream>
#include "sgonzales.h"

Enemy::Enemy(float x, float y) : posX(x), posY(y) {}

void Enemy::draw() {
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue color
    glBegin(GL_QUADS);
    float size = 0.5f;  // Increase the size here
    glVertex2f(-12.0f - size, -10.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f( 12.0f - size, -10.0f);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();

}

void EnemyManager::addEnemy(float x, float y) {
    enemies.push_back(Enemy(x, y));
}

void EnemyManager::drawEnemies() {
    for (auto& enemy : enemies) {
        enemy.draw();
        std::cout << "Drawing enemy at (" << enemy.getX() << ", " << enemy.getY() << ")\n"; // Example debug output
    }
}

