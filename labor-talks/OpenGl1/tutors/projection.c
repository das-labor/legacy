/*
    projection.c
    Nate Robins, 1997

    Tool for teaching about OpenGL projections.
    
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "glm.h"


typedef struct _cell {
    int id;
    int x, y;
    float min, max;
    float value;
    float step;
    char* info;
    char* format;
} cell;


cell lookat[9] = {
    { 1, 180, 120, -5.0, 5.0, 0.0, 0.1,
        "Specifies the X position of the eye point.", "%.2f" },
    { 2, 240, 120, -5.0, 5.0, 0.0, 0.1,
    "Specifies the Y position of the eye point.", "%.2f" },
    { 3, 300, 120, -5.0, 5.0, 2.0, 0.1,
    "Specifies the Z position of the eye point.", "%.2f" },
    { 4, 180, 160, -5.0, 5.0, 0.0, 0.1,
    "Specifies the X position of the reference point.", "%.2f" },
    { 5, 240, 160, -5.0, 5.0, 0.0, 0.1,
    "Specifies the Y position of the reference point.", "%.2f" },
    { 6, 300, 160, -5.0, 5.0, 0.0, 0.1,
    "Specifies the Z position of the reference point.", "%.2f" },
    { 7, 180, 200, -2.0, 2.0, 0.0, 0.1,
    "Specifies the X direction of the up vector.", "%.2f" },
    { 8, 240, 200, -2.0, 2.0, 1.0, 0.1,
    "Specifies the Y direction of the up vector.", "%.2f" },
    { 9, 300, 200, -2.0, 2.0, 0.0, 0.1,
    "Specifies the Z direction of the up vector.", "%.2f" },
};

cell perspective[4] = {
    { 10, 180, 80, 1.0, 179.0, 60.0, 1.0,
        "Specifies field of view angle (in degrees) in y direction.", "%.1f" },
    { 11, 240, 80, -3.0, 3.0, 1.0, 0.01,
    "Specifies field of view in x direction (width/height).", "%.2f" },
    { 12, 300, 80, 0.1, 10.0, 1.0, 0.05,
    "Specifies distance from viewer to near clipping plane.", "%.1f" },
    { 13, 360, 80, 0.1, 10.0, 10.0, 0.05,
    "Specifies distance from viewer to far clipping plane.", "%.1f" },
};

cell frustum[6] = {
    { 14, 120, 80, -10.0, 10.0, -1.0, 0.1,
        "Specifies coordinate for left vertical clipping plane.", "%.2f" },
    { 15, 180, 80, -10.0, 10.0, 1.0, 0.1,
    "Specifies coordinate for right vertical clipping plane.", "%.2f" },
    { 16, 240, 80, -10.0, 10.0, -1.0, 0.1,
    "Specifies coordinate for bottom vertical clipping plane.", "%.2f" },
    { 17, 300, 80, -10.0, 10.0, 1.0, 0.1,
    "Specifies coordinate for top vertical clipping plane.", "%.2f" },
    { 18, 360, 80, 0.1, 5.0, 1.0, 0.01,
    "Specifies distance to near clipping plane.", "%.2f" },
    { 19, 420, 80, 0.1, 5.0, 3.5, 0.01,
    "Specifies distance to far clipping plane.", "%.2f" },
};

cell ortho[6] = {
    { 14, 120, 80, -10.0, 10.0, -1.0, 0.1,
        "Specifies coordinate for left vertical clipping plane.", "%.2f" },
    { 15, 180, 80, -10.0, 10.0, 1.0, 0.1,
    "Specifies coordinate for right vertical clipping plane.", "%.2f" },
    { 16, 240, 80, -10.0, 10.0, -1.0, 0.1,
    "Specifies coordinate for bottom vertical clipping plane.", "%.2f" },
    { 17, 300, 80, -10.0, 10.0, 1.0, 0.1,
    "Specifies coordinate for top vertical clipping plane.", "%.2f" },
    { 18, 360, 80, -5.0, 5.0, 1.0, 0.01,
    "Specifies distance to near clipping plane.", "%.2f" },
    { 19, 420, 80, -5.0, 5.0, 3.5, 0.01,
    "Specifies distance to far clipping plane.", "%.2f" },
};

enum {
    PERSPECTIVE, FRUSTUM, ORTHO
} mode = PERSPECTIVE;

GLboolean world_draw = GL_TRUE;
GLMmodel* pmodel = NULL;
GLint selection = 0;

void redisplay_all(void);
GLdouble projection[16], modelview[16], inverse[16];
GLuint window, world, screen, command;
GLuint sub_width = 256, sub_height = 256;


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

void
setfont(char* name, int size)
{
    font_style = GLUT_BITMAP_HELVETICA_10;
    if (strcmp(name, "helvetica") == 0) {
        if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}

void 
drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

void
cell_draw(cell* cell)
{
    glColor3ub(0, 255, 128);
    if (selection == cell->id) {
        glColor3ub(255, 255, 0);
        drawstr(10, 240, cell->info);
        glColor3ub(255, 0, 0);
    }
    
    drawstr(cell->x, cell->y, cell->format, cell->value);
}

int
cell_hit(cell* cell, int x, int y)
{
    if (x > cell->x && x < cell->x + 60 &&
        y > cell->y-30 && y < cell->y+10)
        return cell->id;
    return 0;
}

void
cell_update(cell* cell, int update)
{
    if (selection != cell->id)
        return;
    
    cell->value += update * cell->step;
    
    if (cell->value < cell->min)
        cell->value = cell->min;
    else if (cell->value > cell->max) 
        cell->value = cell->max;
    
}

void
cell_vector(float* dst, cell* cell, int num)
{
    while (--num >= 0)
        dst[num] = cell[num].value;
}

void
drawmodel(void)
{
    if (!pmodel) {
        pmodel = glmReadOBJ("data/al.obj");
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}

void
drawaxes(void)
{
    glColor3ub(255, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, -0.25, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(0.75, 0.0, 0.25);
    glVertex3f(0.75, 0.0, -0.25);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.75, 0.25);
    glVertex3f(0.0, 0.75, -0.25);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    glVertex3f(-0.25, 0.75, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(0.25, 0.0, 0.75);
    glVertex3f(-0.25, 0.0, 0.75);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.25, 0.75);
    glVertex3f(0.0, -0.25, 0.75);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    
    glColor3ub(255, 255, 0);
    glRasterPos3f(1.1, 0.0, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
    glRasterPos3f(0.0, 1.1, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
    glRasterPos3f(0.0, 0.0, 1.1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
}

void
identity(GLdouble m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

GLboolean
invert(GLdouble src[16], GLdouble inverse[16])
{
    double t;
    int i, j, k, swap;
    GLdouble tmp[4][4];
    
    identity(inverse);
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = src[i*4+j];
        }
    }
    
    for (i = 0; i < 4; i++) {
        /* look for largest element in column. */
        swap = i;
        for (j = i + 1; j < 4; j++) {
            if (fabs(tmp[j][i]) > fabs(tmp[i][i])) {
                swap = j;
            }
        }
        
        if (swap != i) {
            /* swap rows. */
            for (k = 0; k < 4; k++) {
                t = tmp[i][k];
                tmp[i][k] = tmp[swap][k];
                tmp[swap][k] = t;
                
                t = inverse[i*4+k];
                inverse[i*4+k] = inverse[swap*4+k];
                inverse[swap*4+k] = t;
            }
        }
        
        if (tmp[i][i] == 0) {
        /* no non-zero pivot.  the matrix is singular, which
           shouldn't happen.  This means the user gave us a bad
            matrix. */
            return GL_FALSE;
        }
        
        t = tmp[i][i];
        for (k = 0; k < 4; k++) {
            tmp[i][k] /= t;
            inverse[i*4+k] /= t;
        }
        for (j = 0; j < 4; j++) {
            if (j != i) {
                t = tmp[j][i];
                for (k = 0; k < 4; k++) {
                    tmp[j][k] -= tmp[i][k]*t;
                    inverse[j*4+k] -= inverse[i*4+k]*t;
                }
            }
        }
    }
    return GL_TRUE;
}

