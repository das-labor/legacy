#include "controllPoint.h"

ControllPoint::ControllPoint(Point p, int lineNo, int firstElementNo) {
	this->p = p;
	this->lineNo = lineNo;
	this->elementNo = firstElementNo;
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
