

#include <QStandardItemModel>
#include <QModelIndex>
#include <QTreeView>
#include <QImage>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
	ui.setupUi(this);
	
	canSocket = new CanSocket(this);
	
	canSocket->connectToCand(ui.lineEditHost->text(), ui.lineEditPort->text().toInt());
	
	model = new QStandardItemModel();
    model->insertColumns(0, 3);
        
/*
	QModelIndex parentm;
    for (int i = 0; i < 4; ++i) {
        parentm = model->index(0, 0, parentm);
        model->insertRows(0, 1, parentm);
        model->insertColumns(0, 3, parentm);
        QModelIndex index = model->index(0, 0, parentm);
        model->setData(index, i);
		index = model->index(0, 2, parentm);
        model->setData(index, 12);
    }
*/
	
	ui.treeView->setModel(model);
	
	//connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(reject()));
	
	connect (canSocket, SIGNAL(gotPacket(CanPacket)), this, SLOT(gotPacket(CanPacket)));
	connect (ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectButtonPressed()));
	connect (ui.pushButtonScanCan, SIGNAL(clicked()), this, SLOT(scanCanButtonPressed()));

}

void MainWindow::gotPacket(CanPacket cp){
	int pos = model->rowCount();
	model->insertRows(pos, 1);
	QModelIndex index = model->index(pos, 0);
    model->setData(index, cp.addr_src);
}

void MainWindow::connectButtonPressed(){
	canSocket->connectToCand(ui.lineEditHost->text(), ui.lineEditPort->text().toInt());
}

void MainWindow::scanCanButtonPressed(){
	for(uint i=0;i<255;i++){
		CanPacket cp(0,0x30,i,0x30);
		cp.data[0] = 0;
		canSocket->sendPacket(cp);		
	}	
}
