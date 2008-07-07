
#include "ui_MainWindow.h"
#include <QGraphicsScene>

class MainWindow : public QMainWindow{
public:
	MainWindow();

private:
	Ui::MainWindow ui;
	QGraphicsScene * scene;
};
