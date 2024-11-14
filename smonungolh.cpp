/* Name: Sebastiann Monungolh
 * Team: Lil Pea-Shooters
 * Date: 13-11-2024
 * Purpose: Individual source file for CMPS 3350 project
 */

#ifndef SMONUNGOLH_H
#define SMONUNGOLH_H

#include "fonts.h"
#include <cstdio>

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

/*
void healthBar(int health)
{
    // Draw boxes here
}
*/