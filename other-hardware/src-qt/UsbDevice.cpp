#include "LibUsbWrapper.h"

bool UsbDevice::connect()
{
	if(device)
	{
		handle = usb_open(device);
	}

	return isConnected();
}


bool UsbDevice::disconnect()
{
	int tmp;

	if(handle)
	{
		tmp = usb_close(handle);
		handle = NULL;
		return (tmp == 0)?true:false;
	}
	else
	{
		return false;
	}
}


bool UsbDevice::isConnected()
{
	(handle)?true:false;
}


int UsbDevice::reqeuestRead(int requesttype, int request, int value, int index, char *bytes, int size, int timeout)
{
	return usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		requesttype, value, index, bytes, size, timeout);
}


int UsbDevice::requestWrite(int requesttype, int request, int value, int index, char *bytes, int size, int timeout)
{
	//not implemented yet
	return -1;
}


int UsbDevice::fetchInterrupt(int ep, char *bytes, int size, int timeout)
{
	return usb_interrupt_read(handle, ep, bytes, size, timeout);
}


UsbDevice::UsbDevice(int vid, int pid)
{
	struct	usb_bus      *bus;
	struct	usb_device   *dev;

	//init member variables
	handle = NULL;
	device = NULL;

	LibUsbWrapper::Instance().findBusses();
    LibUsbWrapper::Instance().findDevices();

    for(bus = LibUsbWrapper::Instance().getBusses(); bus; bus = bus->next)
	{
        for(dev = bus->devices; dev; dev = dev->next)
		{
            if((vendorID == 0 || dev->descriptor.idVendor == vendorID) && (productID == 0 || dev->descriptor.idProduct == productID))
			{
				//save device
				device = dev;
				break;
            }
        }

		//device found
        if(device)
		{
            break;
		}
    }
}


UsbDevice::~UsbDevice()
{
	disconnect();
}
