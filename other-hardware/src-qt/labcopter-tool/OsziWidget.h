
#include <QWidget>
#include <QGraphicsView>

#include "ui_OsziWidget.h"
#include "Graph.h"

class OsziWidget : public QWidget{
	Q_OBJECT;
	
	public:
		OsziWidget(QWidget * parent);
		void addGraph(Graph *);
		void update();
	
	private:
		Ui::OsziWidget ui;

		QGraphicsScene * scene;
		QList<Graph *> graphs;
	
	
};
