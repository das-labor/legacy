#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QStandardItemModel>

#include "ui_mainwindow.h"
#include "client.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent);

	public slots:
	//	void doPlot();
	
	private slots:
		void scanCanButtonPressed();
		void connectButtonPressed();
		void gotPacket(CanPacket cp);
	
	private:
		QStandardItemModel *model ;
		CanSocket *canSocket;	
	
		Ui::MainWindow ui;
};


#endif
