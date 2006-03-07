#include "controllPoint.h"

ControllPoint::ControllPoint(Point p, int lineNo, int firstElementNo, float *inverse, 
	              bool hasAngle, float angle) {
	this->p = p;
	this->lineNo = lineNo;
	this->elementNo = firstElementNo;
	this->hasAngle = hasAngle;
	this->angle = angle;
	setInverse(inverse);
}

int ControllPoint::getLine() {
	return lineNo;
}

int ControllPoint::getElement() {
	return elementNo;
}

void ControllPoint::setInverse(float *matrix) {
	for (int i = 0; i < 9; i++) {
		inverse[i] = matrix[i]; 
	}
}

float* ControllPoint::getInverse() {
	return inverse;
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
