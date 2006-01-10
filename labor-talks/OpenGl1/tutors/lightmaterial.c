/*
    lightmaterial.c
    Nate Robins, 1997

    Tool for teaching about OpenGL lighting & material properties.
    
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "glm.h"
#include "materials.h"


typedef struct _cell {
    int id;
    int x, y;
    float min, max;
    float value;
    float step;
    char* info;
    char* format;
} cell;


cell light_pos[4] = {
    { 1, 210, 30, -5.0, 5.0, -2.0, 0.01,
        "Specifies X coordinate of light vector.", "%.2f" },
    { 2, 270, 30, -5.0, 5.0, 2.0, 0.01,
    "Specifies Y coordinate of light vector.", "%.2f" },
    { 3, 330, 30, -5.0, 5.0, 2.0, 0.01,
    "Specifies Z coordinate of light vector.", "%.2f" },
    { 4, 390, 30, 0.0, 1.0, 1.0, 1.0,
    "Specifies directional (0) or positional (1) light.", "%.2f" },
};

cell light_Ka[4] = {
    { 5, 200, 60, 0.0, 1.0, 0.0, 0.01,
        "Specifies ambient red intensity of the light.", "%.2f" },
    { 6, 260, 60, 0.0, 1.0, 0.0, 0.01,
    "Specifies ambient green intensity of the light.", "%.2f" },
    { 7, 320, 60, 0.0, 1.0, 0.0, 0.01,
    "Specifies ambient blue intensity of the light.", "%.2f" },
    { 8, 380, 60, 0.0, 1.0, 1.0, 0.01,
    "Specifies ambient alpha intensity of the light.", "%.2f" },
};

cell light_Kd[4] = {
    {  9, 200, 90, 0.0, 1.0, 1.0, 0.01,
        "Specifies diffuse red intensity of the light.", "%.2f" },
    { 10, 260, 90, 0.0, 1.0, 1.0, 0.01,
    "Specifies diffuse green intensity of the light.", "%.2f" },
    { 11, 320, 90, 0.0, 1.0, 1.0, 0.01,
    "Specifies diffuse blue intensity of the light.", "%.2f" },
    { 12, 380, 90, 0.0, 1.0, 1.0, 0.01,
    "Specifies diffuse alpha intensity of the light.", "%.2f" },
};

cell light_Ks[4] = {
    { 13, 200, 120, 0.0, 1.0, 1.0, 0.01,
        "Specifies specular red intensity of the light.", "%.2f" },
    { 14, 260, 120, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular green intensity of the light.", "%.2f" },
    { 15, 320, 120, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular blue intensity of the light.", "%.2f" },
    { 16, 380, 120, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular alpha intensity of the light.", "%.2f" },
};

cell spot_direction[3] = {
    { 17, 250, 260, -1.0, 1.0, 1.0, 0.01,
        "Specifies X coordinate of spotlight direction vector.", "%.2f" },
    { 18, 310, 260, -1.0, 1.0, -1.0, 0.01,
    "Specifies Y coordinate of spotlight direction vector.", "%.2f" },
    { 19, 370, 260, -1.0, 1.0, -1.0, 0.01,
    "Specifies Z coordinate of spotlight direction vector.", "%.2f" },
};

cell spot_exponent = { 20, 210, 290, 0.0, 128.0, 30.0, 1.0,
"Specifies intensity distribution of spotlight.", "%.0f" };
cell spot_cutoff = { 21, 410, 290, 0.0, 91.0, 91.0, 1.0,
"Specifies maximum spread angle of spotlight (180 = off).", "%.0f" };

cell Kc = { 22, 120, 410, 0.0, 5.0, 1.0, 0.01,
"Specifies constant attenuation factor.", "%.2f" };
cell Kl = { 23, 215, 410, 0.0, 5.0, 0.0, 0.01,
"Specifies linear attenuation factor.", "%.2f" };
cell Kq = { 24, 315, 410, 0.0, 5.0, 0.0, 0.01,
"Specifies quadratic attenuation factor.", "%.2f" };

cell material_Ka[4] = {
    { 25, 220, 260, 0.0, 1.0, 0.2, 0.01,
        "Specifies ambient red reflectance of the material.", "%.2f" },
    { 26, 280, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient green reflectance of the material.", "%.2f" },
    { 27, 340, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient blue reflectance of the material.", "%.2f" },
    { 28, 400, 260, 0.0, 1.0, 1.0, 0.01,
    "Specifies ambient alpha reflectance of the material.", "%.2f" },
};

cell material_Kd[4] = {
    { 29, 220, 290, 0.0, 1.0, 0.8, 0.01,
        "Specifies diffuse red reflectance of the material.", "%.2f" },
    { 30, 280, 290, 0.0, 1.0, 0.8, 0.01,
    "Specifies diffuse green reflectance of the material.", "%.2f" },
    { 31, 340, 290, 0.0, 1.0, 0.8, 0.01,
    "Specifies diffuse blue reflectance of the material.", "%.2f" },
    { 32, 400, 290, 0.0, 1.0, 1.0, 0.01,
    "Specifies diffuse alpha reflectance of the material.", "%.2f" },
};

cell material_Ks[4] = {
    { 33, 220, 320, 0.0, 1.0, 1.0, 0.01,
        "Specifies specular red reflectance of the material.", "%.2f" },
    { 34, 280, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular green reflectance of the material.", "%.2f" },
    { 35, 340, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular blue reflectance of the material.", "%.2f" },
    { 36, 400, 320, 0.0, 1.0, 1.0, 0.01,
    "Specifies specular alpha reflectance of the material.", "%.2f" },
};

cell material_Ke[4] = {
    { 37, 220, 350, 0.0, 1.0, 0.0, 0.01,
        "Specifies red emitted light intensity of the material.", "%.2f" },
    { 38, 280, 350, 0.0, 1.0, 0.0, 0.01,
    "Specifies green emitted light intensity of the material.", "%.2f" },
    { 39, 340, 350, 0.0, 1.0, 0.0, 0.01,
    "Specifies blue emitted light intensity of the material.", "%.2f" },
    { 40, 400, 350, 0.0, 1.0, 1.0, 0.01,
    "Specifies alpha emitted light intensity of the material.", "%.2f" },
};

cell material_Se = { 41, 200, 380, 0.0, 128.0, 50.0, 1.0,
"Specifies the specular exponent of the material.", "%.0f" };

cell lmodel_Ka[4] = {
    { 42, 220, 260, 0.0, 1.0, 0.2, 0.01,
        "Specifies ambient red intensity of the entire scene.", "%.2f" },
    { 43, 280, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient green intensity of the entire scene.", "%.2f" },
    { 44, 340, 260, 0.0, 1.0, 0.2, 0.01,
    "Specifies ambient blue intensity of the entire scene.", "%.2f" },
    { 45, 400, 260, 0.0, 1.0, 1.0, 0.01,
    "Specifies ambient alpha intensity of the entire scene.", "%.2f" },
};

cell local_viewer = { 46, 460, 340, 0.0, 1.0, 0.0, 1.0,
"Specifies infinite (0.0) or local (1.0) light model.", "%.1f" };

cell two_side = { 47, 415, 390, 0.0, 1.0, 0.0, 1.0,
"Specifies one (0.0) or two (1.0) sided lighting.", "%.1f" };


GLfloat eye[3] = { 0.0, 0.0, 3.0 };
GLfloat at[3]  = { 0.0, 0.0, 0.0 };
GLfloat up[3]  = { 0.0, 1.0, 0.0 };

GLboolean world_draw = GL_TRUE;
GLMmodel* pmodel = NULL;
GLint selection = 0;
GLfloat spin_x = 0.0;
GLfloat spin_y = 0.0;

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
        drawstr(10, 525, cell->info);
        glColor3ub(255, 0, 0);
    }
    
    if (cell->id == 21 && cell->value > 90.0) /* treat cutoff specially */
        drawstr(cell->x, cell->y, cell->format, 180.0);
    else 
        drawstr(cell->x, cell->y, cell->format, cell->value);
}

