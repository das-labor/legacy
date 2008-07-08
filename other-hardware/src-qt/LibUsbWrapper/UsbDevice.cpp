#include "LibUsbWrapper.h"
#include "UsbDevice.h"

bool UsbDevice::usbConnect()
{
	if(device)
	{
		handle = usb_open(device);
	}

	return isConnected();
}


bool UsbDevice::usbDisconnect()
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
	return (handle?true:false);
}


int UsbDevice::reqeuestRead(int request, int value, int index, char *bytes, int size, int timeout)
{
	return usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		request, value, index, bytes, size, timeout);
}


int UsbDevice::requestWrite(int request, int value, int index, char *bytes, int size, int timeout)
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

	LibUsbWrapper::Instance().FindBusses();
    LibUsbWrapper::Instance().FindDevices();

    for(bus = LibUsbWrapper::Instance().GetBusses(); bus; bus = bus->next)
	{
        for(dev = bus->devices; dev; dev = dev->next)
		{
            if((vid == 0 || dev->descriptor.idVendor == vid) && (pid == 0 || dev->descriptor.idProduct == pid))
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
	usbDisconnect();
}
