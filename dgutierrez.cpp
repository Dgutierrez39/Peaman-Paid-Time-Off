//David

#include <GL/glx.h>
#include <X11/Xlib.h>


void Tile_layer(unsigned char map[3][31][30],int row, int col, float offx,
        float offy, float tile[2], int stage)
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
             if (map[stage][j][i] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                glTranslatef(offx+(tile[0]*j),
                        offy+(tile[1]*i), 0.0);
                glBegin(GL_QUADS);
                    glVertex2f(-offx, -offy);
                    glVertex2f(-offx,  offy);
                    glVertex2f( offx,  offy);
                    glVertex2f( offx, -offy);
                glEnd();
                glPopMatrix();
             }
             if (map[stage][j][i] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslatef(offx+(tile[0]*j),
                        offy+(tile[1]*i), 0.0);
                glBegin(GL_QUADS);
                    glVertex2f(-offx, -offy);
                    glVertex2f(-offx,  offy);
                    glVertex2f( offx,  offy);
                    glVertex2f( offx, -offy);
                glEnd();
                glPopMatrix();
             }
             if (map[stage][j][i] == 'd') {
                glColor3f(0, 0, 1);
                glPushMatrix();
                glTranslatef(offx+(tile[0]*j),
                        offy+(tile[1]*i), 0.0);
                glBegin(GL_QUADS);
                    glVertex2f(-offx, -offy);
                    glVertex2f(-offx,  offy);
                    glVertex2f( offx,  offy);
                    glVertex2f( offx, -offy);
                glEnd();
                glPopMatrix();
             }

            
        }
    }
}
float Player_Collision_x(unsigned char map[3][31][30], int row, int col,
                      float player[2], float offx, float offy, float tile[2], int way, int stage)
{
    for (int i = 0; i<col; i++) {
        //int row_count = row-1;
        for (int j = 0; j<row; j++) {
            if ((map[stage][j][i] == 'w' || map[stage][j][i] == 'b'  )&&
                    (player[0] + offx-1) > (tile[0]*j) &&
                    (player[0] - offx+1) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy+1) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy-1) > (tile[1]*i)) {
                    
                if (way == 0){
                    while((player[0] - offx) < ((2*offx) + (tile[0]*j)))
                        player[0] += 0.1;
                }
                if (way == 1){
                    while((player[0] + offx) > (tile[0]*j))
                        player[0] -= 0.1;
                }

                return player[0];
            }
        }
    }
    return player[0];
}
  



float Player_Collision_y(unsigned char map[3][31][30], int row, int col,
                      float player[2], float offx, float offy, float tile[2], int way, int stage)
{
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if ((map[stage][j][i] == 'w' || map[stage][j][i] == 'b'  )&&
                    (player[0] + offx-1) > (tile[0]*j) &&
                    (player[0] - offx+1) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy+1) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy-1) > (tile[1]*i)) {
                
                //going down
                
                if (way == 0) {
                while ((player[1] - offy) < ((2*offy) + (tile[1]*i))) {
                        player[1] += 0.1;   
                    }
                }
                //Going up
                if (way == 1){
                    while ((player[1] + offy) > (tile[1]*i))    
                        player[1] -= 0.1;
                   
                }                
                return player[1];
            }
        }
    }
    return player[1];
}

int Door_X(unsigned char map[3][31][30], int row, int col,
           float player[2], float offx, float offy, 
           float tile[2], int way, int stage, int Yres)
{ 
    int temp = stage;
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if (map[stage][j][i] == 'd' && 
                    (player[0] + offx) > (tile[0]*j) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i) &&
                    ((player[1] >= ((Yres/2) - 4*tile[1])) && (player[1] <= ((Yres/2) + 4*tile[1])))
                    ) {
                if (way == 0)
                    temp = 0;
                if (way == 1)
                    temp = 1;
            }
        }
    }
    return temp;
}

int Door_Y(unsigned char map[3][31][30], int row, int col,
           float player[2], float offx, float offy,
           float tile[2], int way, int stage, int Xres)
{
    int temp = stage;
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if (map[stage][j][i] == 'd' &&
                    (player[0] + offx) > (tile[0]*j) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i) &&
                    (player[0] >= (Xres/2) - 4*tile[0] && player[0] <= (Xres/2) + 4*tile[0])
                    ) {
                if (way == 0)
                    temp = 1;
                if (way == 1)
                    temp = 2;
            }
        }
    }
    return temp;
}