int
cell_hit(cell* cell, int x, int y)
{
    if (x > cell->x && x < cell->x+60 &&
        y > cell->y-20 && y < cell->y+10)
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
        pmodel = glmReadOBJ("data/soccerball.obj");
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH);
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
    sub_width = (width-GAP*3)/3;
    sub_height = (height-GAP*3)/2;
    
    glutSetWindow(screen);
    glutPositionWindow(GAP, GAP);
    glutReshapeWindow(sub_width, sub_height);
    glutSetWindow(world);
    glutPositionWindow(GAP, GAP+sub_height+GAP);
    glutReshapeWindow(sub_width, sub_height);
    glutSetWindow(command);
    glutPositionWindow(GAP+sub_width+GAP, GAP);
    glutReshapeWindow(sub_width*2, sub_height*2+GAP);
}

void
main_display(void)
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3ub(0, 0, 0);
    setfont("helvetica", 12);
    drawstr(GAP, GAP-5, "Screen-space view");
    drawstr(GAP+sub_width+GAP, GAP-5, "Command manipulation window");
    drawstr(GAP, GAP+sub_height+GAP-5, "World-space view");
    glutSwapBuffers();
}

void
world_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width/height, 0.01, 256.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -6.0);
    glRotatef(-45.0, 0.0, 1.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void
world_display(void)
{
    double length;
    float l[3];
    GLfloat pos[4], lKa[4], lKd[4], lKs[4];
    GLfloat dir[3], mKa[4], mKd[4], mKs[4], mKe[4];
    GLfloat lmKa[4];
    
    cell_vector(pos, light_pos, 4);
    cell_vector(lKa, light_Ka, 4);
    cell_vector(lKd, light_Kd, 4);
    cell_vector(lKs, light_Ks, 4);
    cell_vector(dir, spot_direction, 3);
    cell_vector(mKa, material_Ka, 4);
    cell_vector(mKd, material_Kd, 4);
    cell_vector(mKs, material_Ks, 4);
    cell_vector(mKe, material_Ke, 4);
    cell_vector(lmKa, lmodel_Ka, 4);
    
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer.value);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, two_side.value);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
    glMaterialf(GL_FRONT, GL_SHININESS, material_Se.value);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lKs);
    glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, (int)spot_exponent.value);
    if (spot_cutoff.value > 90)
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    else
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, (int)spot_cutoff.value);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc.value);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl.value);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq.value);
    
    l[0] = at[0] - eye[0]; 
    l[1] = at[1] - eye[1]; 
    l[2] = at[2] - eye[2];
    
    invert(modelview, inverse);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glMultMatrixd(inverse);
    glTranslatef(l[0], l[1], l[2]);
    glColor3fv(lKd);
    glBegin(GL_LINE_STRIP);
    if (spot_cutoff.value > 90)
        glVertex3f(0, 0, 0);
    else
        glVertex3f(pos[0]+spot_direction[0].value,
        pos[1]+spot_direction[1].value, 
        pos[2]+spot_direction[2].value);
    
    if (pos[3] == 0)        /* 10.0 = 'infinite' light */
        glVertex3f(pos[0]*10.0,pos[1]*10.0,pos[2]*10.0);
    else 
        glVertex3f(pos[0], pos[1], pos[2]);
    glEnd();
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();
    
    length = normalize(l);
    
    if (world_draw) {
        glEnable(GL_LIGHTING);
        if (pmodel)
            drawmodel();
        else 
            glutSolidTorus(0.25, 0.75, 28, 28);
        glDisable(GL_LIGHTING);
    }
    
