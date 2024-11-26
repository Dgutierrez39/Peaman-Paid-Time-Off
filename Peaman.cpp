//
//modified by: David, Skyblue
//date:
//
//original author: Gordon Griesel
//date:            Fall 2024
//purpose:         OpenGL sample program
//
//This program needs some refactoring.
//We will do this in class together.
//
//
//

/* LAB 9 INSERTION TEST FOR GIT */

//
//
#include <iostream>
#include <chrono>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//some structures
//Added code 
//#include <time.h>
#include "fonts.h"
#include "log.h"
#include "sgonzales.h"
#include "game.h"
#include <vector>
#include <string>
//David's functions
extern void Tile_layer(unsigned char map[16][31][30],int row, int col, float offx,
        float offy, float tile[2], int stage);

extern float Player_Collision_x(unsigned char map[16][31][30], int row, int col,
        float player[2], float offx, float offy, float tile[2], int way, int stage);

extern float Player_Collision_y(unsigned char map[16][31][30], int row, int col,
        float player[2], float offx, float offy, float tile[2], int way, int stage);

extern int Door_X(unsigned char map[16][31][30], int row, int col,
           float player[2], float offx, float offy,
           float tile[2], int way, int stage);
extern int Door_Y(unsigned char map[16][31][30], int row, int col,
           float player[2], float offx, float offy,
           float tile[2], int way, int stage);

extern bool checkCollision(float,float,float,float,float);
const char stages[][25] = {"./levels/r1.txt","./levels/h1.txt",
    "./levels/r2.txt", "./levels/r3.txt", "./levels/h2.txt",
    "./levels/h3.txt", "./levels/r4.txt", "./levels/h4.txt",
    "./levels/r5.txt", "./levels/r6.txt", "./levels/r7.txt",
    "./levels/h5.txt", "./levels/r8.txt", "./levels/h6.txt",
    "./levels/key.txt", "./levels/r9.txt",};
extern void removeBullet(int);
//sky added
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//Shane added
//const int MAX_BULLETS = 11;
extern void show_my_featureSW(int, int);
void fire_bullet(void);
//void update_bullets(void);
void update_bullets(unsigned char map[16][31][30], int row, int col, float offx, float offy, float tile[2], int stage);
void display_gun_info(void);
void render_bullets(void);
extern void show_gun(int, int);
extern void CarrotCollision(Game &ga);
#define BULLET_LIFESPAN 1.0

//extern Gun* currentGun;

// Sebastiann's functions
extern int smonungolh_show;
extern void show_my_featureSM(int, int);
extern int playerHealth;
extern int bulletCount;
extern bool is_dead;
extern void healthBar(int, int, int);
extern void isDead(int);

//macros
#define MAX_HEALTH 20
#define ALPHA 1
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]



//constants
const float something = 0.0f;
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793

class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
            int ppmFlag = 0;
            char name[40];
            strcpy(name, fname);
            int slen = strlen(name);
            char ppmname[80];
            if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                ppmFlag = 1;
            if (ppmFlag) {
                strcpy(ppmname, name);
            } else {
                name[slen-4] = '\0';
                //printf("name **%s**\n", name);
                sprintf(ppmname,"%s.ppm", name);
                //printf("ppmname **%s**\n", ppmname);
                char ts[100];
                //system("convert eball.jpg eball.ppm");
                sprintf(ts, "convert %s %s", fname, ppmname);
                system(ts);
            }
            //sprintf(ts, "%s", name);
            FILE *fpi = fopen(ppmname, "r");
            if (fpi) {
                char line[200];
                fgets(line, 200, fpi);
                fgets(line, 200, fpi);
                //skip comments and blank lines
                while (line[0] == '#' || strlen(line) < 2)
                    fgets(line, 200, fpi);
                sscanf(line, "%i %i", &width, &height);
                fgets(line, 200, fpi);
                //get pixel data
                int n = width * height * 3;    
                data = new unsigned char[n];    
                for (int i=0; i<n; i++)
                    data[i] = fgetc(fpi);
                fclose(fpi);
            } else {
                printf("ERROR opening image: %s\n",ppmname);
                exit(0);
            }
            if (!ppmFlag)
                unlink(ppmname);
        }
};

