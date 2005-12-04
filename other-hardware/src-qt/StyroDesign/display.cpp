#include <QPainter>

#include "display.h"

DrawArea::DrawArea(QWidget *parent)
	: QWidget(parent)
{
	currentAngle = 45;
	setPalette(QPalette(QColor(250, 250, 200)));
}

void DrawArea::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);

	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::blue);

	painter.translate(0, rect().height());
	painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
	painter.rotate(-currentAngle);
	painter.drawRect(QRect(33, -4, 15, 8));
}
