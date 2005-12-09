#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QList>

#include "controllPoint.h"

class DrawArea : public QWidget {
	Q_OBJECT

public:
	DrawArea(QTextEdit *textedit, QWidget *parent);
	~DrawArea();
	void setZoom(float zoom);
	float getZoom();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	

public slots:
	void checkAndDraw();

private:
	void drawLineTo(Point p, QPainter *g, int lineNo);
	void drawBezier(Point p2, Point p3, Point p4, QPainter *g, int lineNo);
	void drawBezierRec(Point p1, Point p2, Point p3, Point p4, int level, QPainter *g);
	Point midpoint(Point p1, Point p2);
	void getChainCode();
	void chainLineTo(Point p);
	void chainBezier(Point p2, Point p3, Point p4);
	void chainBezierRec(Point p1, Point p2, Point p3, Point p4, int level);
	void startChain(int px, int py);
	void addToChain(int px, int py);
	void addControlPoint(Point p, int line, int firstElement, QPainter *g);
	void deleteControlPoints();

	QList<ControllPoint*> controllPoints;
	QString chain;
	QStringList list;
	int currentAngle;
  	int height, width;
	float zoom;
	int chainPosX;
	int chainPosY;
	int drawLevel;
	int chainLevel;
	Point CurrentPoint;
	QTextEdit *text;
};

#endif // DISPLAY_H
