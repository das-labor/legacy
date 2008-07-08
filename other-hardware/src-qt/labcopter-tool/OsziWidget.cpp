
#include "OsziWidget.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>


OsziWidget::OsziWidget(QWidget * parent){
	ui.setupUi(parent);

	scene = new QGraphicsScene;
	
	ui.graphicsView->setScene(scene);
	
	//scene->setBackgroundBrush(Qt::black);
	QGraphicsRectItem *r = new QGraphicsRectItem(10, 10, 30, 40);
	r->setFlag(QGraphicsItem::ItemIsSelectable);
	scene->addItem(r);
	
	
	QGraphicsTextItem *t = scene->addText("Hello, world!");
	t->setFlag(QGraphicsItem::ItemIsSelectable);
	
	
}
