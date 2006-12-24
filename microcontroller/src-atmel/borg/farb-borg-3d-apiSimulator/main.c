/** This is the main file of Simulator and includes the opengl code. 
 *  Only the trackball, for rotating the cube with the mouse is defined 
 *  "trackball.c" and "trackball.h".
 *  The program has two parts, witch are running in saperate threads.
 *
 *  1. The simulator visualisates the the imagebuffer pixmap in OpenGl. 
 *     It has the same format as the orginal one. Its compareable to the
 *     Interrupt-Service-Routine of the real one.
 *  2. display_loop() is the secound thread and it includes the animationsprogramms.
 *     actually its at the end of program.c. It normaly contains a for ever loop
 *     with all animationprograms
 */

// type make -f Makefile.osx
// to compile the simulator for MacOSX (tested on 10.2, 10.3, 10.4)
#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

#include <stdio.h>

extern void *display_loop(void * unused);


// To Compile the Simulator on Windows, you have to install DevCpp
// And also the glut.devpack for DevCpp
// You can download it there:
//   http://www.bloodshed.net/dev/devcpp.html
//   http://vis.computer.org/vis2005contest/hand/glut.3.7.6+.DevPak
// 
// please use for windows the DevCpp projectfile. _WIN32 is set there
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

//#include "pixel3d.h"
//#include "programs.h"
#include "trackball.h"
#include "config.h"

// variables for the simulator screensize
int WindWidth, WindHeight;

/** 
 *
 */
unsigned char pixmap[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];


// rotations variables for keyboardrotation
float view_rotx = 0, view_roty = 0, view_rotz = 0;
// stores the glut window 
int win;
// manage the joystick -> please look at joystick.h 
char joy1_up = 0, joy1_down = 0, joy1_right = 0, joy1_left = 0, joy_en0 = 0;

// variable for the thread
pthread_t simthread;
// its needed to draw spheres
GLUquadric* quad;


/** draws a LED to the position (pos_x, pos_y, poy_z) to the screen.
 *  It has the brightness color. 0 = off -> 3 = full on. Its done by 
 *  using glCallLists. They a defined in main() and because they were put 
 *  onto the graficcard, are much more faster.
 */
void drawLED(int color, float pos_x, float pos_y, float pos_z) {
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	gluCylinder(quad, 0.45, 0.45, 2.0, 6, 1);
	//glCallList(color);
	glPopMatrix();
}

/** This is the most importend function, because it puts the virtual LEDs 
 *  onto the screen.
 */
void display(void){
  	int x, y, z, color = 0;
	unsigned char *pix;
  	tbReshape(WindWidth, WindHeight);
  	glClear(GL_COLOR_BUFFER_BIT);
  	glPushMatrix();
	glTranslatef(MAX_X*2., MAX_Y*2., MAX_Z*2.);
	tbMatrix(); // Adds the rotationspart of the trackball 
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
	glTranslatef(-MAX_X*2., -MAX_Y*2., -MAX_Z*2.);
  	for (x = 0; x < MAX_X; x++) {
		for (y = 0; y < MAX_Y; y++) { 
			for (z = 0; z < MAX_Z; z++) {
				/*printf("%f %f %f\n", pixmap[z][y][x][0]/64.0, 
									 pixmap[z][y][x][1]/64.0, 
									 pixmap[z][y][x][2]/64.0); */
				glColor4f(pixmap[z][y][x][0]/255.0, 
						  pixmap[z][y][x][1]/255.0, 
						  pixmap[z][y][x][2]/255.0,
						  1);
				// display the LEDs in a the original size grid
				// in the real one are also 4cm beetween the LEDs
				drawLED(color, (float)x*4.0, (float)y*4.0, (float)z*4.);
			}
		}
  	}
	glPopMatrix();
	glutSwapBuffers();
	// waits. If this value is lower, only the CPU-time rises. you cant see it.
#ifdef _WIN32
	Sleep(20);
#else
	usleep(20000);
#endif
}

/** gets the joystick-keys, and the exit key.
 *
 */
void keyboard(unsigned char key, int x, int y){
	switch (key) {  
		case 'q': printf("Quit\n");
    		glutDestroyWindow(win);
    		exit(0); 
			break;
        case 'w':
    	    joy1_up++;
   	        break;
        case 's':
   	        joy1_down++;
     	    break;
        case 'd':	
   	        joy1_right++;
     	    break;
        case 'a':
     	    joy1_left++;
     	    break;
	}
}

// this is needed by the trackball
void mouse(int button, int state, int x, int y)
{
  tbMouse(button, state, x, y);
}

void motion(int x, int y)
{
  tbMotion(x, y);
}

/** if the size of the viewingarea changes, this function will be
 *  started
 */
void reshape(int width, int height)
{
  tbReshape(width, height);

  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float)WindHeight/(float)WindWidth, 5., 1000.);
  gluLookAt(MAX_X*2., MAX_Y*2.+50., MAX_Z*2.,
            MAX_X*2., MAX_Y*2.,     MAX_Z*2.,
            0.0,      0.0,          1.0); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  WindWidth  = width;
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
/* // tried it out, but it didn«t function.
void timf(int value) {
  glutPostRedisplay();
  glutTimerFunc(1, timf, 0);
}
*/

int main(int argc, char **argv) {
    // init
    WindHeight = 600;
    WindWidth  = 600;         
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WindHeight, WindWidth);
    win = glutCreateWindow("Farb Borg 3D Simulator");
    
    // callback functions
    //glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    // clearcolor & main loop
    glClearColor(0.1,0.1,0.1,0.1);
    gluPerspective(60.0, (float) WindHeight/(float) WindWidth, 5., 1000.);
    gluLookAt(MAX_X*2., MAX_Y*2.+50., MAX_Z*2.,
              MAX_X*2., MAX_Y*2.,     MAX_Z*2.,
              0.0, 0.0, 1.0); 

	// init Call List for LED. The List number is
	// the brightnessnumber of the LED	
	quad = gluNewQuadric();
	glNewList(0, GL_COMPILE);
		gluCylinder(quad, 0.45, 0.45, 2.0, 6, 1);
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
