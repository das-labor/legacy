#include <GL/glut.h>   // for gl* glu* glut*
#include <pthread.h>   // for threads in linux
#include "pixel3d.h"
#include "programm.h"


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
  	int x, y, z;
  	glClear(GL_COLOR_BUFFER_BIT);
  	glPushMatrix();
	glTranslatef(BORGSIZE*2., BORGSIZE*2., BORGSIZE*2.);
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_roty, 0.0, 0.0, 1.0);
	glTranslatef(-BORGSIZE*2., -BORGSIZE*2., -BORGSIZE*2.);
  	for (x = 0; x < BORGSIZE; x++) {
		for (y = 0; y < BORGSIZE; y++) { 
			for (z = 0; z < BORGSIZE; z++) {
				drawLED(data[x+BORGSIZE*y+BORGSIZE*BORGSIZE*z],
						(float)x*4.0, (float)y*4.0, (float)z*4.);
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
  for (i = 0; i < BORGLEDS; i++) {
	data[i] = 0;
  }
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
  gluLookAt(BORGSIZE*2., BORGSIZE*2.+50., BORGSIZE*2.,
            BORGSIZE*2., BORGSIZE*2., BORGSIZE*2.,
            0.0, 0.0, 1.0); 
  // start display_loop thread
  pthread_create(&simthread, NULL, display_loop, NULL);
  printf("Starting MainLoop\n");
  quad = gluNewQuadric();
  glutMainLoop();
  return 0;
}
