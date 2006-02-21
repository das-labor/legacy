#include "controllPoint.h"

ControllPoint::ControllPoint(Point p, int lineNo, int firstElementNo, bool hasAngle, float angle) {
	this->p = p;
	this->lineNo = lineNo;
	this->elementNo = firstElementNo;
	this->hasAngle = hasAngle;
	this->angle = angle;
}

int ControllPoint::getLine() {
	return lineNo;
}

int ControllPoint::getElement() {
	return elementNo;
}

Point ControllPoint::getPoint() {
	return p;
}

bool ControllPoint::isAngle() {
	return angle;
}

float ControllPoint::getAngle() {
	return p.x;
}