float
normalize(float* v)
{
    float length;
    
    length = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= length;
    v[1] /= length;
    v[2] /= length;
    
    return length;
}

void
main_reshape(int width,  int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
#define GAP  25             /* gap between subwindows */
    sub_width = (width-GAP*3)/2.0;
    sub_height = (height-GAP*3)/2.0;
    
    glutSetWindow(world);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
    glutSetWindow(screen);
    glutPositionWindow(GAP+sub_width+GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
    glutSetWindow(command);
    glutPositionWindow(GAP, GAP+sub_height+GAP);
    glutReshapeWindow(sub_width+GAP+sub_width, sub_height);
}

void
main_display(void)
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3ub(0, 0, 0);
    setfont("helvetica", 12);
    drawstr(GAP, GAP-5, "World-space view");
    drawstr(GAP+sub_width+GAP, GAP-5, "Screen-space view");
    drawstr(GAP, GAP+sub_height+GAP-5, "Command manipulation window");
    glutSwapBuffers();
}

void
main_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'p':
        mode = PERSPECTIVE;
        break;
    case 'o':
        mode = ORTHO;
        break;
    case 'f':
        mode = FRUSTUM;
        break;
    case 'r':
        perspective[0].value = 60.0;
        perspective[1].value = 1.0;
        perspective[2].value = 1.0;
        perspective[3].value = 10.0;
        ortho[0].value = -1.0;
        ortho[1].value = 1.0;
        ortho[2].value = -1.0;
        ortho[3].value = 1.0;
        ortho[4].value = 1.0;
        ortho[5].value = 3.5;
        frustum[0].value = -1.0;
        frustum[1].value = 1.0;
        frustum[2].value = -1.0;
        frustum[3].value = 1.0;
        frustum[4].value = 1.0;
        frustum[5].value = 3.5;
        lookat[0].value = 0.0;
        lookat[1].value = 0.0;
        lookat[2].value = 2.0;
        lookat[3].value = 0.0;
        lookat[4].value = 0.0;
        lookat[5].value = 0.0;
        lookat[6].value = 0.0;
        lookat[7].value = 1.0;
        lookat[8].value = 0.0;
        break;
    case 27:
        exit(0);
    }
    
    redisplay_all();
}

