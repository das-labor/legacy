#include "LibUsbWrapper.h"

LibUsbWrapper::LibUsbWrapper()
{
	usb_init();
}

LibUsbWrapper::~LibUsbWrapper()
{
}

void LibUsbWrapper::FindBusses()
{
	//ensure lib is loaded
	Instance();

	usb_find_busses();
}

void LibUsbWrapper::FindDevices()
{
	//ensure lib is loaded
	Instance();

	usb_find_devices();
}

usb_bus* LibUsbWrapper::GetBusses()
{
	//ensure lib is loaded
	Instance();

	return usb_get_busses();
}
