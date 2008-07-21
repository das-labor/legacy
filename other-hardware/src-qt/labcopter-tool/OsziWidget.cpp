
#include "OsziWidget.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>


OsziWidget::OsziWidget(QWidget * parent){
	ui.setupUi(parent);
	
	//graphs = new QList<Graph*>;

	scene = new QGraphicsScene;
	ui.graphicsView->setScene(scene);
	
	scene->setBackgroundBrush(Qt::black);
	
	//QGraphicsRectItem *r = new QGraphicsRectItem(10, 10, 30, 40);
	//r->setFlag(QGraphicsItem::ItemIsSelectable);
	//scene->addItem(r);
	
	
	//QGraphicsTextItem *t = scene->addText("Hello, world!");
	//t->setFlag(QGraphicsItem::ItemIsSelectable);
}

void OsziWidget::addGraph(Graph * graph){
	graphs.append(graph);
}

void OsziWidget::update(){
	QList<Graph*>::iterator it;
	for(it = graphs.begin(); it != graphs.end(); it++){
		int x;
		for(x=0; x< (*it)->values.size()-1; x++){
			QGraphicsLineItem * l = new QGraphicsLineItem(	
					(*it)->timestamps[x], (*it)->values[x], (*it)->timestamps[x+1], (*it)->values[x+1] , 0);
			l->setPen(QPen(Qt::red));
			scene->addItem(l);
		}
	}
}
