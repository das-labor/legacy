class UsbDevice
public:
	UsbDevice(int vid, int pid);
	~UsbDevice();

	bool connect();
	bool disconnect();
	bool isConnected();

	int reqeuestRead(int requesttype, int request, int value, int index, char *bytes, int size, int timeout);
	//not implemented yet
	int requestWrite(int requesttype, int request, int value, int index, char *bytes, int size, int timeout);

	int fetchInterrupt(int ep, char *bytes, int size, int timeout);

private:
	usb_dev_handle	*handle;
	usb_device		*device;
};