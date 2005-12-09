#ifndef CONTROLLPOINT_H
#define CONTROLLPOINT_H

#include <QObject>

#include "display.h"

typedef struct {
	float x;
	float y;
} Point;

class ControllPoint : public QObject{
public:
	ControllPoint(Point p, int lineNo, int firstElementNo);	
	
	int getLine();
	int getElement();
	Point getPoint();
	
private: 
	Point p;

	int lineNo;
	int elementNo;
};

#endif // CONTROLLPOINT_H
