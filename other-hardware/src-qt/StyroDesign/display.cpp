#include <QPainter>

#include "display.h"

DrawArea::DrawArea(QWidget *parent)
	: QWidget(parent)
{
	currentAngle = 45;
	setPalette(QPalette(QColor(255, 255, 255)));
}

void DrawArea::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);

	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::black);

	painter.translate(0, rect().height());
	painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
	painter.rotate(-currentAngle);
	painter.drawRect(QRect(33, -4, 15, 8));

}


void DrawArea::drawLineTo(Point p, QPainter *g) {
	g->drawLine((int)(zoom*CurrentPoint.x+0.5), 
			    (int)(zoom*CurrentPoint.y+0.5),
			    (int)(zoom*p.x+0.5), 
		   	    (int)(zoom*p.y+0.5));
	CurrentPoint = p;
}

void DrawArea::drawBezier(Point p2, Point p3, Point p4, QPainter *g) {
	drawBezierRec(CurrentPoint, p2, p3, p4, drawLevel, g);
	CurrentPoint = p4;
}

void DrawArea::drawBezierRec(Point p1, Point p2, Point p3, Point p4, int level, QPainter *g) {
	if (level == 0) {
		drawLineTo(p4, g);
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


Point DrawArea::midpoint(Point p1, Point p2) {
	return (Point) {(p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0};
} 	

QString DrawArea::getChainCode() {
/*
	foreach (object[] row in store) {
		string a = row[0] + "";
		string[] ps = a.Split(new Char [] {' '});
		switch (ps[0].ToLower().ToCharArray()[0]) {
			case 's': {
				CurrentPoint.x = Single.Parse(ps[1]);
				CurrentPoint.y = Single.Parse(ps[2]);
				startChain((int)CurrentPoint.x, (int)CurrentPoint.y);
				break;
			} 
			case 'l': {
				chainLineTo(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])));
				break;
			}
			case 'c': {
				chainBezier(new Point(Single.Parse(ps[1]), Single.Parse(ps[2])),
							new Point(Single.Parse(ps[3]), Single.Parse(ps[4])),
							new Point(Single.Parse(ps[5]), Single.Parse(ps[6])));
				break;		                                  
			}
		}
	}
	*/
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
	px = (int)(CurrentPoint.x+0.5);
	py = (int)(CurrentPoint.y+0.5);
	addToChain(px, py);
	if (dxabs >= dyabs) { // the line is more horizontal than vertical
		for (i = 0; i < dxabs; i++) {
			y += dyabs;
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			addToChain(px, py);
		}
	} else { // the line is more vertical than horizontal
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
	chain.append(px);
	chain.append(';');
	chain.append(py);
	
	chainPosX = px;
	chainPosY = py;
}

void DrawArea::addToChain(int px, int py) {
	int dx = px - chainPosX;
	int dy = py - chainPosY;
	char addChain = ' ';
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
			case -1: addChain = 'G'; break;
			case  0: skip     = true; break;
			case  1: addChain = 'C'; break;
			default: addChain = 'X'; break;
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