#if 0    
#define TESS 20
    glNormal3f(0.0, 1.0, 0.0);
    for (i = 0; i < TESS; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (j = 0; j <= TESS; j++) {
            glVertex3f(-1+(float)i/TESS*2, -1.0, -1+(float)j/TESS*2);
            glVertex3f(-1+(float)(i+1)/TESS*2, -1.0, -1+(float)j/TESS*2);
        }
        glEnd();
    }
#endif
    
    glPushMatrix();
    glMultMatrixd(inverse);
    
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
new_material(float* material)
{
    material_Ka[0].value = material[0];
    material_Ka[1].value = material[1];
    material_Ka[2].value = material[2];
    material_Ka[3].value = material[3];
    material_Kd[0].value = material[4];
    material_Kd[1].value = material[5];
    material_Kd[2].value = material[6];
    material_Kd[3].value = material[7];
    material_Ks[0].value = material[8];
    material_Ks[1].value = material[9];
    material_Ks[2].value = material[10];
    material_Ks[3].value = material[11];
    material_Ke[0].value = 0;
    material_Ke[1].value = 0;
    material_Ke[2].value = 0;
    material_Ke[3].value = 0;
    material_Se.value = material[12];
}

void
world_menu(int value)
{
    switch (value) {
    case 1:
        new_material(Brass);
        break;
    case 2:
        new_material(Bronze);
        break;
    case 3:
        new_material(Polished_Bronze);
        break;
    case 4:
        new_material(Chrome);
        break;
    case 5:
        new_material(Copper);
        break;
    case 6:
        new_material(Polished_Copper);
        break;
    case 7:
        new_material(Gold);
        break;
    case 8:
        new_material(Polished_Gold);
        break;
    case 9:
        new_material(Pewter);
        break;
    case 10:
        new_material(Silver);
        break;
    case 11:
        new_material(Polished_Silver);
        break;
    case 12:
        new_material(Emerald);
        break;
    case 13:
        new_material(Jade);
        break;
    case 14:
        new_material(Obsidian);
        break;
    case 15:
        new_material(Pearl);
        break;
    case 16:
        new_material(Ruby);
        break;
    case 17:
        new_material(Turquoise);
        break;
    case 18:
        new_material(Black_Plastic);
        break;
    case 19:
        new_material(Black_Rubber);
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
    gluPerspective(60.0, (float)width/height, 0.5, 8.0);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1],up[2]);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void
