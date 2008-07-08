
#include <QWidget>
#include <QGraphicsView>

#include "ui_OsziWidget.h"

class OsziWidget : public QWidget{
	Q_OBJECT;
	
	public:
		OsziWidget(QWidget * parent);
	
	private:
		Ui::OsziWidget ui;

		QGraphicsScene * scene;
	
	
};
