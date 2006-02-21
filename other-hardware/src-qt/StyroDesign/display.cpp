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
	drag = false;
	imageScaleChanged = false;
	showControlElements = true;
	dragImage = 0;
	
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
	while (i.hasNext())  {
		QStringList ps = i.next().split(' ');
		switch ((ps.at(0).toAscii())[0]) {
			case 's': {
				if (ps.size() >= 3) {
					deleteControlPoints();
					CurrentPoint.x = ps.at(1).toFloat();
					CurrentPoint.y = ps.at(2).toFloat();
					addControlPoint(CurrentPoint, j, 1, &painter);
				}
				break;
			} 
			case 'l': {
				if (ps.size() >= 3) {
					drawLineTo((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()}, &painter, j);
				}
				break;
			}
			case 'c': {
				if (ps.size() >= 7) {
					drawBezier((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()},
							   (Point) {ps.at(3).toFloat(), ps.at(4).toFloat()},
							   (Point) {ps.at(5).toFloat(), ps.at(6).toFloat()},
								&painter, j);
				}
				break;		                                  
			}
			case 'k': {
				if (ps.size() >= 4) {
					drawCircle((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()},
							   ps.at(3).toFloat(), &painter, j);
				}
				break;		                                  
			}
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
	if (drag)
		text->setPlainText(list.join("\n"));
	drag = false;
	dragImage = 0;
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
