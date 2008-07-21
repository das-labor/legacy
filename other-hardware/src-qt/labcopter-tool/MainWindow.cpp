
#include "MainWindow.h"

MainWindow::MainWindow(){
	char buf[255];
	ui.setupUi(this);
	
	usbDevice = new UsbEventDevice(0x16c0, 0x05dc);
	usbDevice->usbConnect();
	
	//usbDevice->reqeuestRead(1, 0, 0, buf, sizeof(buf), 4000);

	
	Graph * graph = new Graph;
	
	graph->append(0,0);
	graph->append(1,12);
	graph->append(2,5);
	graph->append(3,4);
	graph->append(4,3);
	graph->append(5,7);
	graph->append(6,10);
	graph->append(7,1);
	graph->append(8,1);
	graph->append(9,1);
	graph->append(10,1);
	graph->append(11,1);
	graph->append(12,1);
	graph->append(13,1);
	graph->append(14,1);
	graph->append(15,1);
	graph->append(16,1);
	graph->append(17,1);
	graph->append(18,1);
	graph->append(19,1);
	graph->append(20,1);
	graph->append(21,4);
	graph->append(22,3);
	graph->append(23,7);
	
	ui.widget->addGraph(graph);
	
	ui.widget->update();
	
	
}
