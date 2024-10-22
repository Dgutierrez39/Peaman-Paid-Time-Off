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
#include <iostream>
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
//David's functions
extern void Tile_layer(unsigned char map[19][80],int row, int col, float offx,
        float offy, float tile[2]);

extern float Player_Collision_x(unsigned char map[19][80], int row, int col,
        float player[2], float offx, float offy, float tile[2], int way);

extern float Player_Collision_y(unsigned char map[19][80], int row, int col,
        float player[2], float offx, float offy, float tile[2], int way);

//sky added
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//Shane added
const int MAX_BULLETS = 11;

//macros
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

//Setup timers //added more timers - Shane
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);


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

class Global {
    public:
        int useless_Var;
        unsigned char keys[65536];
        int xres, yres;
        double delay;
        //skyadded
        GLuint MenuTexture;
        GLuint silhouetteTexture;
        GLuint PlayTexture;
        int playPress;
        int silhouette;
        int menu;
        //
        Global() {   
            xres=800;            
            yres=600;
            delay = 0.1;
            memset(keys, 0, 65536);
            menu = 1;
            silhouette = 0;
        }
} g;

class Ball {
    public:
        Vec vel;
        Vec dir;
        float angle;
        float color[3];
        float pos[2];
        float movement[2];
        double camera [2];
        Ball() {
            pos[0] = g.xres/2;
            pos[1] = g.yres/2;
            //VecZero(dir);
            angle = 0.0;
            movement[0] = g.xres / 100; 
            movement[1] = g.yres / 100;
            camera[0] = camera[1] = 0;
        }
} bal;

class Bullet {
public:
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;
public:
    Bullet() { }
};

class Game {
public:
    Ball bal;
    Bullet *barr;
    int nbullets;
    struct timespec bulletTimer;

public:
    Game() {
        barr = new Bullet[MAX_BULLETS];
        nbullets = 0;
        clock_gettime(CLOCK_REALTIME, &bulletTimer);
    }
    ~Game() {
        delete [] barr;
    }

} ga;


class Level {
    public:
        unsigned char arr[19][80];
        int nrows, ncols;
        float tilesize[2];
        float ftsz[2];
        //Flt tile_base;
        int spawn;
        float tx,ty;
        Level() {
            //Log("Level constructor\n");
            spawn = 0;
            tilesize[0] =  g.xres / 30;
            tilesize[1] =  g.yres / 30;
            ftsz[0] = (double)tilesize[0];
            ftsz[1] = (double)tilesize[1];
            //tile_base = 220.0;
            tx = tilesize[0]/2;
            ty = tilesize[1]/2;
            //read level
            FILE *fpi = fopen("level2.txt","r");
            if (fpi) {
                nrows=0;
                char line[100];
                while (fgets(line, 100, fpi) != NULL) {
                    removeCrLf(line);
                    int slen = strlen(line);
                    ncols = slen;
                    //Log("line: %s\n", line);
                    for (int j=0; j<slen; j++) {
                        arr[nrows][j] = line[j];
                    }
                    ++nrows;
                }
                fclose(fpi);
                //printf("nrows of background data: %i\n", nrows);
            }
            for (int i=0; i<nrows; i++) {
                for (int j=0; j<ncols; j++) {
                    printf("%c", arr[i][j]);
                }
                printf("\n");
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
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&ga.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&ga.bulletTimer, &bt);
                //shoot a bullet...
                if (ga.nbullets < MAX_BULLETS) {
                    Bullet *b = &ga.barr[ga.nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = ga.bal.pos[0];
                    b->pos[1] = ga.bal.pos[1];
                    b->vel[0] = ga.bal.vel[0];
                    b->vel[1] = ga.bal.vel[1];
                    //convert ship angle to radians
                    Flt rad = ((ga.bal.angle+90.0) / 360.0f) * PI * 2.0;
                    //convert angle to a vector
                    Flt xdir = cos(rad);
                    Flt ydir = sin(rad);
                    b->pos[0] += xdir*20.0f;
                    b->pos[1] += ydir*20.0f;
                    b->vel[0] += xdir*6.0f + rnd()*0.1;
                    b->vel[1] += ydir*6.0f + rnd()*0.1;
                    b->color[0] = 1.0f;
                    b->color[1] = 1.0f;
                    b->color[2] = 1.0f;
                    ++ga.nbullets;
        
           // return;
                }
            }
        }
        if (e->xbutton.button==3) {
            //Right button was pressed.
            return;
        }
    }
    if (e->type == MotionNotify) {
        //The mouse moved!
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            savex = e->xbutton.x;
            savey = e->xbutton.y;
            //Code placed here will execute whenever the mouse moves.

        }
    }
}

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
        case XK_p:
            g.menu = 0;
            break;
        case XK_Escape:
            //Escape key was pressed
            return 1;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            break;
        case XK_Down:
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
   Gordon will demonstrate this process now.

   • git clone                  # Get your group project.
   • branch [branch name]       # Start a new branch.
   • checkout                   # Go to your branch.
   • modify some code           # Write some program code. Test it.
   • git add                    # Stage the source file(s).
   • git commit                 # Commit the changes.
   • git checkout master        # Go to the master branch.
   • git pull                   # See if any other changes while you worked. Resolve the conflicts.
   • git merge [branch name]    # Merge your branch into the project!
   • git push origin master     # Add your changes to the project.


   Now we will get a volunteer group member to show us this
   process using Github.
   
   Can I get a volunteer?

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


    int b = (int)(bal.pos[1]/lev.ftsz[1]);
    b = b % lev.ncols;
    int a = (int)(bal.pos[0]/lev.ftsz[0]);
    a = a % lev.nrows;

    if (g.keys[XK_w]) {
        printf("row is: %i\n", a);
        printf("Column is: %i\n", b);
        printf("The slot has a : '%c'\n", lev.arr[a][b]);}

    if (g.keys[XK_Left]){
        bal.pos[0] -= bal.movement[0];
        bal.pos[0] = Player_Collision_x(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1);

    }
    if (g.keys[XK_Right]){
        bal.pos[0] += bal.movement[0];
        bal.pos[0] = Player_Collision_x(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,0);
    
    }
    if (g.keys[XK_Up]) {            
        bal.pos[1] += bal.movement[1];
        bal.pos[1] = Player_Collision_y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,1);
    }
    if (g.keys[XK_Down]){
        bal.pos[1] -= bal.movement[1];
        bal.pos[1] = Player_Collision_y(lev.arr, lev.nrows, lev.ncols,
                                    bal.pos, lev.tx, lev.ty, lev.tilesize,0);   
    }


