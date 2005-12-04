#ifndef CANNON_H
#define CANNON_H

#include <QWidget>

struct Point {
	float x;
	float y;
};

class DrawArea : public QWidget {
	Q_OBJECT

public:
	DrawArea(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *event);

private:
	int currentAngle;
	float zoom;
	int chainPosX;
	int chainPosY;
	int drawLevel;
	int chainLevel;
	Point CurrentPoint;
};

#endif // CANNON_H
