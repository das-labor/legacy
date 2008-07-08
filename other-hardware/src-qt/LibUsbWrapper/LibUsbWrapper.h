//#pragma once
#include <usb.h>

class LibUsbWrapper
{
public:
	//singleton instance method
	static LibUsbWrapper& Instance()
	{
		static LibUsbWrapper _instance;
		return _instance;
	}

	static void FindBusses();
	static void FindDevices();
	static usb_bus* GetBusses();

private:
	LibUsbWrapper();
	~LibUsbWrapper();	
};