screen_display(void)
{
    GLfloat pos[4], lKa[4], lKd[4], lKs[4];
    GLfloat dir[3], mKa[4], mKd[4], mKs[4], mKe[4];
    GLfloat lmKa[4];
    
    cell_vector(pos, light_pos, 4);
    cell_vector(lKa, light_Ka, 4);
    cell_vector(lKd, light_Kd, 4);
    cell_vector(lKs, light_Ks, 4);
    cell_vector(dir, spot_direction, 3);
    cell_vector(mKa, material_Ka, 4);
    cell_vector(mKd, material_Kd, 4);
    cell_vector(mKs, material_Ks, 4);
    cell_vector(mKe, material_Ke, 4);
    cell_vector(lmKa, lmodel_Ka, 4);
    
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer.value);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, two_side.value);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lKs);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
    glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, (int)spot_exponent.value);
    if (spot_cutoff.value > 90)
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    else
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, (int)spot_cutoff.value);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc.value);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl.value);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq.value);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
    glMaterialf(GL_FRONT, GL_SHININESS, material_Se.value);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spin_y, 1.0, 0.0, 0.0);
    glRotatef(spin_x, 0.0, 1.0, 0.0);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    if (pmodel)
        drawmodel();
    else 
        glutSolidTorus(0.25, 0.75, 28, 28);
    glPopMatrix();
    
#if 0    
#define TESS 20
    glNormal3f(0.0, 1.0, 0.0);
    for (i = 0; i < TESS; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (j = 0; j <= TESS; j++) {
            glVertex3f(-1+(float)i/TESS*2, -1.0, -1+(float)j/TESS*2);
            glVertex3f(-1+(float)(i+1)/TESS*2, -1.0, -1+(float)j/TESS*2);
        }
        glEnd();
    }
#endif
    
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
    case 'n':
        if (pmodel) glmDelete(pmodel);
        pmodel = NULL;
        redisplay_all();
        return;
    }
    
    if (name) {
        if (pmodel) glmDelete(pmodel);
        pmodel = glmReadOBJ(name);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    redisplay_all();
}

int old_x, old_y;

void
screen_mouse(int button, int state, int x, int y)
{
    old_x = x;
    old_y = y;
    
    redisplay_all();
}

