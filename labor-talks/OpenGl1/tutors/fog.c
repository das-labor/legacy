/*
    fog.c
    Nate Robins, 1997

    Tool for teaching about OpenGL fog.
    
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


cell color[4] = {
    { 1, 180, 40, 0.0, 1.0, 0.7, 0.005,
        "Specifies red component of fog color.", "%.2f" },
    { 2, 240, 40, 0.0, 1.0, 0.7, 0.005, 
    "Specifies green component of fog color.", "%.2f" },
    { 3, 300, 40, 0.0, 1.0, 0.7, 0.005, 
    "Specifies blue component of fog color.", "%.2f" },
    { 4, 360, 40, 0.0, 1.0, 1.0, 0.005, 
    "Specifies alpha component of fog color.", "%.2f" }
};

cell density = { 5, 260, 120,  0.0, 3.0, 1.0, 0.01, 
"Specifies the near distance.", "%.2f" };
cell fstart  = { 6, 240, 120, -3.0, 3.0, 0.5, 0.01, 
"Specifies the far distance.", "%.2f" };
cell fend    = { 7, 240, 160, -3.0, 3.0, 2.0, 0.01,
"Specifies the fog density.", "%.2f" };

GLenum mode = GL_LINEAR;

GLboolean clear = GL_TRUE;
GLMmodel* pmodel = NULL;


GLfloat eye[3] = { 0.5, 0.25, 1.5 };
GLfloat at[3]  = { 0.0, -0.25, 0.0 };
GLfloat up[3]  = { 0.0, 1.0, 0.0 };

void redisplay_all(void);
GLuint window, world, screen, command;
GLuint sub_width = 256, sub_height = 256;
GLint selection = 0;


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
        pmodel = glmReadOBJ("data/f-16.obj");
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
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
    drawstr(GAP, GAP-5, "Fog equation");
    drawstr(GAP+sub_width+GAP, GAP-5, "Screen-space view");
    drawstr(GAP, GAP+sub_height+GAP-5, "Command manipulation window");
    glutSwapBuffers();
}

void
main_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'l':
        mode = GL_LINEAR;
        break;
    case 'e':
        mode = GL_EXP;
        break;
    case 'x':
        mode = GL_EXP2;
        break;
    case 'c':
        clear = !clear;
        break;
    case 'r':
        color[0].value = 0.7;
        color[1].value = 0.7;
        color[2].value = 0.7;
        color[3].value = 1.0;
        density.value = 1.0;
        fstart.value = 0.5;
        fend.value = 2.0;
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
    glOrtho(0, width, height, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(2.0);
}

void
world_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3ub(255, 255, 255);
    setfont("helvetica", 18);
    
    if (mode == GL_LINEAR) {
        drawstr(100, 80, "end - z");
        drawstr(30, 95, "f   = ");
        drawstr(90, 110, "end - start");
        glBegin(GL_LINE_STRIP);
        glVertex2i(85, 90);
        glVertex2i(185, 90);
        glEnd();
    } else {
        drawstr(90, 80, "- (density * z)");
        drawstr(30, 95, "f   =   e");
        if (mode == GL_EXP2)
            drawstr(208, 68, "2");
    }
    
    setfont("helvetica", 12);
    drawstr(20, 180, "z is the distance in eye coordinates");
    drawstr(20, 195, "from origin to fragment being fogged.");
    
    glutSwapBuffers();
}

void
screen_reshape(int width, int height)
{
    float fog_color[4];
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width/height, 0.5, 8.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1],up[2]);
    if (clear)
        glClearColor(color[0].value, color[1].value, 
        color[2].value, color[3].value);
    else
        glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, mode);
    glFogf(GL_FOG_START, fstart.value);
    glFogf(GL_FOG_END, fend.value);
    glFogf(GL_FOG_DENSITY, density.value);
    
    cell_vector(fog_color, color, 4);
    glFogfv(GL_FOG_COLOR, fog_color);
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
    
    glColor3ub(255, 255, 255);
    setfont("helvetica", 18);
    drawstr(20, color[0].y, "GLfloat color[4] = {");
    drawstr(230, color[0].y, ",");
    drawstr(290, color[0].y, ","); 
    drawstr(350, color[0].y, ",");
    drawstr(410, color[0].y, "};");
    drawstr(20, 80, "glFogfv(GL_FOG_COLOR, color);");
    if (mode == GL_LINEAR) {
        drawstr(20, fstart.y, "glFogf(GL_FOG_START, ");
        drawstr(290, fstart.y, ");");
        drawstr(20, fend.y, "glFogf(GL_FOG_END, ");
        drawstr(290, fend.y, ");");
    } else {
        drawstr(20, density.y, "glFogf(GL_FOG_DENSITY, ");
        drawstr(310, density.y, ");");
    }
    drawstr(20, 200, "glFogi(GL_FOG_MODE, %s);", 
        mode == GL_LINEAR ? "GL_LINEAR" : 
    mode == GL_EXP ? "GL_EXP" : "GL_EXP2");
    
    cell_draw(&color[0]);
    cell_draw(&color[1]);
    cell_draw(&color[2]);
    cell_draw(&color[3]);
    
    if (mode == GL_LINEAR) {
        cell_draw(&fstart);
        cell_draw(&fend);
    } else {
        cell_draw(&density);
    }
    
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
    /* mouse should only hit _one_ of the cells, so adding up all
        the hits just propagates a single hit. */
        selection += cell_hit(&color[0], x, y);
        selection += cell_hit(&color[1], x, y);
        selection += cell_hit(&color[2], x, y);
        selection += cell_hit(&color[3], x, y);
        if (mode == GL_LINEAR) {
            selection += cell_hit(&fstart, x, y);
            selection += cell_hit(&fend, x, y);
        } else {
            selection += cell_hit(&density, x, y);
        }
    }
    
    old_y = y;
    
    redisplay_all();
}

void
command_motion(int x, int y)
{
    cell_update(&color[0], old_y-y);
    cell_update(&color[1], old_y-y);
    cell_update(&color[2], old_y-y);
    cell_update(&color[3], old_y-y);
    cell_update(&fstart, old_y-y);
    cell_update(&fend, old_y-y);
    cell_update(&density, old_y-y);
    
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
    
    window = glutCreateWindow("Fog");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
    
    world = glutCreateSubWindow(window, GAP, GAP, 256, 256);
    glutReshapeFunc(world_reshape);
    glutDisplayFunc(world_display);
    glutKeyboardFunc(main_keyboard);
    
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
    glutAddMenuEntry("Fog", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("[l]  GL_LINEAR", 'l');
    glutAddMenuEntry("[e]  GL_EXP", 'e');
    glutAddMenuEntry("[x]  GL_EXP2", 'x');
    glutAddMenuEntry("[c]  Clear color toggle", 'c');
    glutAddMenuEntry("[r]  Reset parameters", 'r');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Quit", 27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    redisplay_all();
    
    glutMainLoop();
    
    return 0;
}
