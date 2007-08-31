
#include "MainWindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>




class block : public QGraphicsRectItem{
	public:
	block(qreal x, qreal y, qreal width, qreal height) : QGraphicsRectItem(x, y, width, height,0,0){
		
	}
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * event ){
		setPos(event->scenePos());
		
	}
	
	
};


MainWindow::MainWindow(){
	ui.setupUi(this);

	scene = new QGraphicsScene;
	
	QGraphicsRectItem *r = new block(10, 10, 30, 40);
	
	r->setFlag(QGraphicsItem::ItemIsSelectable);
	
	ui.graphicsView->setScene(scene);
	
	QGraphicsTextItem *t = scene->addText("Hello, world!");
	
	t->setFlag(QGraphicsItem::ItemIsSelectable);
	
	
	scene->addItem(r);
	

}
