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

//void Collision[unsigned char map[19][80]]
//{


//}
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