void
screen_motion(int x, int y)
{
    spin_x = x - old_x;
    spin_y = y - old_y;
    
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
lighting_display(void)
{
    setfont("helvetica", 18);
    drawstr(10, light_pos[0].y, "GLfloat light_pos[ ] = {");
    drawstr(10, light_Ka[0].y, "GLfloat light_Ka[ ] = {");
    drawstr(10, light_Kd[0].y, "GLfloat light_Kd[ ] = {");
    drawstr(10, light_Ks[0].y, "GLfloat light_Ks[ ] = {");
    drawstr(light_pos[0].x+50, light_pos[0].y, ",");
    drawstr(light_pos[1].x+50, light_pos[1].y, ",");
    drawstr(light_pos[2].x+50, light_pos[2].y, ",");
    drawstr(light_pos[3].x+50, light_pos[3].y, "};");
    drawstr(light_Ka[0].x+50, light_Ka[0].y, ",");
    drawstr(light_Ka[1].x+50, light_Ka[1].y, ",");
    drawstr(light_Ka[2].x+50, light_Ka[2].y, ",");
    drawstr(light_Ka[3].x+50, light_Ka[3].y, "};");
    drawstr(light_Kd[0].x+50, light_Kd[0].y, ",");
    drawstr(light_Kd[1].x+50, light_Kd[1].y, ",");
    drawstr(light_Kd[2].x+50, light_Kd[2].y, ",");
    drawstr(light_Kd[3].x+50, light_Kd[3].y, "};");
    drawstr(light_Ks[0].x+50, light_Ks[0].y, ",");
    drawstr(light_Ks[1].x+50, light_Ks[1].y, ",");
    drawstr(light_Ks[2].x+50, light_Ks[2].y, ",");
    drawstr(light_Ks[3].x+50, light_Ks[3].y, "};");
    
    setfont("helvetica", 12);
    drawstr(10, light_Ks[0].y+30,
        "glLightfv(GL_LIGHT0, GL_POSITION, light_pos);");
    drawstr(10, light_Ks[1].y+50,
        "glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);");
    drawstr(10, light_Ks[2].y+70,
        "glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);");
    drawstr(10, light_Ks[3].y+90,
        "glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);");
    
    setfont("helvetica", 18);
    
    cell_draw(&light_pos[0]);
    cell_draw(&light_pos[1]);
    cell_draw(&light_pos[2]);
    cell_draw(&light_pos[3]);
    
    cell_draw(&light_Ka[0]);
    cell_draw(&light_Ka[1]);
    cell_draw(&light_Ka[2]);
    cell_draw(&light_Ka[3]);
    
    cell_draw(&light_Kd[0]);
    cell_draw(&light_Kd[1]);
    cell_draw(&light_Kd[2]);
    cell_draw(&light_Kd[3]);
    
    cell_draw(&light_Ks[0]);
    cell_draw(&light_Ks[1]);
    cell_draw(&light_Ks[2]);
    cell_draw(&light_Ks[3]);
    
    glColor3ub(255, 255, 255);
}

void
spotlight_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255, 255, 255);
    
    lighting_display();
    
    setfont("helvetica", 18);
    drawstr(10, spot_direction[0].y, "GLfloat spot_direction[ ] = {");
    drawstr(10, spot_exponent.y, "GLint spot_exponent =                 "
        "spot_cutoff =   ");
    drawstr(spot_direction[0].x+50, spot_direction[0].y, ",");
    drawstr(spot_direction[1].x+50, spot_direction[1].y, ",");
    drawstr(spot_direction[2].x+50, spot_direction[2].y, "};");
    drawstr(spot_exponent.x+40, spot_cutoff.y, ",");
    drawstr(spot_cutoff.x+40, spot_cutoff.y, ";");
    
    setfont("helvetica", 12);
    drawstr(10, spot_cutoff.y+30,
        "glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);");
    drawstr(10, spot_cutoff.y+50,
        "glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);");
    drawstr(10, spot_cutoff.y+70,
        "glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);");
    
    setfont("helvetica", 18);
    drawstr(10, Kc.y, 
        "GLfloat Kc =          , Kl =          , Kq =          ;");
    
    setfont("helvetica", 12);
    drawstr(10, Kq.y+30, "glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);");
    drawstr(10, Kq.y+50, "glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);");
    drawstr(10, Kq.y+70, "glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);");
    
    setfont("helvetica", 18);
    cell_draw(&spot_direction[0]);
    cell_draw(&spot_direction[1]);
    cell_draw(&spot_direction[2]);
    
    cell_draw(&spot_exponent);
    cell_draw(&spot_cutoff);
    
    cell_draw(&Kc);
    cell_draw(&Kl);
    cell_draw(&Kq);
    
    if (!selection) {
        glColor3ub(255, 255, 0);
        drawstr(10, 525,
            "Click on the arguments and move the mouse to modify values.");
    }   
    
    glutSwapBuffers();
}

void
material_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255, 255, 255);
    
    lighting_display();
    
    setfont("helvetica", 18);
    drawstr(10, material_Ka[0].y, "GLfloat material_Ka[ ] = {");
    drawstr(10, material_Kd[0].y, "GLfloat material_Kd[ ] = {");
    drawstr(10, material_Ks[0].y, "GLfloat material_Ks[ ] = {");
    drawstr(10, material_Ke[0].y, "GLfloat material_Ke[ ] = {");
    drawstr(10, material_Se.y, "GLfloat material_Se =          ;");
    drawstr(material_Ka[0].x+50, material_Ka[0].y, ",");
    drawstr(material_Ka[1].x+50, material_Ka[1].y, ",");
    drawstr(material_Ka[2].x+50, material_Ka[2].y, ",");
    drawstr(material_Ka[3].x+50, material_Ka[3].y, "};");
    drawstr(material_Kd[0].x+50, material_Kd[0].y, ",");
    drawstr(material_Kd[1].x+50, material_Kd[1].y, ",");
    drawstr(material_Kd[2].x+50, material_Kd[2].y, ",");
    drawstr(material_Kd[3].x+50, material_Kd[3].y, "};");
    drawstr(material_Ks[0].x+50, material_Ks[0].y, ",");
    drawstr(material_Ks[1].x+50, material_Ks[1].y, ",");
    drawstr(material_Ks[2].x+50, material_Ks[2].y, ",");
    drawstr(material_Ks[3].x+50, material_Ks[3].y, "};");
    drawstr(material_Ke[0].x+50, material_Ke[0].y, ",");
    drawstr(material_Ke[1].x+50, material_Ke[1].y, ",");
    drawstr(material_Ke[2].x+50, material_Ke[2].y, ",");
    drawstr(material_Ke[3].x+50, material_Ke[3].y, "};");
    
    setfont("helvetica", 12);
    drawstr(10, material_Se.y+30,
        "glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);");
    drawstr(10, material_Se.y+50,
        "glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);");
    drawstr(10, material_Se.y+70,
        "glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);");
    drawstr(10, material_Se.y+90,
        "glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);");
    drawstr(10, material_Se.y+110,
        "glMaterialfv(GL_FRONT, GL_SHININESS, material_Se);");
    
    setfont("helvetica", 18);
    
    cell_draw(&material_Ka[0]);
    cell_draw(&material_Ka[1]);
    cell_draw(&material_Ka[2]);
    cell_draw(&material_Ka[3]);
    
    cell_draw(&material_Kd[0]);
    cell_draw(&material_Kd[1]);
    cell_draw(&material_Kd[2]);
    cell_draw(&material_Kd[3]);
    
    cell_draw(&material_Ks[0]);
    cell_draw(&material_Ks[1]);
    cell_draw(&material_Ks[2]);
    cell_draw(&material_Ks[3]);
    
    cell_draw(&material_Ke[0]);
    cell_draw(&material_Ke[1]);
    cell_draw(&material_Ke[2]);
    cell_draw(&material_Ke[3]);
    
    cell_draw(&material_Se);
    
    if (!selection) {
        glColor3ub(255, 255, 0);
        drawstr(10, 525,
            "Click on the arguments and move the mouse to modify values.");
    }   
    
    glutSwapBuffers();
}