void
world_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width/height, 1.0, 256.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    glRotatef(-45.0, 0.0, 1.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
}

void
world_display(void)
{
    GLfloat light_pos[] = { 0.0, 0.0, 1.0, 0.0 };
    double length;
    float l[3];
    
    l[0] = lookat[3].value - lookat[0].value; 
    l[1] = lookat[4].value - lookat[1].value; 
    l[2] = lookat[5].value - lookat[2].value;
    length = normalize(l);
    
    invert(modelview, inverse);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (world_draw) {
        glEnable(GL_LIGHTING);
        glPushMatrix();
        glMultMatrixd(inverse);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glPopMatrix();
        drawmodel();
        glDisable(GL_LIGHTING);
    }
    
    glPushMatrix();
    
    glMultMatrixd(inverse);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    /* draw the axis and eye vector */
    glPushMatrix();
    glColor3ub(0, 0, 255);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, -1.0*length);
    glVertex3f(0.1, 0.0, -0.9*length);
    glVertex3f(-0.1, 0.0, -0.9*length);
    glVertex3f(0.0, 0.0, -1.0*length);
    glVertex3f(0.0, 0.1, -0.9*length);
    glVertex3f(0.0, -0.1, -0.9*length);
    glVertex3f(0.0, 0.0, -1.0*length);
    glEnd();
    glColor3ub(255, 255, 0);
    glRasterPos3f(0.0, 0.0, -1.1*length);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'e');
    glColor3ub(255, 0, 0);
    glScalef(0.4, 0.4, 0.4);
    drawaxes();
    glPopMatrix();
    
    invert(projection, inverse);
    glMultMatrixd(inverse);
    
    /* draw the viewing frustum */
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
    glVertex3i(1, 1, 1);
    glVertex3i(-1, 1, 1);
    glVertex3i(-1, -1, 1);
    glVertex3i(1, -1, 1);
    glEnd();
    
    glColor3ub(128, 196, 128);
    glBegin(GL_LINES);
    glVertex3i(1, 1, -1);
    glVertex3i(1, 1, 1);
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, 1, 1);
    glVertex3i(-1, -1, -1);
    glVertex3i(-1, -1, 1);
    glVertex3i(1, -1, -1);
    glVertex3i(1, -1, 1);
    glEnd();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2, 0.2, 0.4, 0.5);
    glBegin(GL_QUADS);
    glVertex3i(1, 1, -1);
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, -1, -1);
    glVertex3i(1, -1, -1);
    glEnd();
    glDisable(GL_BLEND);
    
    glPopMatrix();
    glutSwapBuffers();
}

