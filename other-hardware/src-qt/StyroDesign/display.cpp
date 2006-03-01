#include <QPainter>
#include <QStringListIterator>
#include <QChar>
#include <iostream>
#include <QMouseEvent>
#include <math.h>

#define DRAG_START 1
#define DRAG_END 2
#define DRAG_IMAGE 3

#define PI 3.1415926535897932384626433832795
 
using namespace std;

#include "display.h"

DrawArea::DrawArea(QTextEdit *textedit, QWidget *parent)
	: QWidget(parent)
{
	width  = 2000;
	height = 2000;
	scale = 10.0;
	currentAngle = 45;
	text = textedit;
	drawLevel = 7;
	chainLevel = 10;
	setPalette(QPalette(QColor(255, 255, 255)));
	setZoom(1.0);
	setFocusPolicy(Qt::WheelFocus);
	drag = false;
	imageScaleChanged = false;
	showControlElements = true;
	dragImage = 0;
	dragAbort = false;
	connect(textedit, SIGNAL(textChanged()), this, SLOT(checkAndDraw()));
}

DrawArea::~DrawArea() {
}

void DrawArea::paintEvent(QPaintEvent * /* event */)
{
	int j = 0;
	QPainter painter(this);
	painter.setBrush(Qt::black);

	
	if (!image.isNull()) {
		if (imageScaleChanged) {
			imageBuf = image.scaled((int)(zoom*(imageEnd.x-imageBegin.x)+0.5),
									(int)(zoom*(imageEnd.y-imageBegin.y)+0.5));
			imageScaleChanged = false;
		}
		
		painter.drawImage((int)(zoom*imageBegin.x+0.5), 
			    		  (int)(zoom*imageBegin.y+0.5), imageBuf);
		if (showControlElements) {
			painter.setBrush(Qt::green);
			painter.drawRect((int)(zoom*imageBegin.x+0.5)-2,(int)(zoom*imageBegin.y+0.5)-2, 4, 4);
			painter.drawRect((int)(zoom*imageEnd.x+0.5)-2,  (int)(zoom*imageEnd.y+0.5)-2, 4, 4);
			painter.setBrush(Qt::black);
		}
	}
	
	QStringListIterator i(list);
	identety();
	while (i.hasNext())  {
		QStringList ps = i.next().split(' ');
		QString compare = ps.at(0);
		if ((compare == "start" || compare == "s") && ps.size() >= 3) {
			deleteControlPoints();
			Point P = transform((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()});
			CurrentPoint.x = P.x;
			CurrentPoint.y = P.y;
			addControlPoint(CurrentPoint, j, 1, &painter);
		} else if ((compare == "line" || compare == "l") && ps.size() >= 3) {
			drawLineTo(transform((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()}), 
			           &painter, j);
		} else if ((compare == "curve" || compare == "b") && ps.size() >= 7) {
			drawBezier(transform((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()}),
					   transform((Point) {ps.at(3).toFloat(), ps.at(4).toFloat()}),
					   transform((Point) {ps.at(5).toFloat(), ps.at(6).toFloat()}),
					   &painter, j);
		} else if ((compare == "circle" || compare == "c") && ps.size() >= 4) {
			drawCircle(transform((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()}),
					   ps.at(3).toFloat(), &painter, j);
		} else if ((compare == "scale" || compare == "sc") && ps.size() >= 5) {
			scaleP(ps.at(1).toFloat(), ps.at(2).toFloat(),
				   (Point) {ps.at(3).toFloat(), ps.at(4).toFloat()});
		} else if ((compare == "translate" || compare == "t") && ps.size() >= 3) {
			translate((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()});
		} else if ((compare == "rotate" || compare == "r") && ps.size() >= 4) {
			rotateP(ps.at(3).toFloat(), (Point) {ps.at(1).toFloat(), ps.at(2).toFloat()});
		} else if ((compare == "identety" || compare == "i") && ps.size() >= 1) {
			identety();
		} else if ((compare == "size" || compare == "si") && ps.size() >= 3) {
			height = ps.at(1).toInt();
			width =  ps.at(2).toInt();
			if (height <= 0) 
				height = 1;
			if (width <= 0) 
				width = 1;
			resize((int)(zoom*height), (int)(zoom*width));
			imageScaleChanged = true;
		}
		j++;
	}
}

