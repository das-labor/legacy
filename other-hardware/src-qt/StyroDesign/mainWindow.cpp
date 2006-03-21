#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include "mainWindow.h"
#include "machine.h"

MyWidget::MyWidget(QWidget *parent): QMainWindow(parent)
{
	toolbar = new QToolBar;
	widget = new QWidget;
	widget->setGeometry(50, 50, 800, 600);

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
	connect(toolbar->addAction(tr("OpenImage")), SIGNAL(triggered()),
			this, SLOT(openImage()));
	connect(toolbar->addAction(tr("HideControlPoints")), SIGNAL(triggered()),
			this, SLOT(switchControlView()));
	connect(toolbar->addAction(tr("SaveChainCode")), SIGNAL(triggered()),
			this, SLOT(saveChainCode()));
	connect(toolbar->addAction(tr("Plot")), SIGNAL(triggered()),
			this, SLOT(plot()));
	
	
	setCentralWidget(widget);
	addToolBar(toolbar);
	
	scroll = new QScrollArea;
	text = new QTextEdit;
	drawArea = new DrawArea(text, scroll);
	machineDialog = new MachineDialog(this, drawArea);
	
	scroll->setWidget(drawArea);
	split = new QSplitter;
	split->addWidget(text);
	split->addWidget(scroll);
	 
	layout = new QHBoxLayout;
	layout->addWidget(split);
	widget->setLayout(layout);
	showControlElements = true;
	
}

MyWidget::~MyWidget() {
	/*
	delete split;
	delete drawArea;
	delete scroll;
	delete text;
	delete layout;
	delete widget;
	*/
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

void MyWidget::openImage() {
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), "", "Image (*.png)");
	drawArea->setImage(fileName);
}

void MyWidget::switchControlView() {
	showControlElements = showControlElements ? false : true;
	drawArea->setShowControlElements(showControlElements);
}

void MyWidget::plot() {
	machineDialog->show();
}

void MyWidget::saveChainCode() {
	if (fileName.isNull()) {
		fileName = "test";
	}
	QString newName = fileName + ".code";
	QFile file(newName);
	if (file.open(QFile::WriteOnly | QFile::Text)) {
		 QTextStream out(&file);
       	 out << drawArea->getChainCode();
	}
}
