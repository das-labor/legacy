#include "mainWindow.h"

MyWidget::MyWidget(QWidget *parent): QMainWindow(parent)
{
	toolbar = new QToolBar;
	widget = new QWidget;
	widget->setGeometry(100, 100, 500, 355);

	connect(toolbar->addAction("Zoom in"), SIGNAL(triggered()), 
			this, SLOT(zoomInc()));
	
	connect(toolbar->addAction("Zoom out"), SIGNAL(triggered()),
			this, SLOT(zoomDec()));	

	setCentralWidget(widget);
	addToolBar(toolbar);
	
	quit = new QPushButton("Quit");
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
	scroll = new QScrollArea;
	text = new QTextEdit;
	drawArea = new DrawArea(text, scroll);
	
	scroll->setWidget(drawArea);
	
	gridLayout = new QGridLayout;
	gridLayout->addWidget(quit, 1, 0);
	gridLayout->addWidget(scroll, 0, 1, 2, 1);
	gridLayout->addWidget(text, 0, 0);
	gridLayout->setColumnStretch(1, 10);
	widget->setLayout(gridLayout);
	
}

MyWidget::~MyWidget() {
	delete gridLayout;
	delete drawArea;
	delete quit;
	delete scroll;
	delete text;
	delete widget;
}

void MyWidget::zoomInc() {
	drawArea->setZoom(drawArea->getZoom()+0.1);
}

void MyWidget::zoomDec() {
	drawArea->setZoom(drawArea->getZoom()-0.1);
}