void DrawArea::setZoom(float zoom) {
	this->zoom = zoom;
	resize((int)(zoom*height), (int)(zoom*width));
	imageScaleChanged = true;
}

float DrawArea::getZoom() {
	return zoom;
}


void DrawArea::checkAndDraw() {
	QString textStr = text->toPlainText();
	list = textStr.split("\n");
	repaint();
}

void DrawArea::drawLineTo(Point p, QPainter *g, int lineNo) {
	g->drawLine((int)(zoom*CurrentPoint.x+0.5), 
			    (int)(zoom*CurrentPoint.y+0.5),
			    (int)(zoom*p.x+0.5), 
		   	    (int)(zoom*p.y+0.5));
	if (lineNo != -1) {	
		addControlPoint(p, lineNo, 1, g);
	}
	CurrentPoint = p;
}

void DrawArea::drawBezier(Point p2, Point p3, Point p4, QPainter *g, int lineNo) {
	Point help = CurrentPoint;
	drawBezierRec(CurrentPoint, p2, p3, p4, drawLevel, g);
	
	if (showControlElements) {
		g->setPen(Qt::red);
		CurrentPoint = help;
		drawLineTo(p2, g, -1);
		CurrentPoint = p3;
		drawLineTo(p4, g, -1);
		g->setPen(Qt::black);
	}

	addControlPoint(p2, lineNo, 1, g);
	addControlPoint(p3, lineNo, 3, g);
	addControlPoint(p4, lineNo, 5, g);
	CurrentPoint = p4;
}

void DrawArea::drawCircle(Point p2, float angle, QPainter *g, int lineNo) {
    float curAngle, addAngle, oldAngle = 0.0, helpAngle;
    float radius = sqrt((p2.x-CurrentPoint.x)*(p2.x-CurrentPoint.x) + 
	                    (p2.y-CurrentPoint.y)*(p2.y-CurrentPoint.y)), radius2;
    Point n1, n2, n3;
    float n1e, n2e, n3e;
    radius2 = zoom * zoom * radius * radius;
    int x, y;
    
	angle = fmodf(angle, 720.);
	
	oldAngle = fmodf(atan2(p2.y - CurrentPoint.y, p2.x - CurrentPoint.x)*180./PI + 360., 360.);
		
    // center 
	addControlPoint(p2, lineNo, 1, g);
	
	x = (int) (zoom*CurrentPoint.x + 0.5);
	y = (int) (zoom*CurrentPoint.y + 0.5);
	
    while (1) {
        curAngle = fmod(atan2((float)((int) (zoom*p2.y + 0.5) - y), 
							  (float)((int) (zoom*p2.x + 0.5) - x))*180./PI + 360., 360.);
		if (((helpAngle = curAngle - oldAngle)) < -180.)
			addAngle += helpAngle + 360.;
		else if (helpAngle > 180.)
			addAngle += helpAngle - 360.;
		else 
			addAngle += helpAngle;

        switch ((int) (curAngle/45.) % 8) {
            case 7:
            case 0:
                 n1.x = -1;
                 n1.y = -1;
                 n2.x =  0;
                 n2.y = -1;
                 n3.x =  1;
                 n3.y = -1;        
                 break;
            case 1:
            case 2:
                 n1.x =  1;
                 n1.y =  1;
                 n2.x =  1;
                 n2.y =  0;
                 n3.x =  1;
                 n3.y = -1;
                 break;
            case 3: 
            case 4: 
                 n1.x = -1;
                 n1.y =  1;
                 n2.x =  0;
                 n2.y =  1;
                 n3.x =  1;
                 n3.y =  1;
                 break;
            case 5:
            case 6:
                 n1.x = -1;
                 n1.y =  1;
                 n2.x = -1;
                 n2.y =  0;
                 n3.x = -1;
                 n3.y = -1;
                 break;
        }  
        if (angle < 0.0) {
             n1.x = -n1.x;
             n1.y = -n1.y;
             n2.x = -n2.x;
             n2.y = -n2.y;
             n3.x = -n3.x;
             n3.y = -n3.y;             
        }
        n1e =  fabs((zoom*p2.x-(n1.x+(float) x))*(zoom*p2.x-(n1.x+(float) x)) + 
                    (zoom*p2.y-(n1.y+(float) y))*(zoom*p2.y-(n1.y+(float) y)) - radius2);
        n2e =  fabs((zoom*p2.x-(n2.x+(float) x))*(zoom*p2.x-(n2.x+(float) x)) +  
                    (zoom*p2.y-(n2.y+(float) y))*(zoom*p2.y-(n2.y+(float) y)) - radius2);
        n3e =  fabs((zoom*p2.x-(n3.x+(float) x))*(zoom*p2.x-(n3.x+(float) x)) + 
                    (zoom*p2.y-(n3.y+(float) y))*(zoom*p2.y-(n3.y+(float) y)) - radius2);
        if (n1e < n2e) {
           if (n3e < n1e) {
              x += (int) n3.x;
              y += (int) n3.y;
           } else {
              x += (int) n1.x;
              y += (int) n1.y;
           }
        } else {
           if (n3e < n2e) {
              x += (int) n3.x;
              y += (int) n3.y;
           } else {
              x += (int) n2.x;
              y += (int) n2.y;
           }
        }
		g->drawPoint(x, y);
		if (fabs(addAngle) >= fabs(angle))
           break;
		oldAngle = curAngle;
    } 	
	CurrentPoint.x = x/zoom;
	CurrentPoint.y = y/zoom;
	addControlPoint(CurrentPoint, lineNo, 3, g, true, angle);
}

