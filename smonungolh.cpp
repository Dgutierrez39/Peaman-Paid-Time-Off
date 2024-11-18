/* Name: Sebastiann Monungolh
 * Team: Lil Pea-Shooters
 * Date: 13-11-2024
 * Purpose: Individual source file for CMPS 3350 project
 */

#ifndef SMONUNGOLH_H
#define SMONUNGOLH_H

#include "fonts.h"
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#endif

int smonungolh_show = 0;
bool is_dead = false;

void show_my_featureSM(int x, int y)
{
    // Draw a rectangle
    // Show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "text test");
    ggprint8b(&r, 16, 0x00ff0000, "Peaman");
}

void show_if_thrust(int x, int y)
{
    // Draw a rectangle
    // Show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "thrust!");
}

void isDead(int health)
{
    if (health == 0 && !is_dead) {
        printf("Peaman is Dead! Long live Peaman!\n\n");
        is_dead = true;
    }
}

void healthBar(int xres, int yres, int health, int max_health)
{
    // printf("\ntest %f test\n", (GLfloat)health);
    // Show health bar
    glPushMatrix();
	glTranslatef(xres / 30, yres - 20, 0.0f);
	glBegin(GL_QUADS);
        glColor3f(0.663f, 0.663f, 0.663f); // Dark Gray
        glVertex2f(0, -10);
		glVertex2f(0,  10);
		glVertex2f(300,  10);
		glVertex2f(300, -10);
        glColor3f(0.0f, 1.0f, 0.0f); // Lime
	    glVertex2f(0, -10);
		glVertex2f(0,  10);
		glVertex2f(300 * ((GLfloat)health / max_health),  10);
		glVertex2f(300 * ((GLfloat)health / max_health), -10);
	glEnd();
	glPopMatrix();
    // Show health as text
    char char_health[10];
    sprintf(char_health, "%d", health);
    glEnable(GL_TEXTURE_2D);
    Rect r;
    r.bot = yres - 25;
    r.left = (xres / 30) + 8;
    r.center = 1;
    ggprint8b(&r, 32, 0xFF274E13, (char*)char_health);
    glDisable(GL_TEXTURE_2D);
}