/*
    for (int i = 0; i<ncols_to_render; i++) {
        int row = lev.nrows-1;
        for (int j = 0; j<lev.nrows; j++) {
            if ((lev.arr[row][i] == 'w' || lev.arr[row][i] == 'b'  )&&
                    (bal.pos[1] - lev.tx) < ((2*lev.ty) + (lev.tilesize[1]*i))  && 
                    (bal.pos[1] + lev.tx) > (lev.tilesize[1]*i)  &&
                    (bal.pos[0] + lev.tx) > (lev.tilesize[0]*row) &&
                    (bal.pos[0] - lev.tx) < ((2*lev.tx)+(lev.tilesize[0]*row))) {
                //Colssion, however the hell you spell it I'm tired
                bal.pos[0] = bal.old_pos[0];   
                bal.pos[1] = bal.old_pos[1];
            }
             if ((lev.arr[row][i] == 'b') &&
               (bal.pos[1]) < (lev.tilesize[1] + (lev.tilesize[1]*i))  &&
               (bal.pos[1]) > (lev.tilesize[1]*i)  &&
               (bal.pos[0]) > (lev.tilesize[0]*row) &&
               (bal.pos[0]) < (lev.tilesize[0] + (lev.tilesize[0]*row))) {
            //Colssion, however the hell you spell it I'm tired
            bal.pos[0] = bal.old_pos[0];
            bal.pos[1] = bal.old_pos[1];
            }
            row--;
        }
    }
*/
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < ga.nbullets) {
        Bullet *b = &ga.barr[i];
        //How long has bullet been alive?
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
        //Check for collision with window edges
        if (b->pos[0] < 0.0) {
            b->pos[0] += (float)g.xres;
        }
        else if (b->pos[0] > (float)g.xres) {
            b->pos[0] -= (float)g.xres;
        }
        else if (b->pos[1] < 0.0) {
            b->pos[1] += (float)g.yres;
        }
        else if (b->pos[1] > (float)g.yres) {
            b->pos[1] -= (float)g.yres;
        }
        ++i;
    }




}

void render()
{
    //Rect r;
    //int ncols_to_render = (bal.pos[0] * 2)  / lev.tilesize[0] + 2;
    //int nrow_to_render = (bal.pos[1] * 2) / lev.tilesize[1] + 2;
   // glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //draw a quad with texture
    //float wid = 120.0f;
///*<<<<<<<*/ HEAD
    glColor3f(1.0, 1.0, 1.0);
/*
=======
   // glColor3f(1.0, 1.0, 1.0);

    if (g.menu) {
        glBindTexture(GL_TEXTURE_2D, g.MenuTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
        glEnd();
    } 
    */
    Rect r;
    if (g.menu) {
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
    else {
        glDisable(GL_TEXTURE_2D);
        extern void drawCarrot();
        glPopMatrix();
        Tile_layer(lev.arr, lev.nrows, lev.ncols, lev.tx, lev.ty, lev.tilesize);
    }
    
    
    /*for (int i = 0; i<lev.ncols; i++) {
        int row = lev.nrows-1;
        for (int j = 0; j<lev.nrows; j++) {
            if (lev.arr[row][i] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                glTranslatef(lev.tx+(lev.tilesize[0]*row),
                        lev.ty+(lev.tilesize[1]*i), 0.0);
                glBegin(GL_QUADS);
                glVertex2f(-lev.tx, -lev.ty);
                glVertex2f(-lev.tx,  lev.ty);
                glVertex2f( lev.tx,  lev.ty);
                glVertex2f( lev.tx, -lev.ty);
                glEnd();
                glPopMatrix();
            }

            if (lev.arr[row][i] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslatef(lev.tx+(lev.tilesize[0]*row),
                        lev.ty+(lev.tilesize[1]*i), 0.0);
                glBegin(GL_QUADS);
                glVertex2f(-lev.tx, -lev.ty);
                glVertex2f(-lev.tx,  lev.ty);
                glVertex2f( lev.tx,  lev.ty);
                glVertex2f( lev.tx, -lev.ty);
                glEnd();
                glPopMatrix();
            }
            row--;
        }
    }
*/
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
     for (int i=0; i<ga.nbullets; i++) {
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
     }

}





