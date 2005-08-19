#include <GL/glut.h>   // for gl* glu* glut*
#include <pthread.h>   // for threads in linux
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "pixel3d.h"
#include "programm.h"

unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

int view_rotx = 0, view_roty = 0, view_rotz = 0;
int win;
pthread_t simthread;
GLUquadric* quad;

void drawLED(int color, float pos_x, float pos_y, float pos_z) {
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	switch (color) {
		case 1 : glColor4f(0.5, 0., 0., 1.); break;
		case 2 : glColor4f(0.7, 0., 0., 1.); break;
		case 3 : glColor4f(0.5, 0., 0., 1.); break;
		default: glColor4f(0.2, 0., 0., 1.); break;
	}
	gluCylinder(quad, 0.25, 0.25, 1.0, 6, 1);
    glTranslatef(0., 0., 1.);
	gluSphere(quad, 0.25, 8, 8);          
	glPopMatrix();
}

void display(void){
  	int x, y, z, level, color;
  	glClear(GL_COLOR_BUFFER_BIT);
  	glPushMatrix();
	glTranslatef(NUM_PLANES*2., NUM_ROWS*2., NUM_COLS*2.);
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_roty, 0.0, 0.0, 1.0);
	glTranslatef(-NUM_PLANES*2., -NUM_ROWS*2., -NUM_COLS*2.);
  	for (x = 0; x < NUM_PLANES; x++) {
		for (y = 0; y < NUM_ROWS; y++) { 
			for (z = 0; z < NUM_COLS; z++) {
				color = 0;
				for (level = 0; level < NUM_LEVELS; level++) {
					if (pixmap[level][x%NUM_PLANES][y%PLANEBYTES] 
						& (1 << z%NUM_ROWS)) {
						color = level;		
					}
				}
				drawLED(color, (float)x*4.0, (float)y*4.0, (float)z*4.);
			}
		}
  	}
	glPopMatrix();
	glutSwapBuffers();
	usleep(3500);	
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {  
		case 'q': printf("Quit\n");
    		glutDestroyWindow(win);
    		exit(0); 
			break;
	}
}

/* change view angle */
static void special(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty += 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv){
  int i;	

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  win = glutCreateWindow("Borg 3D Simulator");
  
  // callback
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
 
  // clearcolor & main loop
  glClearColor(0.1,0.1,0.1,0.1);
  gluPerspective(60.0, 800./600., 5., 1000.);
  gluLookAt(NUM_PLANES*2., NUM_ROWS*2.+50., NUM_COLS*2.,
            NUM_PLANES*2., NUM_ROWS*2., NUM_COLS*2.,
            0.0, 0.0, 1.0); 
  // start display_loop thread
  pthread_create(&simthread, NULL, display_loop, NULL);
  printf("Starting MainLoop\n");
  quad = gluNewQuadric();
  glutMainLoop();
  return 0;
}
