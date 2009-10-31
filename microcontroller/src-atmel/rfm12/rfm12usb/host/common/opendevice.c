/* Name: opendevice.c
 * Project: AVR-USB host-side library
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id: opendevice.c 553 2008-04-17 19:00:20Z cs $
 */

/*
General Description:
The functions in this module can be used to find and open a device based on
libusb or libusb-win32.
*/

#include <stdio.h>
#include "opendevice.h"

/* ------------------------------------------------------------------------- */

int usbOpenDevice(usb_dev_handle **device, int vendorID, int productID)
{
    struct usb_bus      *bus;
    struct usb_device   *dev;
    usb_dev_handle      *handle = NULL;
    int                 errorCode = USBOPEN_ERR_NOTFOUND;

    usb_find_busses();
    usb_find_devices();
	
	/* iterate over all devices on all busses */
    for(bus = usb_get_busses(); bus; bus = bus->next)
	{
        for(dev = bus->devices; dev; dev = dev->next)
		{
            if((vendorID == 0 || dev->descriptor.idVendor == vendorID)
                        && (productID == 0 || dev->descriptor.idProduct == productID))
			{
                handle = usb_open(dev); /* we need to open the device in order to query strings */
                if(!handle)
				{
                    errorCode = USBOPEN_ERR_ACCESS;
                    fprintf(stderr, "Warning: cannot open VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
                    continue;
                }
				else break;
            }
        }
		
        if(handle)  /* we have found a device */
		{
            break;
		}
    }
	
    if(handle != NULL){
        errorCode = 0;
        *device = handle;
    }
	
    return errorCode;
}


int usbCountDevices(int vendorID, int productID)
{
    struct usb_bus      *bus;
    struct usb_device   *dev;
    int                 dev_cnt = 0;

    usb_find_busses();
    usb_find_devices();
	
	/* iterate over all devices on all busses */
    for(bus = usb_get_busses(); bus; bus = bus->next)
	{
        for(dev = bus->devices; dev; dev = dev->next)
		{  
            if((vendorID == 0 || dev->descriptor.idVendor == vendorID)
                        && (productID == 0 || dev->descriptor.idProduct == productID))
			{
                
				dev_cnt++;
            }
        }
    }
	
    return dev_cnt;
}

int usbListDevices(struct usb_device **devices, int vendorID, int productID)
{
    struct usb_bus      *bus;
    struct usb_device   *dev;
    int                 dev_cnt = 0;

    usb_find_busses();
    usb_find_devices();
	
    for(bus = usb_get_busses(); bus; bus = bus->next)
	{
        for(dev = bus->devices; dev; dev = dev->next)
		{  
            if((vendorID == 0 || dev->descriptor.idVendor == vendorID)
                        && (productID == 0 || dev->descriptor.idProduct == productID))
			{
				devices[dev_cnt++] = dev;
            }
        }
    }

    return dev_cnt;
}

/* ------------------------------------------------------------------------- */

