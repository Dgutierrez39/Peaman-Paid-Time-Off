//Skyblue Gonzales
//Other information
//Date: 9/27/2024
//
//
//

#include "fonts.h"
//int sgonzales_show = 0;

//WE CREATE THE COORDINATES OF THE MOUSE
int Mx = 0;
int My = 0;
bool play1 = false;



//menu's background

class menu { 
public:
    float pos[2];
    int w;
    int h;
    background() {
        pos[0] = 0;
        pos[1] = 0;
        w = 400;
        h = 400;
} menu;

//Now the Background's rect

Rect background_rect;
background_rect.left = 0;   //X COORDINATE
background_rect.bot = 0;   //Y COORDINATE
background_rect.w = 400; //WIDTH
background_rect.h = 400; //HEIGHT



//The New Game button


//The game button's rect
Rect newGame_Rect;
newGame_Rect.x = 0;
newGame_Rect.y = 100;
newGame_Rect.w = 50;
newGame_Rect.h = 25;

//constant input the the x coordinate into Mx 
//and the y coordinate of the mouse into My

//collision state
if (Mx >= newGame_Rect.x && Mx <= newGame_Rect.x + 
        newGame_Rect.w && My >= newGame_Rect.y && My <= 
        newGame_Rect.y + newGame_Rect.h)
{

    //change color when hover over button


    //if (event->type == ) {
    //if button it is pressed then play1 becomes 
    //true which you could use to initiate the newgame
    //play1 = true;    
    //}

}
else
{
    // Tints the texture white if the cursor isn't over the button
}


   void menu screen(int x, int y)
   {
//draw a rectangle
//show some text
Rect r;
r.bot = y; 
r.left = x; 
r.center = 0;
ggprint8b(&r, 32, 0x0000ff00, "PLAY");


}

