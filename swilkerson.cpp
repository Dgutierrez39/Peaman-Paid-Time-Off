//Shane Wilkerson

//DISCLAIMER: Code is from asteroids. Trying to make it work with this framework before adding our own types of bullets with different time speeds and widths

//Spend a lot of the time working to git hub working

#include "fonts.h"
int shane_show = 0;
const int MAX_BULLETS = 30;
/* Can we include classes in separate function?
class Game {
    public: 
        Bullet *barr;
        int nbullets;
        struct timespec bulletTimer;
    public: 
        Game() {
            barr = new Bullet[MAX_BULLETS];
            nbullets = 0;
        }
        ~Game() {
            delete [] barr;
        }

class Bullet {
public:
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;
public:
    Bullet() { }
};

*/
//Update bullet positions



//Draw bullets -- will go in render function
void drawbullets()
{
    for (int i=0; i<g.nbullets; i++) {
        Bullet *b = &g.barr[i];
        //Log("draw bullet...\n");
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0],      b->pos[1]);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]);
        glVertex2f(b->pos[0],      b->pos[1]-1.0f);
        glVertex2f(b->pos[0],      b->pos[1]+1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
    }

}

void show_my_feature(int x, int y)
{
    //draw a rectangle 
    //show some text
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Shane");


}