void DrawArea::drawBezierRec(Point p1, Point p2, Point p3, Point p4, int level, QPainter *g) {
	if (level == 0) {
		drawLineTo(p4, g, -1);
	} else {
		Point l1 = p1;
		Point l2 = midpoint(p1, p2);
		Point h  = midpoint(p2, p3);
		Point r3 = midpoint(p3, p4);
		Point r4 = p4;
		Point l3 = midpoint(l2, h);
		Point r2 = midpoint(r3, h);
		Point l4 = midpoint(l3, r2);
		Point r1 = l4;
		drawBezierRec(l1, l2, l3, l4, level-1, g);
		drawBezierRec(r1, r2, r3, r4, level-1, g);
	}
}

void DrawArea::addControlPoint(Point p, int line, int firstElement, QPainter *g, bool hasAngle, float angle) {
	controllPoints.append(new ControllPoint(p, line, firstElement, hasAngle, angle));
	if (showControlElements) {
		g->setBrush(Qt::green);
		g->drawRect((int)(zoom*p.x + 0.5) - 2, (int)(zoom*p.y + 0.5) - 2, 4, 4);
		g->setBrush(Qt::black);
	}
}

void DrawArea::deleteControlPoints() {
	while (!controllPoints.isEmpty())
        delete controllPoints.takeFirst();
}

Point DrawArea::midpoint(Point p1, Point p2) {
	return (Point) {(p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0};
}	

QString DrawArea::getChainCode() {
	QStringListIterator i(list);
	
	while (i.hasNext())  {
		QStringList ps = i.next().split(' ');
		switch ((ps.at(0).toAscii())[0]) {
			case 's': {
				if (ps.size() >= 3) {
					CurrentPoint.x = ps.at(1).toFloat()*scale;
					CurrentPoint.y = ps.at(2).toFloat()*scale;
					startChain((int)(CurrentPoint.x), (int)(CurrentPoint.y));
				}
				break;
			} 
			case 'l': {
				if (ps.size() >= 3) {
					chainLineTo((Point) {ps.at(1).toFloat()*scale, ps.at(2).toFloat()*scale});
				}
				break;
			}
			case 'c': {
				if (ps.size() >= 7) {
					chainBezier((Point) {ps.at(1).toFloat()*scale, ps.at(2).toFloat()*scale},
								(Point) {ps.at(3).toFloat()*scale, ps.at(4).toFloat()*scale},
								(Point) {ps.at(5).toFloat()*scale, ps.at(6).toFloat()*scale});
				}
				break;		                                  
			}
			case 'k': {
				if (ps.size() >= 4) {
					chainCircle((Point) {ps.at(1).toFloat()*scale, ps.at(2).toFloat()*scale},
							    ps.at(3).toFloat());
				}
				break;		                                  
			}
		}
	}
	return chain;
}

void DrawArea::chainLineTo(Point p) {
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	dx = (int)(p.x+0.5) - (int)(CurrentPoint.x+0.5);
	dy = (int)(p.y+0.5) - (int)(CurrentPoint.y+0.5);
	dxabs = dx >= 0 ? dx: -dx; //abs
	dyabs = dy >= 0 ? dy: -dy; //abs
	sdx = dx >= 0 ? 1: -1;     //sign
	sdy = dy >= 0 ? 1: -1;     //sign
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = (int) (CurrentPoint.x + 0.5);
	py = (int) (CurrentPoint.y + 0.5);
	addToChain(px, py);
	if (dxabs >= dyabs) { 	  // the line is more horizontal than vertical
		for (i = 0; i < dxabs; i++) {
			y += dyabs;
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			addToChain(px, py);
		}
	} else { 				 // the line is more vertical than horizontal
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			addToChain(px, py);
		}
	}
	CurrentPoint = p;
}