void
world_menu(int value)
{
    switch (value) {
    case 'm':
        world_draw = !world_draw;
        break;
    }
    redisplay_all();
}

void
screen_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mode == PERSPECTIVE)
        gluPerspective(perspective[0].value, perspective[1].value, 
        perspective[2].value, perspective[3].value);
    else if (mode == ORTHO)
        glOrtho(ortho[0].value, ortho[1].value, ortho[2].value,
        ortho[3].value, ortho[4].value, ortho[5].value);
    else if (mode == FRUSTUM)
        glFrustum(frustum[0].value, frustum[1].value, frustum[2].value,
        frustum[3].value, frustum[4].value, frustum[5].value);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lookat[0].value, lookat[1].value, lookat[2].value,
        lookat[3].value, lookat[4].value, lookat[5].value,
        lookat[6].value, lookat[7].value, lookat[8].value);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glClearColor(0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void
screen_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawmodel();
    glutSwapBuffers();
}

void
screen_menu(int value)
{
    char* name = 0;
    
    switch (value) {
    case 'a':
        name = "data/al.obj";
        break;
    case 's':
        name = "data/soccerball.obj";
        break;
    case 'd':
        name = "data/dolphins.obj";
        break;
    case 'f':
        name = "data/flowers.obj";
        break;
    case 'j':
        name = "data/f-16.obj";
        break;
    case 'p':
        name = "data/porsche.obj";
        break;
    case 'r':
        name = "data/rose+vase.obj";
        break;
    }
    
    if (name) {
        pmodel = glmReadOBJ(name);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    redisplay_all();
}

void
command_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void
command_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255,255,255);
    
    setfont("helvetica", 18);
    
    if (mode == PERSPECTIVE) {
        drawstr(180, perspective[0].y-40, "fovy"); 
        drawstr(230, perspective[0].y-40, "aspect"); 
        drawstr(300, perspective[0].y-40, "zNear"); 
        drawstr(360, perspective[0].y-40, "zFar");
    } else {
        drawstr(120, perspective[0].y-40, "left"); 
        drawstr(180, perspective[0].y-40, "right"); 
        drawstr(230, perspective[0].y-40, "bottom");
        drawstr(310, perspective[0].y-40, "top"); 
        drawstr(360, perspective[0].y-40, "near");
        drawstr(420, perspective[0].y-40, "far");
    }
    
    if (mode == PERSPECTIVE) {
        drawstr(40, perspective[0].y, "gluPerspective(");
        drawstr(230, perspective[0].y, ","); 
        drawstr(290, perspective[0].y, ",");
        drawstr(350, perspective[0].y, ",");
        drawstr(410, perspective[0].y, ");");
    } else if (mode == FRUSTUM) {
        drawstr(20, frustum[0].y, "glFrustum(");
        drawstr(170, frustum[0].y, ",");
        drawstr(230, frustum[0].y, ","); 
        drawstr(290, frustum[0].y, ",");
        drawstr(350, frustum[0].y, ",");
        drawstr(410, frustum[0].y, ","); 
        drawstr(470, frustum[0].y, ");");
    } else {
        drawstr(35, ortho[0].y, "glOrtho(");
        drawstr(170, ortho[0].y, ",");
        drawstr(230, ortho[0].y, ","); 
        drawstr(290, ortho[0].y, ",");
        drawstr(350, ortho[0].y, ",");
        drawstr(410, ortho[0].y, ","); 
        drawstr(470, ortho[0].y, ");");
    }
    
    drawstr(78, lookat[0].y, "gluLookAt(");
    drawstr(230, lookat[0].y, ","); 
    drawstr(290, lookat[0].y, ",");
    drawstr(350, lookat[0].y, ",");
    drawstr(380, lookat[0].y, "<- eye");
    drawstr(230, lookat[3].y, ","); 
    drawstr(290, lookat[3].y, ",");
    drawstr(350, lookat[3].y, ",");
    drawstr(380, lookat[3].y, "<- center");
    drawstr(230, lookat[6].y, ","); 
    drawstr(290, lookat[6].y, ",");
    drawstr(350, lookat[6].y, ");");
    drawstr(380, lookat[6].y, "<- up");
    
    if (mode == PERSPECTIVE) {
        cell_draw(&perspective[0]);
        cell_draw(&perspective[1]);
        cell_draw(&perspective[2]);
        cell_draw(&perspective[3]); 
    } else if (mode == FRUSTUM) {
        cell_draw(&frustum[0]);
        cell_draw(&frustum[1]);
        cell_draw(&frustum[2]);
        cell_draw(&frustum[3]);
        cell_draw(&frustum[4]);
        cell_draw(&frustum[5]);
    } else if (mode == ORTHO) {
        cell_draw(&ortho[0]);
        cell_draw(&ortho[1]);
        cell_draw(&ortho[2]);
        cell_draw(&ortho[3]);
        cell_draw(&ortho[4]);
        cell_draw(&ortho[5]);
    }   
    
    cell_draw(&lookat[0]);
    cell_draw(&lookat[1]);
    cell_draw(&lookat[2]);
    cell_draw(&lookat[3]);
    cell_draw(&lookat[4]);
    cell_draw(&lookat[5]);
    cell_draw(&lookat[6]);
    cell_draw(&lookat[7]);
    cell_draw(&lookat[8]);
    
    if (!selection) {
        glColor3ub(255, 255, 0);
        drawstr(10, 240,
            "Click on the arguments and move the mouse to modify values.");
    }   
    
    glutSwapBuffers();
}

