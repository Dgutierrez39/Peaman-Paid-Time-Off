//David

#include <GL/glx.h>
#include <X11/Xlib.h>


void Tile_layer(unsigned char map[19][80],int row, int col, float offx,
        float offy, float tile[2])
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i<col; i++) {
        int row_counter = row-1;
        for (int j = 0; j<row; j++) {
             if (map[row_counter][i] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                glTranslatef(offx+(tile[0]*row_counter),
                        offy+(tile[1]*i), 0.0);
                glBegin(GL_QUADS);
                    glVertex2f(-offx, -offy);
                    glVertex2f(-offx,  offy);
                    glVertex2f( offx,  offy);
                    glVertex2f( offx, -offy);
                glEnd();
                glPopMatrix();
             }
             if (map[row_counter][i] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslatef(offx+(tile[0]*row_counter),
                        offy+(tile[1]*i), 0.0);
                glBegin(GL_QUADS);
                    glVertex2f(-offx, -offy);
                    glVertex2f(-offx,  offy);
                    glVertex2f( offx,  offy);
                    glVertex2f( offx, -offy);
                glEnd();
                glPopMatrix();
             }
             row_counter--;
        }
    }
}
float Player_Collision_x(unsigned char map[19][80], int row, int col,
                      float player[2], float offx, float offy, float tile[2])
{
    for (int i = 0; i<col; i++) {
        int row_count = row-1;
        for (int j = 0; j<row; j++) {
            if ((map[row_count][i] == 'w' || map[row_count][i] == 'b'  )&&
                    (player[0] + offx) > (tile[0]*row_count) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*row_count)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i)) {
                //Colssion, however the hell you spell it I'm tired
                while ((player[0] - offx) <= ((2*offx) + (tile[0]*row_count))) {
                    player[0] += 0.1;
                }
                return player[0];
            
            }
            row_count--;
        }
    }
    return player[0];
}

float Player_Collision_y(unsigned char map[19][80], int row, int col,
                      float player[2], float offx, float offy, float tile[2])
{
    for (int i = 0; i<col; i++) {
        int row_count = row-1;
        for (int j = 0; j<row; j++) {
            if ((map[row_count][i] == 'w' || map[row_count][i] == 'b'  )&&
                    (player[0] + offx) > (tile[0]*row_count) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*row_count)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i)) {
                /*Colssion, however the hell you spell it I'm tired
                if (player[1] - offy < (2*offy) + (tile[1]*i)) {
                    while (player[1] >= (2*offy) + (tile[1]*i)) {
                        player[1] -= 1; 
                    }
                }
                else {
                    while (player[1] >= tile[1]*i) {
                        player[1] += 1;
                    }
                }*/
                                
                while ((player[1] - offy) <= ((2*offy) + (tile[1]*i))) {
                    player[1] += 0.1;
                
                }
                return player[1];
            }
            row_count--;
        }
    }
    return player[1];
}


/*
void Aim(int Mx, int My, float Bx, float By, )
{
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



}*/
