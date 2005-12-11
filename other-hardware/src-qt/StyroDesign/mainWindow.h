#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QList>

#include "display.h"
#include "controllPoint.h"

class MyWidget : public QMainWindow
{
	Q_OBJECT
	
public:
	MyWidget(QWidget *parent = 0);
	~MyWidget();
	
public slots:
	void zoomInc();
	void zoomDec();
	void zoom100();
	void open();
	void save();
	void saveAs();
	void openImage();

private:
	QString fileName;
	QGridLayout *gridLayout;
	QPushButton *quit;
	QScrollArea *scroll;
	QTextEdit *text;
	DrawArea *drawArea;
	QWidget *widget;
	QToolBar *toolbar;
};

#endif // MAINWINDOW_H