Image img[2] = {
"./sprites/Play.png",
"./sprites/menuScreen.png"};

Global g;
Ball bal;
Game ga;


Global::Global() {
    xres = 800;
    yres = 600;
    delay = 0.1;
    memset(keys, 0, 65536);
    menu = 0;
    silhouette = 0;
    mx = my = 0;
}
Ball::Ball() {
    pos[0] = g.xres / 2;
    pos[1] = g.yres / 2;
    angle = 0.0;
    movement[0] = g.xres / 100;
    movement[1] = g.yres / 100;
    camera[0] = camera[1] = 0;
}

Bullet::Bullet() {
}

Game::Game() {
    barr = new Bullet[MAX_BULLETS];
    nbullets = 0;
    clock_gettime(CLOCK_REALTIME, &bulletTimer);
}

Game::~Game() {
    delete[] barr;
}

void Game::check_bullet_lifetime() {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);

    for (int i = 0; i < nbullets; i++) {
        double time_diff = (currentTime.tv_sec - barr[i].time.tv_sec) +
                           (currentTime.tv_nsec - barr[i].time.tv_nsec) / 1000000000.0;

        if (time_diff > BULLET_LIFESPAN) {

            for (int j = i; j < nbullets - 1; j++) {
                barr[j] = barr[j + 1];
            }
            nbullets--;
            i--;
        }
    }
}


Gun::Gun(string gunName, float speed, double cd, int capacity, float size, int spread, float angle)
    : name(gunName), bulletSpeed(speed), cooldown(cd), ammoCapacity(capacity), currentAmmo(capacity),
      bulletSize(size), spreadCount(spread), spreadAngle(angle) {}

class Level {
    public:
        unsigned char arr[16][31][30];
        int nrows, ncols;
        float tilesize[2];
        float ftsz[2];
        //Flt tile_base;
        int spawn;
        float tx,ty;
        int current_stage;
        Level() {
            //Log("Level constructor\n");
            spawn = 0;
            current_stage = 0;
            tilesize[0] =  g.xres / 30;
            tilesize[1] =  g.yres / 30;
            ftsz[0] = (double)tilesize[0];
            ftsz[1] = (double)tilesize[1];
            //tile_base = 220.0;
            tx = tilesize[0]/2;
            ty = tilesize[1]/2;
            //read level
            for (int c = 0; c < 15; c++){
                FILE *fpi = fopen(stages[c],"r");
                if (fpi) {
                    nrows=0;
                    char line[100];
                    while (fgets(line, 100, fpi) != NULL) {
                        removeCrLf(line);
                        int slen = strlen(line);
                        ncols = slen;
                        //Log("line: %s\n", line);
                        for (int j=0; j<slen; j++) {
                            arr[c][nrows][j] = line[j];
                        }
                        ++nrows;
                    }
                    fclose(fpi);
                    //printf("nrows of background data: %i\n", nrows);
                }
            
            for (int i=0; i<nrows; i++) {
                for (int j=0; j<ncols; j++) {
                    printf("%c", arr[c][i][j]);
                }
                printf("\n");
            }
            }
        }
        void removeCrLf(char *str) {
            //remove carriage return and linefeed from a Cstring
            char *p = str;
            while (*p) {
                if (*p == 10 || *p == 13) {
                    *p = '\0';
                    break;
                }
                ++p;
            }
        }
} lev;


int n= 0;

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        ~X11_wrapper();
        X11_wrapper();
        void set_title();
        bool getXPending();
        XEvent getXNextEvent();
        void swapBuffers();
        void reshape_window(int width, int height);
        void check_resize(XEvent *e);
        void check_mouse(XEvent *e);
        int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);


int main()
{
    init_opengl();
    int done = 0;

  //clock_gettime(CLOCK_REALTIME, &timePause);
  //clock_gettime(CLOCK_REALTIME, &timeStart);
    //main game loop
    
    while (!done) {
        //look for external events such as keyboard, mouse.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            x11.check_mouse(&e);
            done = x11.check_keys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
        usleep(200);
    }
    return 0;
}