int old_y;

void
command_mouse(int button, int state, int x, int y)
{
    selection = 0;
    
    if (state == GLUT_DOWN) {
        if (mode == PERSPECTIVE) {
        /* mouse should only hit _one_ of the cells, so adding up all
            the hits just propagates a single hit. */
            selection += cell_hit(&perspective[0], x, y);
            selection += cell_hit(&perspective[1], x, y);
            selection += cell_hit(&perspective[2], x, y);
            selection += cell_hit(&perspective[3], x, y);
        } else if (mode == FRUSTUM) {
            selection += cell_hit(&frustum[0], x, y);
            selection += cell_hit(&frustum[1], x, y);
            selection += cell_hit(&frustum[2], x, y);
            selection += cell_hit(&frustum[3], x, y);
            selection += cell_hit(&frustum[4], x, y);
            selection += cell_hit(&frustum[5], x, y);
        } else if (mode == ORTHO) {
            selection += cell_hit(&ortho[0], x, y);
            selection += cell_hit(&ortho[1], x, y);
            selection += cell_hit(&ortho[2], x, y);
            selection += cell_hit(&ortho[3], x, y);
            selection += cell_hit(&ortho[4], x, y);
            selection += cell_hit(&ortho[5], x, y);
        }
        selection += cell_hit(&lookat[0], x, y);
        selection += cell_hit(&lookat[1], x, y);
        selection += cell_hit(&lookat[2], x, y);
        selection += cell_hit(&lookat[3], x, y);
        selection += cell_hit(&lookat[4], x, y);
        selection += cell_hit(&lookat[5], x, y);
        selection += cell_hit(&lookat[6], x, y);
        selection += cell_hit(&lookat[7], x, y);
        selection += cell_hit(&lookat[8], x, y);
    }
    
    old_y = y;
    
    redisplay_all();
}

