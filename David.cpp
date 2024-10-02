/* 
 *
 *compile code: g++ $(CFLAGS) David.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -odavid
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"

typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//const float timeslice = 1.0f;
//const float gravity = -0.2f;
#define ALPHA 1

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]


void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics();
void render();

class Level {
public:
    unsigned char arr[20][80];
    int nrows, ncols;
    int tilesize[2];
    Flt ftsz[2];
    Flt tile_base;
    int spawn;
    Level() {
        //Log("Level constructor\n");
        spawn = 0;
        tilesize[0] = 32;
        tilesize[1] = 32;
        ftsz[0] = (Flt)tilesize[0];
        ftsz[1] = (Flt)tilesize[1];
        tile_base = 220.0;
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


class Global {
public:
    unsigned char keys[65536];
    int xres, yres;
    Vec ball_pos;
    //camera is centered at (0,0) lower-left of screen.
    Flt camera[2];
    ~Global() {
        logClose();
    }
    Global() {
        logOpen();
        camera[0] = camera[1] = 0.0;
        xres=800;
        yres=600;
        MakeVector(ball_pos, 400.0, 150, 0);
        memset(keys, 0, 65536);
    }
} gl;

class X11_wrapper {
private:
    Display *dpy;
    Window win;
public:
    ~X11_wrapper() {
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
    void setTitle() {
        //Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "3350 - Walk Cycle");
    }
    void setupScreenRes(const int w, const int h) {
        gl.xres = w;
        gl.yres = h;
    }
    X11_wrapper() {
        GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
        XSetWindowAttributes swa;
        setupScreenRes(gl.xres, gl.yres);
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            printf("\n\tcannot connect to X server\n\n");
            exit(EXIT_FAILURE);
        }
        Window root = DefaultRootWindow(dpy);
        XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
        if (vi == NULL) {
            printf("\n\tno appropriate visual found\n\n");
            exit(EXIT_FAILURE);
        }
        Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
            StructureNotifyMask | SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
            vi->depth, InputOutput, vi->visual,
            CWColormap | CWEventMask, &swa);
        GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
        setTitle();
    }
    void reshapeWindow(int width, int height) {
        //window has been resized.
        setupScreenRes(width, height);
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
        setTitle();
    }
    void checkResize(XEvent *e) {
        //The ConfigureNotify is sent by the
        //server if the window is resized.
        if (e->type != ConfigureNotify)
            return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != gl.xres || xce.height != gl.yres) {
            //Window size did change.
            reshapeWindow(xce.width, xce.height);
        }
    }
    bool getXPending() {
        return XPending(dpy);
    }
    XEvent getXNextEvent() {
        XEvent e;
        XNextEvent(dpy, &e);
        return e;
    }
    void swapBuffers() {
        glXSwapBuffers(dpy, win);
    }
} x11;

int main(void)
{
    init();
    int done = 0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
    }
    cleanup_fonts();
    return 0;
}

void init() {

}

void checkMouse(XEvent *e)
{
    //printf("checkMouse()...\n"); fflush(stdout);
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type != ButtonRelease && e->type != ButtonPress &&
            e->type != MotionNotify)
        return;
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
        }
        if (e->xbutton.button==3) {
            //Right button is down
        }
    }
    if (e->type == MotionNotify) {
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
        }
    }
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    gl.keys[key]=1;
    if (e->type == KeyRelease) {
        gl.keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    gl.keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }
    (void)shift;
    switch (key) {
        case XK_s:
            break;
        case XK_m:
            break;
        case XK_w:
            break;
        case XK_e:
            break;
        case XK_f:
            break;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
        case XK_equal:
            break;
        case XK_minus:
            break;
        case XK_Escape:
            return 1;
            break;
    }
    return 0;
}

void physics(void)
{
    Flt dd = lev.ftsz[0];
    int col = (int)((gl.camera[0]+gl.ball_pos[0]) / dd);
    col = col % lev.ncols;
    int row = (int)((gl.camera[1] + gl.ball_pos[1]) / dd);
    row = row % lev.nrows;
        
        if (gl.keys[XK_w]) {
            printf("row is: %i\n", row);
            printf("Column is: %i\n", col);
            printf("The slot has a : '%c'\n", lev.arr[row][col]);
        }
        if (gl.keys[XK_Left]) {
            if ((lev.arr[row][col - 1] == ' ') ||
                    (lev.arr[row][col - 1] == 'g')) {
                //gl.camera[0] -= 1;
                gl.ball_pos[0] -= 1;
            }
        } 
       if (gl.keys[XK_Right]) {
            if ((lev.arr[row][col + 1] == ' ') ||
                    (lev.arr[row][col + 1] == 'g')) {
                gl.ball_pos[0] += 1;
                //gl.camera[0] += 1;
            }
        }
    

        
    
    if (gl.keys[XK_Up]) {
            if ((lev.arr[row + 1][col] == ' ') ||
                    (lev.arr[row + 1][col] == 'g')) {
                gl.ball_pos[1] += 1;
                //gl.camera[1] += 1;

            } 
    }
    if (gl.keys[XK_Down]) {
            if ((lev.arr[row - 1][col] == ' ') ||
                    (lev.arr[row - 1][col] == 'g')) {      
                gl.ball_pos[1] -= 1;
                //gl.camera[1] -= 1;
            }        
    }
}



void render(void)
{
    Rect r;
    //Clear the screen
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //
   /* //show ground
    glBegin(GL_QUADS);
        glColor3f(0.2, 0.2, 0.2);
        glVertex2i(0,       220);
        glVertex2i(gl.xres, 220);
        glColor3f(0.4, 0.4, 0.4);
        glVertex2i(gl.xres,   0);
        glVertex2i(0,         0);
    glEnd();
    
    *///show boxes as background
    //========================
    //Render the tile system
    //========================
    float tx = lev.tilesize[0]/2;
    float ty = lev.tilesize[1]/2;
    Flt dd = lev.ftsz[0];
    Flt offy = lev.tile_base;
    int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
    int col = (int)(gl.camera[0] / dd);
    col = col % lev.ncols;
    //Partial tile offset must be determined here.
    //The leftmost tile might be partially off-screen.
    //cdd: camera position in terms of tiles.
    Flt cdd = gl.ball_pos[0] / dd;
    //flo: just the integer portion
    Flt flo = floor(cdd);
    //dec: just the decimal portion
    Flt dec = (cdd - flo);
    //offx: the offset to the left of the screen to start drawing tiles
    Flt offx = -dec * dd;
    //Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
    for (int j=0; j<ncols_to_render; j++) {
        int row = lev.nrows-1;
        for (int i=0; i<lev.nrows; i++) {
            if (lev.arr[row][col] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                //put tile in its place
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                    glVertex2f(-tx, -ty);
                    glVertex2f(-tx,  ty);
                    glVertex2f( tx,  ty);
                    glVertex2f( tx, -ty);
                glEnd();
                glPopMatrix();
            }
            if (lev.arr[row][col] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                    glVertex2f(-tx,-ty);
                    glVertex2f(-tx, ty);
                    glVertex2f( tx, ty);
                    glVertex2f( tx,-ty);
                glEnd();
                glPopMatrix();
            }
            if (lev.arr[row][col] == 'g') {
                glColor3f(0.0, 0.0, 1.0);
                glPushMatrix();
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                    glVertex2f(-tx, -ty);
                    glVertex2f(-tx,  ty);
                    glVertex2f( tx,  ty);
                    glVertex2f( tx, -ty);
                glEnd();
                glPopMatrix();
            }




            --row;
        }
        col = (col+1) % lev.ncols;
    }
    if (lev.spawn != 1){
        for (int j=0; j<lev.ncols; j++) {
            for (int i=0; i<lev.nrows; i++) {
                if (lev.arr[i][j] == 'g') {
                    gl.ball_pos[1] += j;
                    gl.ball_pos[0] += i;
                    lev.spawn = 1;
                }           
            }
        }
    }



    glColor3f(1.0, 1.0, 0.1);
    glPushMatrix();
    //put ball in its place
    glTranslated(gl.ball_pos[0], gl.ball_pos[1], 0);
    glBegin(GL_QUADS);
        glVertex2i(-10, 0);
        glVertex2i(-10, 20);
        glVertex2i( 10, 20);
        glVertex2i( 10, 0);
    glEnd();
    glPopMatrix();

    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "w  - Current row, colume, and file letter");
    ggprint8b(&r, 16, c, "down arrow  - move down");
    ggprint8b(&r, 16, c, "up arrow    - move up");
    ggprint8b(&r, 16, c, "right arrow - move right");
    ggprint8b(&r, 16, c, "left arrow  - move left");
    
}

