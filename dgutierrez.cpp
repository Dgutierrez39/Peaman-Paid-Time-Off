//David

#include <GL/glx.h>
#include <X11/Xlib.h>


void Tile_layer(unsigned char map[16][31][30],int row, int col, float offx,
        float offy, float tile[2], int stage)
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if (map[stage][j][i] != ' ') {
                if (map[stage][j][i] == 'w') {
                    glColor3f(0.8, 0.8, 0.6);}
             else if (map[stage][j][i] == 'b') {
                glColor3f(0.9, 0.2, 0.2);}
             
             else if (map[stage][j][i] == 'd'){
                glColor3f(0, 0, 1);}
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
float Player_Collision_x(unsigned char map[16][31][30], int row, int col,
                      float player[2], float offx, float offy, float tile[2], int way, int stage)
{
    for (int i = 0; i<col; i++) {
        //int row_count = row-1;
        for (int j = 0; j<row; j++) {
            if ((map[stage][j][i] == 'w' || map[stage][j][i] == 'b')&&
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
  



float Player_Collision_y(unsigned char map[16][31][30], int row, int col,
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

int Door_X(unsigned char map[16][31][30], int row, int col,
           float player[2], float offx, float offy, 
           float tile[2], int way, int stage)
{ 
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if (map[stage][j][i] == 'd' && 
                    (player[0] + offx) > (tile[0]*j) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i)
                    ) {
                //left
                if (way == 0) {
                    if (stage == 1)
                        return 0;
                    if (stage == 2)
                        return 1;
                    if (stage == 6)
                        return 5;
                    if (stage == 7)
                        return 6;
                    if (stage == 8)
                        return 7;
                    if (stage == 12)
                        return 11;
                    if (stage == 13)
                        return 12;
                    //BOSS if (stage == #)
                    //    return #;
                }
                //right
                if (way == 1) {
                    if (stage == 0)
                        return 1;
                    if (stage == 1)
                        return 2;
                    if (stage == 5)
                        return 6;
                    if (stage == 6)
                        return 7;
                    if (stage == 7)
                        return 8;
                    if (stage == 11)
                        return 12;
                    if (stage == 12)
                        return 13;
                    //Boss if (stage ==#)
                    //  return #;
                }
            }
        }
    }
    return stage;
}

int Door_Y(unsigned char map[16][31][30], int row, int col,
           float player[2], float offx, float offy,
           float tile[2], int way, int stage)
{
    for (int i = 0; i<col; i++) {
        for (int j = 0; j<row; j++) {
            if (map[stage][j][i] == 'd' &&
                    (player[0] + offx) > (tile[0]*j) &&
                    (player[0] - offx) < ((2*offx)+(tile[0]*j)) &&
                    (player[1] - offy) < ((2*offy) + (tile[1]*i))  &&
                    (player[1] + offy) > (tile[1]*i)
                    ) {
                //down
                if (way == 0) {
                    if (stage == 1) 
                        return 3;
                    if (stage == 2)
                        return 4;
                    if (stage == 3)
                        return 5;
                    if (stage == 4)
                        return 6;
                    if (stage == 5)
                        return 9;
                    if (stage == 7)
                        return 10;
                    if (stage == 9)
                        return 11;
                    if (stage == 10)
                        return 13;
                    if (stage == 11)
                        return 14;
                    if (stage == 13)
                        return 15;
                }
                //up
                if (way == 1) {
                    if (stage == 3)
                        return 1;
                    if (stage == 4)
                        return 2;
                    if (stage == 5)
                        return 3;
                    if (stage == 6)
                        return 4;
                    if (stage == 9)
                        return 5;
                    if (stage == 10)
                        return 7;
                    if (stage == 11)
                        return 9;
                    if (stage == 13)
                        return 10;
                    if (stage == 14)
                        return 11;
                    if (stage == 15)
                        return 13;
                }
                    
            }
        }
    }
    return stage;
}

