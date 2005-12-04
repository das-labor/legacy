#include <QApplication>
#include <QGridLayout>
#include <QPushButton>

#include "display.h"

class MyWidget : public QWidget
{
public:
	MyWidget(QWidget *parent = 0);
};

MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
	QPushButton *quit = new QPushButton("Quit");
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
	DrawArea *drawArea = new DrawArea;

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(quit, 0, 0);
	gridLayout->addWidget(drawArea, 1, 1, 2, 1);
	gridLayout->setColumnStretch(1, 10);
	setLayout(gridLayout);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MyWidget widget;
	widget.setGeometry(100, 100, 500, 355);
	widget.show();
	return app.exec();
}
