//  TestProgramm um das Simulatorwidget zu erstellen.

#include <QtGui>

#include "LMainWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	LMainWindow *window = new LMainWindow;

	window->show();
	return app.exec();
}
 