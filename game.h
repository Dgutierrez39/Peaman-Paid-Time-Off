#ifndef GAME_H
#define GAME_H

//Forward declarations of classes
class Global;
class Ball;
class Bullet;
class Game;
class Level;
class X11_wrapper;

//Declare the external variables that are defined in Peaman.cpp
extern Global g;       
extern Ball bal;        
extern Game ga;         
extern Level lev;       
extern X11_wrapper x11; 

//extern Gun* currentGun;

#endif // GAME_H
