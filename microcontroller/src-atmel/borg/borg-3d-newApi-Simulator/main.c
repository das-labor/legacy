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

#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

// To Compile the Simulator on Windows, you have to install Code::Blocks with mingw.
// http://www.codeblocks.org/
// Addionally extract the following zip archive into the MinGW Subfolder (C:\Programme\CodeBlocks\MinGW ).
// http://www.das-labor.org/files/madex/glut.3.7.6_mingw_codeblocks.zip
// And open the projectfile "win32_code_blocks.cbp". (_WIN32 is set there)

#ifdef _WIN32
#  include <windows.h>
#  include <process.h>
#  define pthread_t int
#else
#  include <pthread.h>   // for threads in linux
#  include <stdlib.h>
#  include <stdarg.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif

#include "api.h"
#include "programs.h"
#include "trackball.h"

char strInit[] = "Test";
char *animStr = strInit;
volatile unsigned int curFrame   =   0;  // counts the waits
volatile unsigned int speed      = 100;  // speed in %
volatile unsigned int speedSave  = 100;  // saved speed for pause
volatile unsigned char showKoord =   1;  // show or hide coordinate system
volatile int debug_val_1 = 0, debug_val_2 = 0, debug_val_3 = 0;

// variables for the simulator screensize
int WindWidth, WindHeight;

/** 
 *
 */
unsigned char pixmap[NUM_LEVELS][LEN_Z][LEN_Y];

// rotations variables for keyboardrotation
float view_rotx = -30., view_roty = 0., view_rotz = 210.;
// stores the glut window 
int win;
// manage the joystick -> please look at joystick.h 
char joy1_up = 0, joy1_down = 0, joy1_right = 0, joy1_left = 0, joy_en0 = 0;

// variable for the thread
pthread_t simthread;
// its needed to draw spheres
GLUquadric* quad;

// sets the name of the animation and stopps skipping
void setAnimName(char* str) {
	curFrame = 0;
	
	debug_val_1 = 0;
	
	debug_val_2 = 0;
	
	debug_val_3 = 0;
	
	if (str)
		animStr = str;
	if (speed > 1000)
		speed = 100;
}

/** draws a LED to the position (pos_x, pos_y, poy_z) to the screen.
 *  It has the brightness color. 0 = off -> 3 = full on. Its done by 
 *  using glCallLists. They a defined in main() and because they were put 
 *  onto the graficcard, are much more faster.
 */
void drawLED(int color, float pos_x, float pos_y, float pos_z) {
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	glCallList(color);
	glPopMatrix();
}

/** draws the coordinate system. red for x, green for y and blue for z-axis.
 */
void drawKoord(float dist, float size) {
	glPushMatrix();
	glTranslatef(-dist, -dist, -dist);
	glColor3f(1., 0., 0.); // red == x
	glBegin(GL_LINES);
                glVertex3f(0., 0., 0.);
                glVertex3f(size, 0., 0.);
	glEnd();
	glColor3f(0., 1., 0.); // red == x
	glBegin(GL_LINES);
                glVertex3f(0., 0., 0.);
                glVertex3f(0., size, 0.);
	glEnd();
	glColor3f(0., 0., 1.); // red == x
	glBegin(GL_LINES);
                glVertex3f(0., 0., 0.);
                glVertex3f(0., 0., size);
	glEnd();
	glPopMatrix();
	
}

/** writes text on the simulator
 */
void output(GLfloat x, GLfloat y, char *format,...)
{
	va_list args;
	char buffer[200], *p;
	
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	glPushMatrix();
	glTranslatef(x, y, 0);
	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
	glPopMatrix();
}


/** This is the most importend function, because it puts the virtual LEDs 
 *  onto the screen.
 */
void display(void){
    int x, y, z, level, color = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);


    tbReshape(WindWidth, WindHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(LEN_Z*2., LEN_Y*2., LEN_X*2.);
    tbMatrix(); // Adds the rotationspart of the trackball 
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);
    if (showKoord)
            drawKoord(LEN_X*2.+2., LEN_X+2.);	

    glTranslatef(-LEN_Z*2., -LEN_Y*2., -LEN_X*2.);
    for (x = 0; x < LEN_X; x++) {
            for (y = 0; y < LEN_Y; y++) { 
                    for (z = 0; z < LEN_Z; z++) {
                            color = 0;
                            // checks what level the LED has got.
                            for (level = 0; level < NUM_LEVELS; level++) {
                                    if (pixmap[level][z][y] & (1 << x)) {
                                            color = level+1;		
                                    }
                            }
                            // display the LEDs in a the original size raster
                            // in the real one are also 4cm beetween the LEDs
                            drawLED(color, (float)x*4.0, (float)y*4.0, (float)z*4.);
                    }
            }
    }
    glPopMatrix();
    
    // Overlay Textdarstellung
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, (WindWidth*5000)/WindHeight, 0, 5000);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1., 0., 0.);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0);
    output(100, 4800, "Animation: %s - Speed = %d %%", animStr, speed);
    output(100, 4600, "Waits: %d, x1: %d, x2: %d, x3: %d", curFrame, debug_val_1, debug_val_2, debug_val_3);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
	
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
		case 'q': 
                    printf("Quit\n");
                    glutDestroyWindow(win);
                    exit(0); 
                    break;
			
		case 'r':
                    view_rotx = -30.;
                    view_roty =   0.;
                    view_rotz = 210.;
                    tbReset();
                    break;
			
		case ' ':
                    if (speed) { // pause
                            speedSave = speed;
                            speed = 0;
                    } else { // resume
                            speed = speedSave;
                    }

                    break;
			
		case 'z':
                    if (showKoord)
                            showKoord = 0;
                    else
                            showKoord = 1;
                    break;
			
		case 't':
                    speed++;
                    break;
			
		case 'i':	
                    debug_val_1++;
                    break;
			
			
                case 'k':
                    debug_val_1--;
                    break;
			
			
		case 'x':
                    speed = 100000;
                    break;
			
		case 'g':
                    if (speed)
                        speed--;
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
	glClearColor(0.1,0.1,0.1,0.1);
	gluPerspective(60.0, (float) width / (float) height, 5., 1000.);
	gluLookAt(LEN_X*2., LEN_Y*2.+65., LEN_Z*2.,
	          LEN_X*2., LEN_Y*2.,     LEN_Z*2.,
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

int main(int argc, char **argv) {
    // init
    WindHeight = 600;
    WindWidth  = 600;         
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WindHeight, WindWidth);
    win = glutCreateWindow("Borg 3D 8^3 Simulator (new api)");
     
    // callback functions
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    // clearcolor & main loop
    glClearColor(0.1,0.1,0.1,0.1);
    gluPerspective(60.0, 1.0, 5., 1000.);
    gluLookAt(LEN_Z*2., LEN_Y*2.+65., LEN_X*2.,
              LEN_Z*2., LEN_Y*2., LEN_X*2.,
              0.0, 0.0, 1.0); 

    // init Call List for LED. The List number is
    // the brightnessnumber of the LED	
    quad = gluNewQuadric();
    glNewList(0, GL_COMPILE);
            glColor4f(0.30, 0., 0. , 1.);
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
