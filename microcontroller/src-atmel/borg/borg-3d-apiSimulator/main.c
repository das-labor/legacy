#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

#ifdef _WIN32
#  include <windows.h>
#  include <process.h>
#  define pthread_t int
#else
#  include <pthread.h>   // for threads in linux
#  include <stdlib.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif

#include "pixel3d.h"
#include "programm.h"
#include "trackball.h"

int WindWidth, WindHeight;

unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

float view_rotx = 0, view_roty = 0, view_rotz = 0;
int win;

char joy1_up = 0, joy1_down = 0, joy1_right = 0, joy1_left = 0;

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
	glTranslatef(NUM_PLANES*2., NUM_ROWS*2., NUM_COLS*2.);
	tbMatrix();
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
	glTranslatef(-NUM_PLANES*2., -NUM_ROWS*2., -NUM_COLS*2.);
  	for (x = 0; x < NUM_PLANES; x++) {
		for (y = 0; y < NUM_ROWS; y++) { 
			for (z = 0; z < NUM_COLS; z++) {
				color = 0;
				for (level = 0; level < NUM_LEVELS; level++) {
					if (pixmap[level][x%NUM_PLANES][y%PLANEBYTES] 
						& (1 << z%NUM_ROWS)) {
						color = level+1;		
					}
				}
				drawLED(color, (float)x*4.0, (float)y*4.0, (float)z*4.);
			}
		}
  	}
	glPopMatrix();
	glutSwapBuffers();
#ifdef _WIN32
	Sleep(20);
#else
	usleep(20000);
#endif
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {  
		case 'q': printf("Quit\n");
    		glutDestroyWindow(win);
    		exit(0); 
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
  gluPerspective(60.0, (float)WindHeight/(float)WindWidth, 5., 1000.);
  gluLookAt(NUM_PLANES*2., NUM_ROWS*2.+50., NUM_COLS*2.,
            NUM_PLANES*2., NUM_ROWS*2., NUM_COLS*2.,
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
  case 'w':
	  joy1_up = 1;
	  break;
  case 's':
	  joy1_down = 1;
	  break;
  //case 'd':	
	  joy1_right = 1;
	  break;
  case 'a':
	  joy1_left = 1;
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

int main(int argc, char **argv){
    WindHeight = 700;
    WindWidth = 700;         
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WindHeight, WindWidth);
    win = glutCreateWindow("Borg 3D Simulator");
    
    // callback
    //glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    // clearcolor & main loop
    glClearColor(0.1,0.1,0.1,0.1);
    gluPerspective(60.0, (float)WindHeight/(float)WindWidth, 5., 1000.);
    gluLookAt(NUM_PLANES*2., NUM_ROWS*2.+50., NUM_COLS*2.,
              NUM_PLANES*2., NUM_ROWS*2., NUM_COLS*2.,
            0.0, 0.0, 1.0); 

	// init Call List for LED	
	quad = gluNewQuadric();
	glNewList(0, GL_COMPILE);
		glColor4f(0.20, 0., 0., 1.);
	  	gluCylinder(quad, 0.25, 0.25, 1.0, 6, 1);
		glTranslatef(0., 0., 1.);
		gluSphere(quad, 0.25, 8, 8);		
	glEndList();
	glNewList(1, GL_COMPILE);
		glColor4f(0.45, 0., 0., 1.);
		gluCylinder(quad, 0.25, 0.25, 1.0, 6, 1);
		glTranslatef(0., 0., 1.);
		gluSphere(quad, 0.25, 8, 8);
	glEndList();
	glNewList(2, GL_COMPILE);
		glColor4f(0.70, 0., 0., 1.);
		gluCylinder(quad, 0.25, 0.25, 1.0, 6, 1);
		glTranslatef(0., 0., 1.);
		gluSphere(quad, 0.25, 8, 8);
	glEndList();
	glNewList(3, GL_COMPILE);
		glColor4f(1.00, 0., 0., 1.);
		gluCylinder(quad, 0.25, 0.25, 1.0, 6, 1);
		glTranslatef(0., 0., 1.);
		gluSphere(quad, 0.25, 8, 8	);
	glEndList();
	
   tbInit(GLUT_LEFT_BUTTON);
   tbAnimate(GL_FALSE);
	
	// start display_loop thread 
#ifdef _WIN32
    _beginthread((void (*)(void*))display_loop, 0, NULL);   
#else
    pthread_create(&simthread, NULL, display_loop, NULL);
#endif
    //glutTimerFunc(40, timf, 0); // Set up timer for 40ms, about 25 fps
    glutMainLoop();
    return 0;
}
