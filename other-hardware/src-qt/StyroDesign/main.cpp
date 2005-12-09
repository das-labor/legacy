#include "mainWindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MyWidget mainWindow;
	
	mainWindow.show();
	return app.exec();
}
