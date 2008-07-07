
#include "MainWindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

MainWindow::MainWindow(){
	ui.setupUi(this);

	scene = new QGraphicsScene;
	
	ui.graphicsView->setScene(scene);
	
	//QGraphicsRectItem *r = new QGraphicsRectItem(10, 10, 30, 40);
	//r->setFlag(QGraphicsItem::ItemIsSelectable);
	//scene->addItem(r);
	
	
	QGraphicsTextItem *t = scene->addText("Hello, world!");
	t->setFlag(QGraphicsItem::ItemIsSelectable);
	
	
	

}