void
lmodel_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255, 255, 255);
    
    lighting_display();
    
    setfont("helvetica", 18);
    drawstr(10, lmodel_Ka[0].y, "GLfloat lmodel_Ka[ ] = {");
    drawstr(lmodel_Ka[0].x+50, lmodel_Ka[0].y, ",");
    drawstr(lmodel_Ka[1].x+50, lmodel_Ka[1].y, ",");
    drawstr(lmodel_Ka[2].x+50, lmodel_Ka[2].y, ",");
    drawstr(lmodel_Ka[3].x+50, lmodel_Ka[3].y, "};");
    
    setfont("helvetica", 12);
    drawstr(10, lmodel_Ka[3].y+30,
        "glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_Ka);");
    
    setfont("helvetica", 18);
    drawstr(10, local_viewer.y, "glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, ");
    drawstr(local_viewer.x+35, local_viewer.y, ");");
    
    drawstr(10, two_side.y, "glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, ");
    drawstr(two_side.x+35, two_side.y, ");");
    
    cell_draw(&lmodel_Ka[0]);
    cell_draw(&lmodel_Ka[1]);
    cell_draw(&lmodel_Ka[2]);
    cell_draw(&lmodel_Ka[3]);
    
    cell_draw(&local_viewer);
    cell_draw(&two_side);
    
    if (!selection) {
        glColor3ub(255, 255, 0);
        drawstr(10, 525,
            "Click on the arguments and move the mouse to modify values.");
    }   
    
    glutSwapBuffers();
}

void
lighting_mouse(int x, int y)
{
/* mouse should only hit _one_ of the cells, so adding up all
    the hits just propagates a single hit. */
    selection += cell_hit(&light_pos[0], x, y);
    selection += cell_hit(&light_pos[1], x, y);
    selection += cell_hit(&light_pos[2], x, y);
    selection += cell_hit(&light_pos[3], x, y);
    selection += cell_hit(&light_Ka[0], x, y);
    selection += cell_hit(&light_Ka[1], x, y);
    selection += cell_hit(&light_Ka[2], x, y);
    selection += cell_hit(&light_Ka[3], x, y);
    selection += cell_hit(&light_Kd[0], x, y);
    selection += cell_hit(&light_Kd[1], x, y);
    selection += cell_hit(&light_Kd[2], x, y);
    selection += cell_hit(&light_Kd[3], x, y);
    selection += cell_hit(&light_Ks[0], x, y);
    selection += cell_hit(&light_Ks[1], x, y);
    selection += cell_hit(&light_Ks[2], x, y);
    selection += cell_hit(&light_Ks[3], x, y);
}

