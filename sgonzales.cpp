//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
//
#include <iostream>
#include "sgonzales.h"


void drawCarrot() {
    glColor3f(1.0f, 0.7f, 0.0f);  // orange color
    glPushMatrix();
    glTranslatef(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    float size = 0.5f;  // Increase the size here
    glVertex2f(-12.0f + size, -10.0f + size);
    glVertex2f(  0.0f,  20.0f - size);
    glVertex2f(  0.0f,  -6.0f + size);
    glVertex2f(  0.0f,  -6.0f + size);
    glVertex2f(  0.0f,  20.0f - size);
    glVertex2f( 12.0f - size, -10.0f + size);
    glEnd();
}

