/* Name: Sebastiann Monungolh
 * Team: Lil Pea-Shooters
 * Date: 10/08/2024
 * Purpose: Individual source file for CMPS 3350 project
 */

#include "fonts.h"

int smonungolh_show = 0;

void show_my_feature(int x, int y)
{
    // Draw a rectangle
    // Show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "smonungolh's feature");
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