X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}


X11_wrapper::X11_wrapper()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w = g.xres, h = g.yres;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        cout << "\n\tcannot connect to X server\n" << endl;
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        cout << "\n\tno appropriate visual found\n" << endl;
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask =
        ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
            InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "3350 Lab-1");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //Window has been resized.
    g.xres = width;
    g.yres = height;
    bal.pos[0] = width/2;
    bal.pos[1] = height/2;
    lev.tilesize[0] =  width / 30;
    lev.tilesize[1] =  height / 30;
    lev.tx = lev.tilesize[0]/2;
    lev.ty = lev.tilesize[1]/2;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != g.xres || xce.height != g.yres) {
        //Window size did change.
        reshape_window(xce.width, xce.height);
    }
}
//-----------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    
    int mx = e->xbutton.x;
    int my = e->xbutton.y;

    g.mx = mx;
    g.my = my;


    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
            e->type != ButtonPress &&
            e->type != MotionNotify) {
        //This is not a mouse event that we care about.
        return;
    }
    //
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed.
            fire_bullet(g.mx, g.my); 
           // return;
                }
            }
        
        if (e->xbutton.button==3) {
            //Right button was pressed.
            return;
        }
    
    if (e->type == MotionNotify) {
        //The mouse moved!
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            savex = e->xbutton.x;
            savey = e->xbutton.y;
            //Code placed here will execute whenever the mouse moves.

        }
        if (savex != mx || savey != my) {
            savex = mx;
            savey = my;
            g.mx = mx;  //Update mouse X
            g.my = my;  //Update mouse Y 
        }

    }
}

extern int shane_show;

int X11_wrapper::check_keys(XEvent *e)
{
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    g.keys[key]=1;

    if (e->type == KeyRelease) {
        g.keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    g.keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }
    (void)shift;

    switch (key) {
        case XK_a:
            //the 'a' key was pressed
            break;
        case XK_w:
            break;
        case XK_l:
            shane_show = !shane_show;
            smonungolh_show = !smonungolh_show;
            break;
        case XK_p:
            if (g.menu == 0) {
                // Menu to game loop
                g.menu = 1;
            }
            break;
        case XK_Escape:
            //Escape key was pressed
            return 1;
        case XK_d:
            break;
        case XK_s:
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
        case XK_1:
            currentGunIndex = 0;
            break;
        case XK_2:
            currentGunIndex = 1;
            break;
        case XK_r:
            reload();
            break;
    }
    return 0;
}

