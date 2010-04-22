#ifdef _WIN32
	#include <GL/glut.h>
	#include <windows.h>
	#include <process.h>
#define pthread_t int
#else
	#define _XOPEN_SOURCE 500
	#ifdef OSX_
		#include <GLUT/glut.h>
	#else
		#include <GL/glut.h>
	#endif
	#include <pthread.h>   // for threads in linux
	#include <stdlib.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

#include <stdio.h>
#include <setjmp.h>

#include "../config.h"
#include "../display_loop.h"
#include "../pixel.h"
#include "trackball.h"

unsigned char fakeport;
volatile unsigned char oldMode, oldOldmode, mode;
extern unsigned char waitForFire;


int WindWidth, WindHeight;

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];
unsigned char joystick;

float view_rotx = 0, view_roty = 0, view_rotz = 0;
int win;


pthread_t simthread;
GLUquadric* quad;

void drawLED(int color, float pos_x, float pos_y, float pos_z) {
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	glCallList(color);
	glPopMatrix();
}

void display(void){
  	int x, y, z, level, color;
  	tbReshape(WindWidth, WindHeight);
  	glClear(GL_COLOR_BUFFER_BIT);
  	glPushMatrix();
	glTranslatef(NUM_COLS*2., 0., NUM_ROWS*2.);
	tbMatrix();
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
	glTranslatef(-NUM_COLS*2, 0., -NUM_ROWS*2.);
  	for (x = 0; x < 1; x++) {
		for (y = 0; y < NUM_COLS; y++) {
			for (z = 0; z < NUM_ROWS; z++) {
				color = 0;
				for (level = 0; level < NUMPLANE; level++) {
					if (pixmap[level][z%NUM_ROWS][y/8] & (1 << y % 8)) {
						color = level+1;
					}
				}
				drawLED(color, (float)y*4.0,
				               (float)x*4.0,
							   (float)(NUM_ROWS-1-z)*4.0);
			}
		}
  	}
	glPopMatrix();
	glutSwapBuffers();
#ifdef _WIN32
	Sleep(10);
#else
	usleep(20000);
#endif
    joystick = 255;
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 'q': printf("Quit\n");
    		glutDestroyWindow(win);
    		exit(0);
			break;
		case ' ':
			fakeport |= 0x01;
			break;
		case 'a':
			fakeport |= 0x02;
			break;
		case 'd':
			fakeport |= 0x04;
			break;
		case 's':
			fakeport |= 0x08;
			break;
		case 'w':
			fakeport |= 0x10;
			break;
	}
}

void keyboardup(unsigned char key, int x, int y){
	switch (key) {
		case ' ':
			fakeport &= ~0x01;
			break;
		case 'a':
			fakeport &= ~0x02;
			break;
		case 'd':
			fakeport &= ~0x04;
			break;
		case 's':
			fakeport &= ~0x08;
			break;
		case 'w':
			fakeport &= ~0x10;
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
  tbMouse(button, state, x, y);
}

void motion(int x, int y)
{
  tbMotion(x, y);
}

void reshape(int width, int height)
{

  tbReshape(width, height);

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float)WindWidth/(float)WindWidth, 5., 1000.);
  gluLookAt(NUM_ROWS*2., NUM_ROWS*2.+50., NUM_COLS*2.,
            NUM_ROWS*2., NUM_ROWS*2., NUM_COLS*2.,
            0.0, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  WindWidth = width;
  WindHeight = height;
}

/* change view angle */
static void special(int k, int x, int y) {
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_rotz += 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_rotz -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}
/*
void timf(int value) {
  glutPostRedisplay();
  glutTimerFunc(1, timf, 0);
}*/

void *display_loop_run(void * unused) {
	display_loop();
	return 0;
}

int main(int argc, char **argv){
    WindHeight = 700;
    WindWidth = 700;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WindHeight, WindWidth);
    win = glutCreateWindow("16x16 Borg Simulator");

    // callback
    //glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // clearcolor & main loop
    glClearColor(0,0,0,1.0);
    gluPerspective(60.0, (float)WindWidth/(float)WindWidth, 5., 1000.);
    gluLookAt(NUM_COLS*2., NUM_COLS*2.+50., NUM_ROWS*2.,
              NUM_COLS*2., NUM_COLS*2., NUM_ROWS*2.,
            0.0, 0.0, 1.0);

	// init Call List for LED
	quad = gluNewQuadric();
	glNewList(0, GL_COMPILE);
		glColor4f(0.8, 0.0, 0.0, 1.);
		gluSphere(quad, 1.0, 12, 12);
	glEndList();
	glNewList(1, GL_COMPILE);
		glColor4f(0.5, 0.0, 0.0, 1.);
		gluSphere(quad, 1.4, 12, 12);
    glEndList();
	glNewList(2, GL_COMPILE);
		glColor4f(0.7, 0.0, 0.0, 1.);
		gluSphere(quad, 1.55, 12, 12);
    glEndList();
	glNewList(3, GL_COMPILE);
		glColor4f(1.00, 0.0, 0.0, 1.);
	    gluSphere(quad, 1.7, 12, 12);
    glEndList();

   tbInit(GLUT_LEFT_BUTTON);
   tbAnimate(GL_FALSE);

	// start display_loop thread
#ifdef _WIN32
    _beginthread((void (*)(void*))display_loop_run, 0, NULL);
#else
    pthread_create(&simthread, NULL, display_loop_run, NULL);
#endif
    //glutTimerFunc(40, timf, 0); // Set up timer for 40ms, about 25 fps
    glutMainLoop();
    return 0;
}
