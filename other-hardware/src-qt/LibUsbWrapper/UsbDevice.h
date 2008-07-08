
#include <usb.h>

class UsbDevice
{
public:
	UsbDevice(int vid, int pid);
	~UsbDevice();

	bool usbConnect();
	bool usbDisconnect();
	bool isConnected();

	int reqeuestRead(int request, int value, int index, char *bytes, int size, int timeout);
	//not implemented yet
	int requestWrite(int request, int value, int index, char *bytes, int size, int timeout);

	int fetchInterrupt(int ep, char *bytes, int size, int timeout);

private:
	usb_dev_handle	*handle;
	struct usb_device		*device;
};