unsigned char *buildAlphaData(Image *img)
{
    //Add 4th component to an RGB stream...
    //RGBA
    //When you do this, OpenGL is able to use the A component to determine
    //transparency information.
    //It is used in this application to erase parts of a texture-map from view.
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //-----------------------------------------------
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //      (int)*(ptr+0) +
        //      (int)*(ptr+1) +
        //      (int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //-----------------------------------------------
        //this code optimizes the commented code above.
        //code contributed by student: Chris Smith
        //
        *(ptr+3) = (a|b|c);
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //create opengl texture elements
    glGenTextures(1, &g.PlayTexture);
    glGenTextures(1, &g.MenuTexture);
    //-------------------------------------------------------------------------
    //Play button
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    glBindTexture(GL_TEXTURE_2D, g.PlayTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
//-------------------------------------------------------------------------
    //
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silhouetteData = buildAlphaData(&img[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                                GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    free(silhouetteData);
  //-------------------------------------------------------------------------
    //
    //Menu Screen
    glBindTexture(GL_TEXTURE_2D, g.MenuTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img[1].width, img[1].height,
                                    0, GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
    //-------------------------------------------------------------------------
    //must build a new set of data...
   // w = img[2].width;
   // h = img[2].height;
  //  unsigned char *ftData = buildAlphaData(&img[2]);
  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
  //                                          GL_RGBA, GL_UNSIGNED_BYTE, ftData);
  //  free(ftData);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
    //GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
    //-------------------------------------------------------------------------


}
void physics()
{

    int temp_stage = lev.current_stage;
    int b = (int)(bal.pos[1]/lev.ftsz[1]);
    b = b % lev.ncols;
    int a = (int)(bal.pos[0]/lev.ftsz[0]);
    a = a % lev.nrows;

   /* if (g.keys[XK_w]) {
        printf("row is: %i\n", a);
        printf("Column is: %i\n", b);
        printf("The stage number is: %i\n", lev.current_stage);
        printf("The slot has a : '%c'\n", lev.arr[lev.current_stage][a][b]);}
*/
    if (g.keys[XK_a]){
        bal.pos[0] -= bal.movement[0];
        if ((bal.pos[1] >= ((g.yres/2) - 5*lev.tilesize[1])) && (bal.pos[1] <= ((g.yres/2) + 5*lev.tilesize[1])))
            lev.current_stage = Door_X(lev.arr, lev.nrows, lev.ncols,
                            bal.pos, lev.tx, lev.ty, lev.tilesize,0, lev.current_stage);
        
        if (temp_stage != lev.current_stage)
            bal.pos[0] = g.xres-(2*lev.tilesize[0]);
        else
            bal.pos[0] = Player_Collision_x(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,0,  lev.current_stage);
    }
    if (g.keys[XK_d]){
        bal.pos[0] += bal.movement[0];
        if ((bal.pos[1] >= ((g.yres/2) - 5*lev.tilesize[1])) && (bal.pos[1] <= ((g.yres/2) + 5*lev.tilesize[1])))
            lev.current_stage = Door_X(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1, lev.current_stage);
        if (temp_stage != lev.current_stage)
            bal.pos[0] = 2*lev.tilesize[0];
        else
            bal.pos[0] = Player_Collision_x(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1, lev.current_stage);
    }

    if (g.keys[XK_w]) {            
        bal.pos[1] += bal.movement[1];
        if ((bal.pos[0] >= ((g.xres/2) - 2*lev.tilesize[0])) && (bal.pos[0] <= ((g.xres/2) + 2*lev.tilesize[0])))     
            lev.current_stage = Door_Y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1, lev.current_stage);
        if (temp_stage != lev.current_stage)
            bal.pos[1] = 2*lev.tilesize[1];
        else
            bal.pos[1] = Player_Collision_y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1, lev.current_stage);
    }
    if (g.keys[XK_s]){
        bal.pos[1] -= bal.movement[1];
        if ((bal.pos[0] >= ((g.xres/2) - 2*lev.tilesize[0])) && (bal.pos[0] <= ((g.xres/2) + 2*lev.tilesize[0])))
            lev.current_stage = Door_Y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,0, lev.current_stage);
        if (temp_stage != lev.current_stage)
            bal.pos[1] = g.yres-(2*lev.tilesize[1]);
        else
            bal.pos[1] = Player_Collision_y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,0, lev.current_stage);   
    }

/*
    for (int i = 0; i < bulletCount; ++i) {
        if (checkCollision(bullets[i].x, bullets[i].y, bal.pos[0], bal.pos[1], 40.0f)) {
            playerHealth -= 1;  
            printf("Player health: %d\n", playerHealth);

            removeBullet(i);
            break; 
        }
    }
 */   /*
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];*/
        //How long has bullet been alive?
        /*
        double ts = timeDiff(&b->time, &bt);
        if (ts > 2.5) {
            //time to delete the bullet.
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;
            //do not increment i.
            continue;
        }
        //move the bullet
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
        */
        //Check for collision with window edges
        /*
        if (b->pos[0] < 0.0) {
           // b->pos[0] += (float)g.xres;
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[0] > (float)g.xres) {
            //b->pos[0] -= (float)g.xres;
            memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] < 0.0) {
           // b->pos[1] += (float)g.yres;
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        else if (b->pos[1] > (float)g.yres) {
           // b->pos[1] -= (float)g.yres;
           memcpy(&ga.barr[i], &ga.barr[ga.nbullets-1],
                sizeof(Bullet));
            ga.nbullets--;

        }
        ++i;
    }*/
   // update_bullets();
   update_bullets(lev.arr, lev.nrows, lev.ncols, lev.tx, lev.ty, lev.tilesize, lev.current_stage);
    update_reload();
//    bullet_collision(lev.arr, lev.nrows, lev.ncols, ga.barr, ga.nbullets, lev);
}

