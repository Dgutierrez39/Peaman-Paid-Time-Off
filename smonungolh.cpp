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
#include <GL/glu.h>
#include <GL/glx.h>

#endif

int smonungolh_show = 0;
bool is_dead = false;

void show_my_featureSM(int x, int y)
{
    // Draw a rectangle
    // Show some text
    Rect r;
    glEnable(GL_TEXTURE_2D);
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ff0000, "text test");
    ggprint8b(&r, 16, 0x00ff0000, "Peaman");
    glDisable(GL_TEXTURE_2D);
}

void show_if_thrust(int x, int y)
{
    // Draw a rectangle
    // Show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    glEnable(GL_TEXTURE_2D);
    ggprint8b(&r, 16, 0x00ff0000, "thrust!");
    glDisable(GL_TEXTURE_2D);
}

void isDead(int health)
{
    // Checks for death condition
    if (health == 0 && !is_dead) {
        printf("Peaman is Dead! Long live Peaman!\n\n");
        is_dead = true;
    }
}

void healthBar(int xres, int health, int max_health)
{
    // printf("\ntest %f test\n", (GLfloat)health);
    // Convert int to GLfloat for drawing bar
    GLfloat health_float = health;
    // normalize health to range [0,1]
    GLfloat health_norm = health_float / max_health;
    GLfloat health_red = 1.0f - health_norm;
    GLfloat health_green = health_norm;
    // health bar length
    GLfloat bar_length = 300;

    // Draw bar
    glPushMatrix();
	glTranslatef(xres / 30, 10, 0.0f);
	glBegin(GL_QUADS);
        // Outline of health bar - dark gray
        glColor3f(0.663f, 0.663f, 0.663f);
        glVertex2f(0, -10);
		glVertex2f(0,  10);
		glVertex2f(bar_length,  10);
		glVertex2f(bar_length, -10);
        // Health bar itself
        // goes from lime to red
        glColor3f(health_red, health_green, 0.0f);
	    glVertex2f(0, -10);
		glVertex2f(0,  10);
		glVertex2f(bar_length * health_norm,  10);
		glVertex2f(bar_length * health_norm, -10);
	glEnd();
	glPopMatrix();

    // Show health as text
    char char_health[32];
    sprintf(char_health, "HP: %d/%d", health, max_health);
    // Starts as black, turns to white
    unsigned int health_text_color;
    // unsigned int health_text_a = 0xFF;
    unsigned int health_text_r = 0xFF - (0xFF * health_norm);
    unsigned int health_text_g = 0xFF - (0xFF * health_norm);
    unsigned int health_text_b = 0xFF - (0xFF * health_norm);
    if ((health_text_r + health_text_g + health_text_b) / 3 >= 128) {
        /* If average of RGB is >= 128 */
        /* Set contrast color to black */
        health_text_color = 0xFFFFFFFF;
    } else {
        /* If <= 128, set contrast color to white */
        health_text_color = 0xFF000000;
    }

    /*
    unsigned int health_text_color = (health_text_a << 24) |
                                     (health_text_r << 16) |
                                     (health_text_g << 8)  |
                                      health_text_b;
    */

    glEnable(GL_TEXTURE_2D);
    Rect r;
    r.bot = 5;
    r.left = (xres / 30) + 8;
    r.center = 0;
    ggprint8b(&r, 32, health_text_color, (char*)char_health);
    glDisable(GL_TEXTURE_2D);
}

void displayScore(int xres, int yres, int score)
{
     // Convert int to GLfloat for drawing bar
    GLfloat score_float = score;
    GLfloat score_limit = 5000;
    // Normalize health to range [0,1]
    GLfloat score_norm = score_float / score_limit;
    if (score_float > score_limit) {
        score_norm = 1.000f;
    }

    // Set constraint for score bar
    GLfloat score_length = xres * score_norm;
    if (score_length > xres - (xres / 15)) {
        score_length = xres - (xres / 15);
    }

    // Interpolate colors from white to gold for score bar
    GLfloat score_r = 1.000f;
    GLfloat score_g = 1.000f + (0.843f - 1.000f) * score_norm;
    GLfloat score_b = 1.000f + (0.000f - 1.000f) * score_norm;


    // Draw bar
    glPushMatrix();
	glTranslatef(xres / 30, yres - 10, 0.0f);
	glBegin(GL_QUADS);
        glColor3f(score_r, score_g, score_b);
        glVertex2f(0, -10);
		glVertex2f(0,  10);
		glVertex2f(score_length,  10);
		glVertex2f(score_length, -10);
	glEnd();
	glPopMatrix();

    // Show player score as text
    char char_score[32];
    sprintf(char_score, "Score: %d", score);
    unsigned int score_color;
    score_color = 0xFFD2691E;

    glEnable(GL_TEXTURE_2D);
    Rect r;
    r.bot = yres - 15;
    r.left = (xres / 30) + 8;
    r.center = 0;
    ggprint8b(&r, 32, score_color, (char*)char_score);
    glDisable(GL_TEXTURE_2D);
}