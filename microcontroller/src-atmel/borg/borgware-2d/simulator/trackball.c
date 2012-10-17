/**
 * \addtogroup unixsimulator
 */
/*@{*/

/**
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 *
 *
 *  Usage:
 *
 *  o  call tbInit() in before any other tb call
 *  o  call tbReshape() from the reshape callback
 *  o  call tbMatrix() to get the trackball matrix rotation
 *  o  call tbStartMotion() to begin trackball movememt
 *  o  call tbStopMotion() to stop trackball movememt
 *  o  call tbMotion() from the motion callback
 *  o  call tbAnimate(GL_TRUE) if you want the trackball to continue
 *     spinning after the mouse button has been released
 *  o  call tbAnimate(GL_FALSE) if you want the trackball to stop
 *     spinning after the mouse button has been released
 *
 *  Typical setup:
 *
 *
 *  void
 *  init(void)
 *  {
 *    tbInit(GLUT_MIDDLE_BUTTON);
 *    tbAnimate(GL_TRUE);
 *    . . .
 *  }
 *
 *  void
 *  reshape(int width, int height)
 *  {
 *    tbReshape(width, height);
 *    . . .
 *  }
 *
 *  void
 *  display(void)
 *  {
 *    glPushMatrix();
 *
 *    tbMatrix();
 *    . . . draw the scene . . .
 *
 *    glPopMatrix();
 *  }
 *
 *  void
 *  mouse(int button, int state, int x, int y)
 *  {
 *    tbMouse(button, state, x, y);
 *    . . .
 *  }
 *
 *  void
 *  motion(int x, int y)
 *  {
 *    tbMotion(x, y);
 *    . . .
 *  }
 *
 *  int
 *  main(int argc, char** argv)
 *  {
 *    . . .
 *    init();
 *    glutReshapeFunc(reshape);
 *    glutDisplayFunc(display);
 *    glutMouseFunc(mouse);
 *    glutMotionFunc(motion);
 *    . . .
 *  }
 *
 * @file trackball.c
 * @brief Helper functions for the UNIX platform Borg simulator.
 * @author Martin Ongsiek, David Yu, David Blythe
 */

/* includes */
#include <math.h>
#include <assert.h>
#ifdef OSX_
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include "trackball.h"


/* globals */
static GLuint    tb_lasttime;
static GLfloat   tb_lastposition[3];

static GLfloat   tb_angle = 0.0;
static GLfloat   tb_axis[3];
static GLfloat   tb_transform[4][4];

static GLuint    tb_width;
static GLuint    tb_height;

static GLint     tb_button = -1;
static GLboolean tb_tracking = GL_FALSE;
static GLboolean tb_animate = GL_TRUE;


/* functions */

/**
 * Project x and y onto a hemisphere centered within given width and height.
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param width Width of the hemisphere.
 * @param height Width of the hemisphere.
 * @param v Vector where the projection is performed on.
 */
static void _tbPointToVector(int x, int y, int width, int height, float v[3]) {
	float d, a;

	/* project x, y onto a hemisphere centered within width, height. */
	v[0] = (2.0 * x - width) / width;
	v[1] = (height - 2.0 * y) / height;
	d = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[2] = cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0));
	a = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}


/**
 *  Redisplay current window contents.
 */
static void _tbAnimate(void) {
	glutPostRedisplay();
}


/**
 * Starts trackball movement depending on the mouse position.
 * @param x X-position of the mouse pointer.
 * @param y Y-position of the mouse pointer.
 * @param button Not used.
 * @param time Elapsed time.
 */
void _tbStartMotion(int x, int y, int button, int time) {
	assert(tb_button != -1);

	tb_tracking = GL_TRUE;
	tb_lasttime = time;
	_tbPointToVector(x, y, tb_width, tb_height, tb_lastposition);
}


/**
 * Stops trackball movement.
 * @param button Not used
 * @param time Not used.
 */
void _tbStopMotion(int button, unsigned time) {
	assert(tb_button != -1);

	tb_tracking = GL_FALSE;

	if (time == tb_lasttime && tb_animate) {
		glutIdleFunc(_tbAnimate);
	} else {
		tb_angle = 0.0;
		if (tb_animate) {
			glutIdleFunc(0);
		}
	}
}



/**
 * Starts or stops the spinning movement of the trackball.
 * @param animate GL_TRUE for starting and GL_FALSE for stopping the animation.
 */
void tbAnimate(GLboolean animate) {
	tb_animate = animate;
}


/**
 * Has to be called before any other tb call.
 * @param button Mouse button state.
 */
void tbInit(GLuint button) {
	tb_button = button;
	tb_angle = 0.0;

	/* put the identity in the trackball transform */
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) tb_transform);
	glPopMatrix();
}


/**
 * Gets the tb matrix rotation.
 */
void tbMatrix() {
	assert(tb_button != -1);

	glPushMatrix();
	glLoadIdentity();
	glRotatef(tb_angle, -tb_axis[0], tb_axis[2], tb_axis[1]);
	glMultMatrixf((GLfloat *) tb_transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) tb_transform);
	glPopMatrix();

	glMultMatrixf((GLfloat *) tb_transform);
}


/**
 * Reshape callback function for determining the window size.
 * @param width Width of the trackball.
 * @param height Height of the trackball.
 */
void tbReshape(int width, int height) {
	assert(tb_button != -1);

	tb_width = width;
	tb_height = height;
}


/**
 * Starts motion depending on mouse position and button state.
 * @param button The button whose state has changed.
 * @param state The state of that button.
 * @param x X-position of the mouse pointer.
 * @param y Y-position of the mouse pointer.
 */
void tbMouse(int button, int state, int x, int y) {
	assert(tb_button != -1);

	if (state == GLUT_DOWN && button == tb_button)
		_tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
	else if (state == GLUT_UP && button == tb_button)
		_tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
}


/**
 * Starts a rotating scene motion to the given coordinates.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 */
void tbMotion(int x, int y) {
	GLfloat current_position[3], dx, dy, dz;

	assert(tb_button != -1);

	if (tb_tracking == GL_FALSE)
		return;

	_tbPointToVector(x, y, tb_width, tb_height, current_position);

	/* calculate the angle to rotate by (directly proportional to the
	   length of the mouse movement */
	dx = current_position[0] - tb_lastposition[0];
	dy = current_position[1] - tb_lastposition[1];
	dz = current_position[2] - tb_lastposition[2];
	tb_angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);

	/* calculate the axis of rotation (cross product) */
	tb_axis[0] = tb_lastposition[1] * current_position[2]
	             - tb_lastposition[2] * current_position[1];
	tb_axis[1] = tb_lastposition[2] * current_position[0]
	             - tb_lastposition[0] * current_position[2];
	tb_axis[2] = tb_lastposition[0] * current_position[1]
	             - tb_lastposition[1] * current_position[0];

	/* reset for next time */
	tb_lasttime = glutGet(GLUT_ELAPSED_TIME);
	tb_lastposition[0] = current_position[0];
	tb_lastposition[1] = current_position[1];
	tb_lastposition[2] = current_position[2];

	/* remember to draw new position */
	glutPostRedisplay();
}

/*@}*/
