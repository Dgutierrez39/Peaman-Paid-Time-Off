//
//modified by: David
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
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//some structures
//Added code 
#include <time.h>
#include "fonts.h"
#include "log.h"


#define rnd() (float)rand() / (float)RAND_MAX


class Global {
public:
    unsigned char keys[65536];
	int xres, yres;
    double delay;
	Global() {   
        xres=800;
        yres=600;
        delay = 0.1;
        memset(keys, 0, 65536);
    }
} g;


class Ball {
public:
    float pos[2];
    float old_pos[2];
    float movement[2];
    Ball() {
        pos[0] = g.xres/2;
        pos[1] = g.yres/2;
        movement[0] = g.xres / 100; 
        movement[1] = g.yres / 100;
    }
} bal;

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
            return;
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
}
void physics()
{
    int ncols_to_render = g.xres / lev.tilesize[0] + 2;

    int b = (int)(bal.pos[1]/lev.ftsz[1]);
    b = b % lev.ncols;
    int a = (int)(bal.pos[0]/lev.ftsz[0]);
    a = a % lev.nrows;

    if (g.keys[XK_w]) {
            printf("row is: %i\n", a);
            printf("Column is: %i\n", b);
            printf("The slot has a : '%c'\n", lev.arr[a][b]);}

    if (g.keys[XK_Left]){
        bal.old_pos[0] = bal.pos[0];
        bal.pos[0] -= bal.movement[0];
    }
    if (g.keys[XK_Right]){
        bal.old_pos[0] = bal.pos[0];    
        bal.pos[0] += bal.movement[0];
    }
    if (g.keys[XK_Up]) {            
        bal.old_pos[1] = bal.pos[1];
        bal.pos[1] += bal.movement[1];
        
    }
    if (g.keys[XK_Down]){
        bal.old_pos[1] = bal.pos[1];
        bal.pos[1] -= bal.movement[1];
    }

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
            /* if ((lev.arr[row][i] == 'b') &&
                  (bal.pos[1]) < (lev.tilesize[1] + (lev.tilesize[1]*i))  &&
                  (bal.pos[1]) > (lev.tilesize[1]*i)  &&
                  (bal.pos[0]) > (lev.tilesize[0]*row) &&
                  (bal.pos[0]) < (lev.tilesize[0] + (lev.tilesize[0]*row))) {
                 //Colssion, however the hell you spell it I'm tired
                 bal.pos[0] = bal.old_pos[0];
                 bal.pos[1] = bal.old_pos[1];
             }*/
             row--;
        }
    }



 
}

void render()
{
    //Rect r;
    //int ncols_to_render = (bal.pos[0] * 2)  / lev.tilesize[0] + 2;
    //int nrow_to_render = (bal.pos[1] * 2) / lev.tilesize[1] + 2;
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i<lev.ncols; i++) {
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

}





