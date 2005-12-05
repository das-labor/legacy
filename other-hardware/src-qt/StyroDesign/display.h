#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QString>

typedef struct {
	float x;
	float y;
} Point;

class DrawArea : public QWidget {
	Q_OBJECT

public:
	DrawArea(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *event);

private:
	void drawLineTo(Point p, QPainter *g);
	void drawBezier(Point p2, Point p3, Point p4, QPainter *g);
	void drawBezierRec(Point p1, Point p2, Point p3, Point p4, int level, QPainter *g);
	Point midpoint(Point p1, Point p2);
	QString getChainCode();
	void chainLineTo(Point p);
	void chainBezier(Point p2, Point p3, Point p4);
	void chainBezierRec(Point p1, Point p2, Point p3, Point p4, int level);
	void startChain(int px, int py);
	void addToChain(int px, int py);

	QString chain;
	int currentAngle;
	float zoom;
	int chainPosX;
	int chainPosY;
	int drawLevel;
	int chainLevel;
	Point CurrentPoint;
	
};

#endif // DISPLAY_H
