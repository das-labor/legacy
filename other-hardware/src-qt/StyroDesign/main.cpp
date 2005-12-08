#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QListView>
#include <QWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QToolBar>
#include <QMainWindow>
#include <QAction>


#include "display.h"

class MyWidget : public QWidget
{
private:
	QGridLayout *gridLayout;
	QPushButton *quit;
	QScrollArea *scroll;
	QTextEdit *text;
	DrawArea *drawArea;
public:
	MyWidget(QWidget *parent = 0);
	~MyWidget();
	
public slots:
	void zoomInc();
	void zoomDec();
};

MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
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
	setLayout(gridLayout);
}

MyWidget::~MyWidget() {
	delete gridLayout;
	delete drawArea;
	delete quit;
	delete scroll;
	delete text;
}

void MyWidget::zoomInc() {
	drawArea->setZoom(drawArea->getZoom()+0.1);
}

void MyWidget::zoomDec() {
	drawArea->setZoom(drawArea->getZoom()-0.1);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMainWindow mainWindow;
	QToolBar toolbar;
	QAction *act;
	
	MyWidget widget;
	widget.setGeometry(100, 100, 500, 355);

	act = toolbar.addAction("Zoom in");
	//connect(act, SIGNAL(triggered()), mainWindow, SLOT(widget.zoomInc()));
	act = toolbar.addAction("Zoom out");
	//connect(act, SIGNAL(triggered()), mainWindow, SLOT(widget.zoomDec()));
	mainWindow.addToolBar(&toolbar);
	mainWindow.setCentralWidget(&widget);
	mainWindow.show();
	return app.exec();
}
