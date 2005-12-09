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

#include "display.h"

class MyWidget : public QMainWindow
{
	Q_OBJECT
	
public:
	MyWidget(QWidget *parent = 0);
	~MyWidget();
	
public slots:
	void zoomInc();
	void zoomDec();

private:
	QGridLayout *gridLayout;
	QPushButton *quit;
	QScrollArea *scroll;
	QTextEdit *text;
	DrawArea *drawArea;
	QWidget *widget;
	QToolBar *toolbar;

};

#endif // MAINWINDOW_H
