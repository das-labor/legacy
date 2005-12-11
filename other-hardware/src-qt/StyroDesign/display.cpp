#include <QPainter>
#include <QStringListIterator>
#include <QChar>
#include <iostream>
#include <QMouseEvent>
#include <math.h>

#define DRAG_START 1
#define DRAG_END 2
#define DRAG_IMAGE 3

using namespace std;

#include "display.h"

DrawArea::DrawArea(QTextEdit *textedit, QWidget *parent)
	: QWidget(parent)
{
	width  = 2000;
	height = 2000;
	currentAngle = 45;
	text = textedit;
	drawLevel = 7;
	chainLevel = 7;
	setPalette(QPalette(QColor(255, 255, 255)));
	setZoom(1.0);
	drag = false;
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
		painter.drawImage((int)(zoom*imageBegin.x+0.5), 
			    		  (int)(zoom*imageBegin.y+0.5), 
						  image.scaled((int)(zoom*(imageEnd.x-imageBegin.x)+0.5),
									   (int)(zoom*(imageEnd.y-imageBegin.y)+0.5)));
		painter.setBrush(Qt::green);
		painter.drawRect((int)(zoom*imageBegin.x+0.5)-2,(int)(zoom*imageBegin.y+0.5)-2, 4, 4);
		painter.drawRect((int)(zoom*imageEnd.x+0.5)-2,  (int)(zoom*imageEnd.y+0.5)-2, 4, 4);
		painter.setBrush(Qt::black);
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
		}
		j++;
	}
}

void DrawArea::setZoom(float zoom) {
	this->zoom = zoom;
	resize((int)(zoom*height), (int)(zoom*width));
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

	g->setPen(Qt::red);
	CurrentPoint = help;
	drawLineTo(p2, g, -1);
	CurrentPoint = p3;
	drawLineTo(p4, g, -1);
	g->setPen(Qt::black);

	addControlPoint(p2, lineNo, 1, g);
	addControlPoint(p3, lineNo, 3, g);
	addControlPoint(p4, lineNo, 5, g);
	CurrentPoint = p4;
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

void DrawArea::addControlPoint(Point p, int line, int firstElement, QPainter *g) {
	controllPoints.append(new ControllPoint(p, line, firstElement));
	g->setBrush(Qt::green);
	g->drawRect((int)(zoom*p.x+0.5)-2,(int)(zoom*p.y+0.5)-2, 4, 4);
	g->setBrush(Qt::black);
}

void DrawArea::deleteControlPoints() {
	while (!controllPoints.isEmpty())
        delete controllPoints.takeFirst();
}

Point DrawArea::midpoint(Point p1, Point p2) {
	return (Point) {(p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0};
} 	

void DrawArea::getChainCode() {
	QStringListIterator i(list);
	
	while (i.hasNext())  {
		QStringList ps = i.next().split(' ');
		switch ((ps.at(0).toAscii())[0]) {
			case 's': {
				if (ps.size() >= 3) {
					CurrentPoint.x = ps.at(1).toFloat();
					CurrentPoint.y = ps.at(2).toFloat();
					startChain((int)(CurrentPoint.x), (int)(CurrentPoint.y));
				}
				break;
			} 
			case 'l': {
				if (ps.size() >= 3) {
					chainLineTo((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()});
				}
				break;
			}
			case 'c': {
				if (ps.size() >= 7) {
				chainBezier((Point) {ps.at(1).toFloat(), ps.at(2).toFloat()},
							(Point) {ps.at(3).toFloat(), ps.at(4).toFloat()},
							(Point) {ps.at(5).toFloat(), ps.at(6).toFloat()});
				}
				break;		                                  
			}
		}
	}	
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
	px = (int)(CurrentPoint.x+0.5);
	py = (int)(CurrentPoint.y+0.5);
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
	 for (int i = 0; i < controllPoints.size(); ++i) {
        if (fabs(controllPoints.at(i)->getPoint().x-x) < 5 && 
			fabs(controllPoints.at(i)->getPoint().y-y) < 5) {
				
			dragLine = controllPoints.at(i)->getLine();
			dragElement = controllPoints.at(i)->getElement();
			drag = true;
			dragCount = 0;
			break;
		}
    }
	if (fabs(imageBegin.x-x) < 5 && 
		fabs(imageBegin.y-y) < 5) {
			dragImage = DRAG_START;
	} else if (fabs(imageEnd.x-x) < 5 && 
			   fabs(imageEnd.y-y) < 5) {
			dragImage = DRAG_END;
	} else if (x >= imageBegin.x && x <= imageEnd.x &&
			   y >= imageBegin.y && y <= imageEnd.y) {
			dragImage = DRAG_IMAGE;
			imageLastPos = (Point) {x, y};
	}
}

void DrawArea::mouseReleaseEvent(QMouseEvent * e) {
	drag = false;
	dragImage = 0;
}

void DrawArea::mouseMoveEvent(QMouseEvent * e) {	
	float x = e->x()/zoom;
	float y = e->y()/zoom;
	if (drag) { //&& (dragCount % 10) == 0
		QString str = text->toPlainText();
		QStringList lines = str.split("\n");
		QStringList line = lines.at(dragLine).split(" ");
		line[dragElement]   = str.setNum(x);
		line[dragElement+1] = str.setNum(y);
		lines[dragLine] = line.join(" ");
		text->setPlainText(lines.join("\n"));
	}
	switch (dragImage) {
	case DRAG_START:
		imageBegin = (Point) {x, y};
		repaint();
		break;
	case DRAG_END:
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
	//++dragCount;
}

void DrawArea::setImage(QString fileName) {
	image.load(fileName);
	imageBegin = (Point) {5.0, 5.0};
	imageEnd = (Point) {image.width()+5.0, image.height()+5.0};
}