/*
extern struct timespec lastShotTime;
const float SHOOT_INTERVAL = 0.5;
float timediff(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}
*/



void render()
{
    /*
   static struct timespec lastResetTime;
   struct timespec currentTimeSpec;
   clock_gettime(CLOCK_REALTIME, &currentTimeSpec);
   float currentTime = currentTimeSpec.tv_sec + currentTimeSpec.tv_nsec / 1e9;
   //std::cout << "Current Time: " << currentTime << std::endl; 
   float elapsedTime = timediff(&lastShotTime, &currentTimeSpec);
   //std::cout << "Elapsed Time: " << elapsedTime << " seconds" << std::endl;
   
   if (currentTime - lastResetTime.tv_sec >= 5.0f) {
        currentTime = 0.0f;  // Reset currentTime to 0
        lastResetTime = currentTimeSpec;  // Update lastResetTime to current time
    }
    */
   //float elapsedTime = getElapsedTime(lastShotTime, currentTimeSpec);
  // std::cout << "Elapsed Time: " << elapsedTime << std::endl; 
    glClear(GL_COLOR_BUFFER_BIT);
    //draw a quad with texture
    glColor3f(1.0, 1.0, 1.0);
    Rect r;
    if (g.menu == 0) {
        // On the menu screen
        glBindTexture(GL_TEXTURE_2D, g.MenuTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.12f); glVertex2i(0, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
        glEnd();
        r.bot = 25;
        r.left = 75;
        r.center = 0;
        ggprint8b(&r, 32, 0xFF87CEEB, "Press p to play");
    } 
    else if (g.menu == 1) {
        // Game loop
        glDisable(GL_TEXTURE_2D);
        extern void drawCarrot(float,float);
        extern void drawBrock(float,float);
        extern void drawTomato(float, float);
        extern void drawLettuce(float, float);
        extern void drawEggplant(float, float);
        Tile_layer(lev.arr, lev.nrows, lev.ncols, lev.tx, lev.ty, lev.tilesize, lev.current_stage);
        drawCarrot(bal.pos[0], bal.pos[1]);
        drawTomato(bal.pos[0], bal.pos[1]);
        drawLettuce(bal.pos[0], bal.pos[1]);
        drawEggplant(bal.pos[0], bal.pos[1]);
        drawBrock(bal.pos[0], bal.pos[1]);
        if (shane_show == 1)
            show_my_featureSW(10, g.yres - 80);
        if (smonungolh_show == 1)
            show_my_featureSM(35, g.yres - 80);

        //BALL
        glColor3f(1.0, 1.0, 0.1);
        glPushMatrix();
        //put ball in its place
        glTranslated(bal.pos[0], bal.pos[1], 0);
        glBegin(GL_QUADS);
        glVertex2i(-lev.tx, -lev.ty);
        glVertex2i(-lev.tx,  lev.ty);
        glVertex2i( lev.tx,  lev.ty);
        glVertex2i( lev.tx, -lev.ty);
        glEnd();
        glPopMatrix();

        //Draw the bullets
       /* for (int i=0; i<ga.nbullets; i++) {
            Bullet *b = &ga.barr[i];
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
        }*/

        render_bullets();
            
        display_gun_info();

        CarrotCollision(ga);

        // Draw health bar
        healthBar(g.xres, playerHealth, MAX_HEALTH);
        
        // Checks for death condition, game over if true
        isDead(playerHealth);
        if (is_dead == true) {
            g.menu = 2;
        }  
    } else {
        // GAME OVER screen
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g.MenuTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.12f); glVertex2i(0, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
        glEnd();
        r.bot = 35;
        r.left = 75;
        r.center = 0;
        ggprint8b(&r, 32, 0xFFF44336, "GAME OVER");
        r.bot = 20;
        r.left = 75;
        ggprint8b(&r, 32, 0xFFF44336, "Press Esc to close");
    }       
    show_gun(50, g.yres - 100);
}