void
material_mouse(int button, int state, int x, int y)
{
    selection = 0;
    
    if (state == GLUT_DOWN) {
        lighting_mouse(x, y);
        /* mouse should only hit _one_ of the cells, so adding up all
        the hits just propagates a single hit. */
        selection += cell_hit(&material_Ka[0], x, y);
        selection += cell_hit(&material_Ka[1], x, y);
        selection += cell_hit(&material_Ka[2], x, y);
        selection += cell_hit(&material_Ka[3], x, y);
        selection += cell_hit(&material_Kd[0], x, y);
        selection += cell_hit(&material_Kd[1], x, y);
        selection += cell_hit(&material_Kd[2], x, y);
        selection += cell_hit(&material_Kd[3], x, y);
        selection += cell_hit(&material_Ks[0], x, y);
        selection += cell_hit(&material_Ks[1], x, y);
        selection += cell_hit(&material_Ks[2], x, y);
        selection += cell_hit(&material_Ks[3], x, y);
        selection += cell_hit(&material_Ke[0], x, y);
        selection += cell_hit(&material_Ke[1], x, y);
        selection += cell_hit(&material_Ke[2], x, y);
        selection += cell_hit(&material_Ke[3], x, y);
        selection += cell_hit(&material_Se, x, y);
    }
    
    old_y = y;
    
    redisplay_all();
}

void
spotlight_mouse(int button, int state, int x, int y)
{
    selection = 0;
    
    if (state == GLUT_DOWN) {
        lighting_mouse(x, y);
        /* mouse should only hit _one_ of the cells, so adding up all
        the hits just propagates a single hit. */
        selection += cell_hit(&spot_direction[0], x, y);
        selection += cell_hit(&spot_direction[1], x, y);
        selection += cell_hit(&spot_direction[2], x, y);
        selection += cell_hit(&spot_exponent, x, y);
        selection += cell_hit(&spot_cutoff, x, y);
        selection += cell_hit(&Kc, x, y);
        selection += cell_hit(&Kl, x, y);
        selection += cell_hit(&Kq, x, y);
    }
    
    old_y = y;
    
    redisplay_all();
}

void
lmodel_mouse(int button, int state, int x, int y)
{
    selection = 0;
    
    if (state == GLUT_DOWN) {
        lighting_mouse(x, y);
        /* mouse should only hit _one_ of the cells, so adding up all
        the hits just propagates a single hit. */
        selection += cell_hit(&lmodel_Ka[0], x, y);
        selection += cell_hit(&lmodel_Ka[1], x, y);
        selection += cell_hit(&lmodel_Ka[2], x, y);
        selection += cell_hit(&lmodel_Ka[3], x, y);
        selection += cell_hit(&local_viewer, x, y);
        selection += cell_hit(&two_side, x, y);
    }
    
    old_y = y;
    
    redisplay_all();
}

void
command_motion(int x, int y)
{
    cell_update(&light_pos[0], old_y-y);
    cell_update(&light_pos[1], old_y-y);
    cell_update(&light_pos[2], old_y-y);
    cell_update(&light_pos[3], old_y-y);
    cell_update(&light_Ka[0], old_y-y);
    cell_update(&light_Ka[1], old_y-y);
    cell_update(&light_Ka[2], old_y-y);
    cell_update(&light_Ka[3], old_y-y);
    cell_update(&light_Kd[0], old_y-y);
    cell_update(&light_Kd[1], old_y-y);
    cell_update(&light_Kd[2], old_y-y);
    cell_update(&light_Kd[3], old_y-y);
    cell_update(&light_Ks[0], old_y-y);
    cell_update(&light_Ks[1], old_y-y);
    cell_update(&light_Ks[2], old_y-y);
    cell_update(&light_Ks[3], old_y-y);
    cell_update(&spot_direction[0], old_y-y);
    cell_update(&spot_direction[1], old_y-y);
    cell_update(&spot_direction[2], old_y-y);
    cell_update(&spot_exponent, old_y-y);
    cell_update(&spot_cutoff, old_y-y);
    cell_update(&Kc, old_y-y);
    cell_update(&Kl, old_y-y);
    cell_update(&Kq, old_y-y);
    cell_update(&material_Ka[0], old_y-y);
    cell_update(&material_Ka[1], old_y-y);
    cell_update(&material_Ka[2], old_y-y);
    cell_update(&material_Ka[3], old_y-y);
    cell_update(&material_Kd[0], old_y-y);
    cell_update(&material_Kd[1], old_y-y);
    cell_update(&material_Kd[2], old_y-y);
    cell_update(&material_Kd[3], old_y-y);
    cell_update(&material_Ks[0], old_y-y);
    cell_update(&material_Ks[1], old_y-y);
    cell_update(&material_Ks[2], old_y-y);
    cell_update(&material_Ks[3], old_y-y);
    cell_update(&material_Ke[0], old_y-y);
    cell_update(&material_Ke[1], old_y-y);
    cell_update(&material_Ke[2], old_y-y);
    cell_update(&material_Ke[3], old_y-y);
    cell_update(&material_Se, old_y-y);
    cell_update(&lmodel_Ka[0], old_y-y);
    cell_update(&lmodel_Ka[1], old_y-y);
    cell_update(&lmodel_Ka[2], old_y-y);
    cell_update(&lmodel_Ka[3], old_y-y);
    cell_update(&local_viewer, old_y-y);
    cell_update(&two_side, old_y-y);
    
    old_y = y;
    
    redisplay_all();
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

void
main_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
        light_pos[0].value = -2.0;
        light_pos[1].value = 2.0;
        light_pos[2].value = 2.0;
        light_pos[3].value = 1.0;
        light_Ka[0].value = 0;
        light_Ka[1].value = 0;
        light_Ka[2].value = 0;
        light_Ka[3].value = 1;
        light_Kd[0].value = 1;
        light_Kd[1].value = 1;
        light_Kd[2].value = 1;
        light_Kd[3].value = 1;
        light_Ks[0].value = 1;
        light_Ks[1].value = 1;
        light_Ks[2].value = 1;
        light_Ks[3].value = 1;
        spot_direction[0].value = 1.0;
        spot_direction[1].value = -1.0;
        spot_direction[2].value = -1.0;
        spot_exponent.value = 30.0;
        spot_cutoff.value = 91.0;
        Kc.value = 1.0;
        Kl.value = 0.0;
        Kq.value = 0.0;
        new_material(Pewter);
        lmodel_Ka[0].value = 0.2;
        lmodel_Ka[1].value = 0.2;
        lmodel_Ka[2].value = 0.2;
        lmodel_Ka[3].value = 1.0;
        local_viewer.value = 0;
        two_side.value = 0;
        break;
    case 'm':
        glutSetWindow(command);
        glutMouseFunc(material_mouse);
        glutDisplayFunc(material_display);
        break;
    case 's':
        glutSetWindow(command);
        glutMouseFunc(spotlight_mouse);
        glutDisplayFunc(spotlight_display);
        break;
    case 'l':
        glutSetWindow(command);
        glutMouseFunc(lmodel_mouse);
        glutDisplayFunc(lmodel_display);
        break;
    case 27:
        exit(0);
    }
    
    redisplay_all();
}

