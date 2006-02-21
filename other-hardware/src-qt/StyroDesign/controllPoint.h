#ifndef CONTROLLPOINT_H
#define CONTROLLPOINT_H

#include <QObject>

typedef struct {
	float x;
	float y;
} Point;

class ControllPoint : public QObject{
public:
	ControllPoint(Point p, int lineNo, int firstElementNo, bool hasAngle = false, float angle = 0.);	
	float getAngle();
	int getLine();
	int getElement();
	bool isAngle();
	Point getPoint();
	
private: 
	Point p;
    bool hasAngle;
	float angle;
	int lineNo;
	int elementNo;
};

#endif // CONTROLLPOINT_H
