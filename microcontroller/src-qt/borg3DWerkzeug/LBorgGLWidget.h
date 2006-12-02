#ifndef LBORGGLWIDGET
#define LBORGGLWIDGET

#include <QGLWidget>

#define SIZE_X 8
#define SIZE_Y 8
#define SIZE_Z 8


class LBorgGLWidget : public QGLWidget {
	Q_OBJECT	

public:
	LBorgGLWidget(QWidget *parent = 0);

	void setTestFrame(); 
	// besser *image und dann mit nur einer Schleife die Daten in viererSchritten kopoieren
	void setFrame(unsigned image[SIZE_X][SIZE_Y][SIZE_Z]);

protected:	
	void mouseMoveEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	
	void tbInit(int button);
	void tbReshape(int width, int height);
	void tbMouse(int button, int state, int x, int y);
	void tbMatrix();
	void tbMotion(int x, int y);
	void tbAnimate(bool animate);
	void _tbPointToVector(int x, int y, int width, int height, float v[3]);
	void _tbStartMotion(int x, int y, int button, unsigned int time);
	void _tbStopMotion(int button, unsigned int time);

private:
	void drawLED(int color, float pos_x, float pos_y, float pos_z);
 
	unsigned char image[SIZE_X][SIZE_Y][SIZE_Z];

	// rotations variables for keyboardrotation
	float view_rotx, view_roty, view_rotz;
	
	// its needed to draw spheres
	GLUquadric* quad;
	
	// properties for the trackball functionality
	unsigned int tb_lasttime;
	float        tb_lastposition[3];

	float        tb_angle;
	float        tb_axis[3];
	float        tb_transform[4][4];

	int          tb_width;
	int          tb_height;

	int          tb_button;
	bool	     tb_tracking;
	bool	     tb_animate;
	
	bool		 mouse_drag;
};

#endif // LBORGGLWIDGET
