//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
//
#include <iostream>
#include "sgonzales.h"


void drawCarrot(int xres, int yres) {
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    glTranslatef(xres/2, yres/2, 0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.0f;  // Increase the size here
    glVertex2f(0.0f + size, -10.0f + size);
    glVertex2f(0.0f,  10.0f - size);
    glVertex2f(50.0f,  0.0f + size);
    glEnd();
    glPopMatrix();
}