void DrawArea::chainBezier(Point p2, Point p3, Point p4) {
	chainBezierRec(CurrentPoint, p2, p3, p4, chainLevel);
	CurrentPoint = p4;
}

void DrawArea::scaleP(float sx, float sy, Point scaleP) {
	float scaleMatrix[9] = {sx,  0., scaleP.x - sx*scaleP.x,
	                        0.,  sy, scaleP.y - sy*scaleP.y,
							0.,  0., 1.};
	float result[9];
	multMatrix(scaleMatrix, curTrans, result);
	for (int i = 0; i < 9; i++) {
		curTrans[i] = result[i];
	}
}

void DrawArea::rotateP(float a, Point rotateP) {
	float b  = a/180. * PI;
	float rotateMatrix[9] = {cos(b), -sin(b), rotateP.x - cos(b)*rotateP.x + sin(b)*rotateP.y,
	                         sin(b),  cos(b), rotateP.y - sin(b)*rotateP.x - cos(b)*rotateP.y,
							 0.,      0.,     1.};
	float result[9];
	multMatrix(rotateMatrix, curTrans, result);
	for (int i = 0; i < 9; i++) {
		curTrans[i] = result[i];
	}
}

void DrawArea::identety() {
	curTrans[0] = 1.;
	curTrans[1] = 0.;
	curTrans[2] = 0.;
	curTrans[3] = 0.;
	curTrans[4] = 1.;
	curTrans[5] = 0.;
	curTrans[6] = 0.;
	curTrans[7] = 0.;
	curTrans[8] = 1.;
}

void DrawArea::multMatrix(float *mat1, float *mat2, float *resultMatrix) {
	resultMatrix[0] = mat1[0]*mat2[0] + mat1[1]*mat2[3] + mat1[2]*mat2[6];
	resultMatrix[1] = mat1[0]*mat2[1] + mat1[1]*mat2[4] + mat1[2]*mat2[7];
	resultMatrix[2] = mat1[0]*mat2[2] + mat1[1]*mat2[5] + mat1[2]*mat2[8];
	resultMatrix[3] = mat1[3]*mat2[0] + mat1[4]*mat2[3] + mat1[5]*mat2[6];
	resultMatrix[4] = mat1[3]*mat2[1] + mat1[4]*mat2[4] + mat1[5]*mat2[7];
	resultMatrix[5] = mat1[3]*mat2[2] + mat1[4]*mat2[5] + mat1[5]*mat2[8];
	resultMatrix[6] = mat1[6]*mat2[0] + mat1[7]*mat2[3] + mat1[8]*mat2[6];
	resultMatrix[7] = mat1[6]*mat2[1] + mat1[7]*mat2[4] + mat1[8]*mat2[7];
	resultMatrix[8] = mat1[6]*mat2[2] + mat1[7]*mat2[5] + mat1[8]*mat2[8];
}

void DrawArea::translate(Point p) {
	float translateMatrix[9] = {1., 0., p.x,
							    0., 1., p.y,
								0., 0., 1.};
	float result[9];
	multMatrix(translateMatrix, curTrans, result);
	for (int i = 0; i < 9; i++) {
		curTrans[i] = result[i];
	}
}

Point DrawArea::transform(Point p) {
	return (Point) {p.x*curTrans[0] + p.y*curTrans[1] + curTrans[2],
	                p.x*curTrans[3] + p.y*curTrans[4] + curTrans[5]};
}

void DrawArea::chainBezierRec(Point p1, Point p2, Point p3, Point p4, int level) {
	if (level == 0) {
		chainLineTo(p4);
	} else {
		Point l1 = p1;
		Point l2 = midpoint(p1, p2);
		Point h  = midpoint(p2, p3);
		Point r3 = midpoint(p3, p4);
		Point r4 = p4;
		Point l3 = midpoint(l2, h);
		Point r2 = midpoint(r3, h);
		Point l4 = midpoint(l3, r2);
		Point r1 = l4;
		chainBezierRec(l1, l2, l3, l4, level-1);
		chainBezierRec(r1, r2, r3, r4, level-1);
	}
}

