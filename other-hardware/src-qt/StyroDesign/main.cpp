#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QListView>
#include <QWidget>
#include <QScrollArea>
#include <QTextEdit>

#include "display.h"

class MyWidget : public QWidget
{
private:
	QGridLayout *gridLayout;
	DrawArea *drawArea;
	QPushButton *quit;
	QScrollArea *scroll;
	QTextEdit *text;
	
public:
	MyWidget(QWidget *parent = 0);
	~MyWidget();
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

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MyWidget widget;
	widget.setGeometry(100, 100, 500, 355);
	widget.show();
	return app.exec();
}
