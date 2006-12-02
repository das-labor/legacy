#include "LMainWindow.h"
#include <QHBoxLayout>

LMainWindow::LMainWindow() {
	LBorgGLWidget *glSimulator = new LBorgGLWidget;
	QHBoxLayout *mainLayout = new QHBoxLayout;
	
	mainLayout->addWidget(glSimulator);
	setLayout(mainLayout);
}