void DrawArea::chainCircle(Point p2, float angle) {
    float curAngle, addAngle, oldAngle = 0.0, helpAngle;
    float radius = sqrt((p2.x-CurrentPoint.x)*(p2.x-CurrentPoint.x) + 
	                    (p2.y-CurrentPoint.y)*(p2.y-CurrentPoint.y)), radius2;
    Point n1, n2, n3;
    float n1e, n2e, n3e;
    radius2 = radius * radius;
    int x, y;
    
	angle = fmodf(angle, 720.);
	
	oldAngle = fmodf(atan2(p2.y - CurrentPoint.y, p2.x - CurrentPoint.x)*180./PI + 360., 360.);
	
	x = (int) (CurrentPoint.x + 0.5);
	y = (int) (CurrentPoint.y + 0.5);
	addToChain(x, y);
    while (1) {
        curAngle = fmod(atan2((float)((int) (p2.y + 0.5) - y), 
							  (float)((int) (p2.x + 0.5) - x))*180./PI + 360., 360.);
		if (((helpAngle = curAngle - oldAngle)) < -180.)
			addAngle += helpAngle + 360.;
		else if (helpAngle > 180.)
			addAngle += helpAngle - 360.;
		else 
			addAngle += helpAngle;

        switch ((int) (curAngle/45.) % 8) {
            case 7:
            case 0:
                 n1.x = -1;
                 n1.y = -1;
                 n2.x =  0;
                 n2.y = -1;
                 n3.x =  1;
                 n3.y = -1;        
                 break;
            case 1:
            case 2:
                 n1.x =  1;
                 n1.y =  1;
                 n2.x =  1;
                 n2.y =  0;
                 n3.x =  1;
                 n3.y = -1;
                 break;
            case 3: 
            case 4: 
                 n1.x = -1;
                 n1.y =  1;
                 n2.x =  0;
                 n2.y =  1;
                 n3.x =  1;
                 n3.y =  1;
                 break;
            case 5:
            case 6:
                 n1.x = -1;
                 n1.y =  1;
                 n2.x = -1;
                 n2.y =  0;
                 n3.x = -1;
                 n3.y = -1;
                 break;
        }  
        if (angle < 0.0) {
             n1.x = -n1.x;
             n1.y = -n1.y;
             n2.x = -n2.x;
             n2.y = -n2.y;
             n3.x = -n3.x;
             n3.y = -n3.y;             
        }
        n1e =  fabs((p2.x-(n1.x+(float) x))*(p2.x-(n1.x+(float) x)) + 
                    (p2.y-(n1.y+(float) y))*(p2.y-(n1.y+(float) y)) - radius2);
        n2e =  fabs((p2.x-(n2.x+(float) x))*(p2.x-(n2.x+(float) x)) +  
                    (p2.y-(n2.y+(float) y))*(p2.y-(n2.y+(float) y)) - radius2);
        n3e =  fabs((p2.x-(n3.x+(float) x))*(p2.x-(n3.x+(float) x)) + 
                    (p2.y-(n3.y+(float) y))*(p2.y-(n3.y+(float) y)) - radius2);
        if (n1e < n2e) {
           if (n3e < n1e) {
              x += (int) n3.x;
              y += (int) n3.y;
           } else {
              x += (int) n1.x;
              y += (int) n1.y;
           }
        } else {
           if (n3e < n2e) {
              x += (int) n3.x;
              y += (int) n3.y;
           } else {
              x += (int) n2.x;
              y += (int) n2.y;
           }
        }
		addToChain(x, y);
		if (fabs(addAngle) >= fabs(angle))
           break;
		oldAngle = curAngle;
    } 
	CurrentPoint.x = x;
	CurrentPoint.y = y;
}

void DrawArea::startChain(int px, int py) {
	chain = "";
	QString help;
	chain.append(help.setNum(px));
	chain.append(';');
	chain.append(help.setNum(py));
	
	chainPosX = px;
	chainPosY = py;
}

