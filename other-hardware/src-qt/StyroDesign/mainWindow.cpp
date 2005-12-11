#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include "mainWindow.h"

MyWidget::MyWidget(QWidget *parent): QMainWindow(parent)
{
	toolbar = new QToolBar;
	widget = new QWidget;
	widget->setGeometry(100, 100, 500, 355);

	connect(toolbar->addAction(tr("Zoom in")), SIGNAL(triggered()), 
			this, SLOT(zoomInc()));
	connect(toolbar->addAction(tr("Zoom out")), SIGNAL(triggered()),
			this, SLOT(zoomDec()));	
	connect(toolbar->addAction(tr("100%")), SIGNAL(triggered()),
			this, SLOT(zoom100()));	
	connect(toolbar->addAction(tr("Open")), SIGNAL(triggered()),
			this, SLOT(open()));	
	connect(toolbar->addAction(tr("Save")), SIGNAL(triggered()),
			this, SLOT(save()));	
	connect(toolbar->addAction(tr("SaveAs")), SIGNAL(triggered()),
			this, SLOT(saveAs()));	
	
	setCentralWidget(widget);
	addToolBar(toolbar);
	
	quit = new QPushButton(tr("Quit"));
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
	drawArea->setZoom(drawArea->getZoom()*1.1);
}

void MyWidget::zoomDec() {
	drawArea->setZoom(drawArea->getZoom()*0.9);
}

void MyWidget::zoom100() {
	drawArea->setZoom(1.0);
}

void MyWidget::open() {
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), "", "StyroDesign Data (*.sdd)");

	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (file.open(QFile::ReadOnly | QFile::Text)) {
			text->setPlainText(file.readAll());
		}
	}
}

void MyWidget::save() {
	if (!fileName.isNull()) {
		QFile file(fileName);
		if (file.open(QFile::WriteOnly | QFile::Text)) {
			 QTextStream out(&file);
        	 out << text->toPlainText();
		}
		
	} else {
		saveAs();
	}
}

void MyWidget::saveAs() {
	fileName = QFileDialog::getSaveFileName(this,
				tr("Open File"), "", "StyroDesign Data (*.sdd)");
	fileName += ".sdd";
	save();
}
