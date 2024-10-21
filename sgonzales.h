// sgonzales.h
#ifndef SGONZALES_H
#define SGONZALES_H

#include <vector>
#include <GL/gl.h>   
#include <GL/glu.h>
#include <iostream>

class Enemy {
public:
    Enemy(float x, float y);
    void draw();
    float getX() const { return posX; } 
    float getY() const { return posY; }
private:
    float posX, posY;
};

class EnemyManager {
public: 
    void addEnemy(float x, float y);
    void drawEnemies();
private:
    std::vector<Enemy> enemies;
};

#endif 