void
command_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize((512+GAP*3)*3/2, 512+GAP*3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
    
    window = glutCreateWindow("Light & Material");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
    
    world = glutCreateSubWindow(window, GAP, GAP, 256, 256);
    glutReshapeFunc(world_reshape);
    glutDisplayFunc(world_display);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(world_menu);
    glutAddMenuEntry("Materials", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Brass", 1);
    glutAddMenuEntry("Bronze", 2);
    glutAddMenuEntry("Polished_Bronze", 3);
    glutAddMenuEntry("Chrome", 4);
    glutAddMenuEntry("Copper", 5);
    glutAddMenuEntry("Polished_Copper", 6);
    glutAddMenuEntry("Gold", 7);
    glutAddMenuEntry("Polished_Gold", 8);
    glutAddMenuEntry("Pewter", 9);
    glutAddMenuEntry("Silver", 10);
    glutAddMenuEntry("Polished_Silver", 11);
    glutAddMenuEntry("Emerald", 12);
    glutAddMenuEntry("Jade", 13);
    glutAddMenuEntry("Obsidian", 14);
    glutAddMenuEntry("Pearl", 15);
    glutAddMenuEntry("Ruby", 16);
    glutAddMenuEntry("Turquoise", 17);
    glutAddMenuEntry("Black_Plastic", 18);
    glutAddMenuEntry("Black_Rubber", 19);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    screen = glutCreateSubWindow(window, GAP+256+GAP, GAP, 256, 256);
    glutReshapeFunc(screen_reshape);
    glutDisplayFunc(screen_display);
    glutKeyboardFunc(main_keyboard);
    glutMotionFunc(screen_motion);
    glutMouseFunc(screen_mouse);
    glutCreateMenu(screen_menu);
    glutAddMenuEntry("Models", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Torus", 'n');
    glutAddMenuEntry("Flat plane", 'l');
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
    glutDisplayFunc(material_display);
    glutMotionFunc(command_motion);
    glutMouseFunc(material_mouse);
    glutKeyboardFunc(main_keyboard);
    glutCreateMenu(command_menu);
    glutAddMenuEntry("Light & Material", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Material parameters", 'm');
    glutAddMenuEntry("Light model parameters", 'l');
    glutAddMenuEntry("Spotlight & attenuation", 's');
    glutAddMenuEntry("Reset parameters (r)", 'r');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Quit", 27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    new_material(Pewter);
    redisplay_all();
    
    glutMainLoop();
    return 0;
}
