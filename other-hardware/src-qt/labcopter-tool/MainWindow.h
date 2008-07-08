
#include "ui_MainWindow.h"
#include "UsbEventDevice.h"

class MainWindow : public QMainWindow{
public:
	MainWindow();

private:
	Ui::MainWindow ui;
	OsziWidget * oszi;
	UsbEventDevice * usbDevice;
	
};
