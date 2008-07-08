
#include "MainWindow.h"


MainWindow::MainWindow(){
	char buf[255];
	ui.setupUi(this);

	//oszi = new OsziWidget(*ui.graphicsView);	
	
	usbDevice = new UsbEventDevice(0x16c0, 0x05dc);
	usbDevice->usbConnect();
	
	usbDevice->reqeuestRead(1, 0, 0, buf, sizeof(buf), 4000);

	qDebug(buf);
	
}