void
command_motion(int x, int y)
{
    cell_update(&perspective[0], old_y-y);
    cell_update(&perspective[1], old_y-y);
    cell_update(&perspective[2], old_y-y);
    cell_update(&perspective[3], old_y-y);
    cell_update(&frustum[0], old_y-y);
    cell_update(&frustum[1], old_y-y);
    cell_update(&frustum[2], old_y-y);
    cell_update(&frustum[3], old_y-y);
    cell_update(&frustum[4], old_y-y);
    cell_update(&frustum[5], old_y-y);
    cell_update(&ortho[0], old_y-y);
    cell_update(&ortho[1], old_y-y);
    cell_update(&ortho[2], old_y-y);
    cell_update(&ortho[3], old_y-y);
    cell_update(&ortho[4], old_y-y);
    cell_update(&ortho[5], old_y-y);
    cell_update(&lookat[0], old_y-y);
    cell_update(&lookat[1], old_y-y);
    cell_update(&lookat[2], old_y-y);
    cell_update(&lookat[3], old_y-y);
    cell_update(&lookat[4], old_y-y);
    cell_update(&lookat[5], old_y-y);
    cell_update(&lookat[6], old_y-y);
    cell_update(&lookat[7], old_y-y);
    cell_update(&lookat[8], old_y-y);
    
    old_y = y;
    
    redisplay_all();
}

void
command_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}

void
redisplay_all(void)
{
    glutSetWindow(command);
    glutPostRedisplay();
    glutSetWindow(world);
    world_reshape(sub_width, sub_height);
    glutPostRedisplay();
    glutSetWindow(screen);
    screen_reshape(sub_width, sub_height);
    glutPostRedisplay();
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512+GAP*3, 512+GAP*3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
    
    window = glutCreateWindow("Projection");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
    
    world = glutCreateSubWindow(window, GAP, GAP, 256, 256);
    glutReshapeFunc(world_reshape);
    glutDisplayFunc(world_display);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(world_menu);
    glutAddMenuEntry("Toggle model", 'm');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    screen = glutCreateSubWindow(window, GAP+256+GAP, GAP, 256, 256);
    glutReshapeFunc(screen_reshape);
    glutDisplayFunc(screen_display);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(screen_menu);
    glutAddMenuEntry("Models", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Soccerball", 's');
    glutAddMenuEntry("Al Capone", 'a');
    glutAddMenuEntry("F-16 Jet", 'j');
    glutAddMenuEntry("Dolphins", 'd');
    glutAddMenuEntry("Flowers", 'f');
    glutAddMenuEntry("Porsche", 'p');
    glutAddMenuEntry("Rose", 'r');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    command = glutCreateSubWindow(window, GAP+256+GAP, GAP+256+GAP, 256, 256);
    glutReshapeFunc(command_reshape);
    glutDisplayFunc(command_display);
    glutMotionFunc(command_motion);
    glutMouseFunc(command_mouse);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(command_menu);
    glutAddMenuEntry("Projection", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("[o]  glOrtho", 'o');
    glutAddMenuEntry("[f]  glFrustum", 'f');
    glutAddMenuEntry("[p]  gluPerspective", 'p');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("[r]  Reset parameters", 'r');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Quit", 27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    redisplay_all();
    
    glutMainLoop();
    
    return 0;
}