void DrawArea::addToChain(int px, int py) {
	int dx = px - chainPosX;
	int dy = py - chainPosY;
	QChar addChain = ' ';
	bool skip = false;	
	if (dy == -1) {
		switch (dx) {
			case -1: addChain = 'H'; break;
			case  0: addChain = 'A'; break;
			case  1: addChain = 'B'; break;
			default: addChain = 'X'; break;
		}
	} else if (dy == 0) {
		switch (dx) {
			case -1: addChain = 'G';  break;
			case  0: skip     = true; break;
			case  1: addChain = 'C';  break;
			default: addChain = 'X';  break;
		}
	} else if (dy == 1) {
		switch (dx) {
			case -1: addChain = 'F'; break;
			case  0: addChain = 'E'; break;
			case  1: addChain = 'D'; break;
			default: addChain = 'X'; break;
		}
	} else {
		addChain = 'X';
	}

	if (!skip) {
		chain.append(addChain);
	}
	chainPosX = px;
	chainPosY = py;
}

void DrawArea::mousePressEvent(QMouseEvent * e) {
	float x = e->x()/zoom;
	float y = e->y()/zoom;
	DragPoint.x = x;
	DragPoint.y = y;
	for (int i = 0; i < controllPoints.size(); ++i) {
        if (fabs(controllPoints.at(i)->getPoint().x-x) < 5/zoom && 
			fabs(controllPoints.at(i)->getPoint().y-y) < 5/zoom) {
			dragHasAngle = controllPoints.at(i)->isAngle();	
			dragAngle = controllPoints.at(i)->getAngle();	
			dragLine = controllPoints.at(i)->getLine();
			dragElement = controllPoints.at(i)->getElement();
			drag = true;
			break;
		}
	}
  	if (!drag) {
		if (fabs(imageBegin.x-x) < 5/zoom && 
			fabs(imageBegin.y-y) < 5/zoom) {
				dragImage = DRAG_START;
		} else if (fabs(imageEnd.x-x) < 5/zoom  && 
				   fabs(imageEnd.y-y) < 5/zoom ) {
				dragImage = DRAG_END;
		} else if (x >= imageBegin.x && x <= imageEnd.x &&
				   y >= imageBegin.y && y <= imageEnd.y) {
				dragImage = DRAG_IMAGE;
				imageLastPos = (Point) {x, y};
		}
	}
}

void DrawArea::mouseReleaseEvent(QMouseEvent * e) {
	if (drag) {
		if (dragAbort) {
			checkAndDraw();
		} else {
			text->setPlainText(list.join("\n"));
		}
	} 
	drag = false;
	dragImage = 0;
	
}

void DrawArea::keyPressEvent(QKeyEvent * e) {
	if (drag && e->key() == Qt::Key_Escape) {
		dragAbort = true;
		drag = false;
		checkAndDraw();
	}
}

void DrawArea::mouseMoveEvent(QMouseEvent * e) {	
	float x = e->x()/zoom;
	float y = e->y()/zoom;
	QString str;
	if (drag) { //&& (dragCount % 10) == 0
		QStringList line = list.at(dragLine).split(" ");
		if (!dragHasAngle) {
			line[dragElement]   = str.setNum(x);
			line[dragElement+1] = str.setNum(y);
			list[dragLine] = line.join(" ");
			repaint();
		} else {
			line[dragElement] = str.setNum(dragAngle + DragPoint.y - y);
			list[dragLine] = line.join(" ");
			repaint();
		}	
	}
	switch (dragImage) {
	case DRAG_START:
		imageBegin = (Point) {x, y};
		imageScaleChanged = true;
		repaint();
		break;
	case DRAG_END:
		imageScaleChanged = true;
		imageEnd = (Point) {x, y};
		repaint();
		break;
	case DRAG_IMAGE:
		float dx = x - imageLastPos.x;
		float dy = y - imageLastPos.y;
		imageBegin.x += dx;
		imageBegin.y += dy;
		imageEnd.x += dx;
		imageEnd.y += dy;
		imageLastPos = (Point) {x, y};
		repaint();
		break;
	}
}

void DrawArea::setImage(QString fileName) {
	image.load(fileName);
	imageBegin = (Point) {5.0, 5.0};
	imageEnd = (Point) {image.width()+5.0, image.height()+5.0};
	imageScaleChanged = true;
	repaint();
}

void DrawArea::setShowControlElements(bool state) {
	showControlElements = state;
	repaint();
}
