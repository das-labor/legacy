
#include "MainWindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

MainWindow::MainWindow(){
	ui.setupUi(this);

	scene = new QGraphicsScene;
	
	QGraphicsRectItem *r = new QGraphicsRectItem(10, 10, 30, 40);
	
	r->setFlag(QGraphicsItem::ItemIsSelectable);
	
	ui.graphicsView->setScene(scene);
	
	QGraphicsTextItem *t = scene->addText("Hello, world!");
	
	t->setFlag(QGraphicsItem::ItemIsSelectable);
	
	
	scene->addItem(r);
	

}
