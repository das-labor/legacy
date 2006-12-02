#include "LBorgGLWidget.h"

#include "trackball.h"
#include <math.h>

#include <QDateTime>
#include <QMouseEvent>

#ifdef Q_OS_MACX
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>  
#endif

 
LBorgGLWidget::LBorgGLWidget(QWidget *parent) : QGLWidget(parent) {
	setTestFrame();
	setMouseTracking(true);
	mouse_drag  = false;
	// init trackball properties
	tbInit(1);
	tb_angle    =  0.0;
	tb_button   = -1;
	tb_tracking = false;
	tb_animate  = true;
}

void LBorgGLWidget::mouseMoveEvent(QMouseEvent * e) {
	if (mouse_drag) {
		tbMotion(e->x(), e->y());
		updateGL();
	}
}

void LBorgGLWidget::mousePressEvent(QMouseEvent * e) {
	mouse_drag = true;
	tbMotion(e->x(), e->y());
	//_tbStartMotion(e->x(), e->y(), 1, (QDateTime::currentDateTime()).toTime_t());
	//updateGL();
}

void LBorgGLWidget::mouseReleaseEvent(QMouseEvent * e) {
	mouse_drag = false;
	_tbStopMotion(1, (QDateTime::currentDateTime()).toTime_t());
	updateGL();
}

void LBorgGLWidget::initializeGL() {
	view_rotx = 0.;
	view_roty = 0.;
	view_rotz = 0.;

	glClearColor(0.1, 0.1, 0.1, 0.1);
    gluPerspective(60.0, (float) width() / (float) height(), 5., 1000.);
	
    gluLookAt(SIZE_X*2.,   SIZE_Y*2.+50., SIZE_Z*2.,
              SIZE_X*2.,   SIZE_Y*2.,     SIZE_Z*2.,
              0.0,         0.0,           1.0); 

	// init Call List for LED. The List number is
	// the brightnessnumber of the LED	
	quad = gluNewQuadric();
	glNewList(0, GL_COMPILE);
		glColor4f(0.30, 0., 0., 1.);
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
	
   // init trackball
   tbInit(1);
   tbAnimate(GL_FALSE);
   resize(600, 600);
}

void LBorgGLWidget::resizeGL(int width, int height) {
  tbReshape(width, height);

  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float) width / (float) height, 5., 1000.);
  gluLookAt(SIZE_X*2.,    SIZE_Y*2.+50., SIZE_Z*2.,
            SIZE_X*2.,    SIZE_Y*2.,     SIZE_Z*2.,
            0.0,          0.0,           1.0); 
  glMatrixMode(GL_MODELVIEW);	
  glLoadIdentity();
}

void LBorgGLWidget::drawLED(int color, float pos_x, float pos_y, float pos_z) {
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	glCallList(color);
	glPopMatrix();
}

void LBorgGLWidget::paintGL() {
	int x, y, z;
  	tbReshape(width(), height());
  	glClear(GL_COLOR_BUFFER_BIT);
  	glPushMatrix();
	glTranslatef(SIZE_X*2., SIZE_Y*2., SIZE_Z*2.);
	
	//tbMatrix(); // Adds the rotationspart of the trackball 
  	glRotatef(view_rotx, 1.0, 0.0, 0.0);
  	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
	glTranslatef(-SIZE_X*2., -SIZE_Y*2., -SIZE_Z*2.);
  	for (x = 0; x < SIZE_X; x++) {
		for (y = 0; y < SIZE_Y; y++) { 
			for (z = 0; z < SIZE_Z; z++) {
				// display the LEDs in the original size raster
				// in the real one there is also 4 cm beetween the LEDs
				drawLED(image[x][y][z], (float) x*4., (float) y*4., (float) z*4.);
			}
		}
  	}
	glPopMatrix();
}

void LBorgGLWidget::setTestFrame() {
	int x, y, z;

	for (x = 0; x < SIZE_X; x++) {
		for (y = 0; y < SIZE_Y; y++) { 
			for (z = 0; z < SIZE_Z; z++) {
				this->image[x][y][z] = (x + y + z) % 4;
			}
		}
	}
	update();
}

void LBorgGLWidget::setFrame(unsigned image[SIZE_X][SIZE_Y][SIZE_Z]) {
	int x, y, z;

	for (x = 0; x < SIZE_X; x++) {
		for (y = 0; y < SIZE_Y; y++) { 
			for (z = 0; z < SIZE_Z; z++) {
				this->image[x][y][z] = image[x][y][z];
			}
		}
	}
	update();
}

void LBorgGLWidget::_tbPointToVector(int x, int y, int width, int height, float v[3]) {
  float d, a;

  /* project x, y onto a hemi-sphere centered within width, height. */
  v[0] = (2.0 * x - width) / width;
  v[1] = (height - 2.0 * y) / height;
  d = sqrt(v[0] * v[0] + v[1] * v[1]);
  v[2] = cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0));
  a = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}

void LBorgGLWidget::_tbStartMotion(int x, int y, int button, unsigned int time) {
  tb_tracking = GL_TRUE;
  tb_lasttime = time;
  _tbPointToVector(x, y, tb_width, tb_height, tb_lastposition);
}

void LBorgGLWidget::_tbStopMotion(int button, unsigned int time) {
  tb_tracking = GL_FALSE;
/*  GLUT specific
  if (time == tb_lasttime && tb_animate) {
    glutIdleFunc(_tbAnimate);
  } else {
    tb_angle = 0.0;
    if (tb_animate)
      glutIdleFunc(0);
  */
  
}

void LBorgGLWidget::tbAnimate(bool animate) {
  tb_animate = animate;
}

void LBorgGLWidget::tbInit(int button) {
  tb_button = button;
  tb_angle = 0.0;

  /* put the identity in the trackball transform */
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, (float *) tb_transform);
  glPopMatrix();
}

void LBorgGLWidget::tbMatrix()
{
  glPushMatrix();
  glLoadIdentity();
  glRotatef(tb_angle, -tb_axis[0], tb_axis[2], tb_axis[1]);
  glMultMatrixf((float *)tb_transform);
  glGetFloatv(GL_MODELVIEW_MATRIX, (float *) tb_transform);
  glPopMatrix();

  glMultMatrixf((float *) tb_transform);
}

void LBorgGLWidget::tbReshape(int width, int height) {
  tb_width  = width;
  tb_height = height;
}

void LBorgGLWidget::tbMouse(int button, int state, int x, int y) {
  //if (state == GLUT_DOWN && button == tb_button)
    //_tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
  //else if (state == GLUT_UP && button == tb_button)
    //_tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
}

void LBorgGLWidget::tbMotion(int x, int y) {
  float current_position[3], dx, dy, dz;
  
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
  tb_axis[0] = tb_lastposition[1] * current_position[2] - 
               tb_lastposition[2] * current_position[1];
  tb_axis[1] = tb_lastposition[2] * current_position[0] - 
               tb_lastposition[0] * current_position[2];
  tb_axis[2] = tb_lastposition[0] * current_position[1] - 
               tb_lastposition[1] * current_position[0];

  /* reset for next time */
  tb_lasttime = (QDateTime::currentDateTime()).toTime_t();
  tb_lastposition[0] = current_position[0];
  tb_lastposition[1] = current_position[1];
  tb_lastposition[2] = current_position[2];
}
