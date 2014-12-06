#ifndef __OPENDEVICE_H_INCLUDED__
#define __OPENDEVICE_H_INCLUDED__

#include <libusb-1.0/libusb.h>    /* this is libusb, see http://libusb.sourceforge.net/ */
#include <stdio.h>




libusb_device_handle *usbGetDeviceHandle(int usb_param);

#endif /* __OPENDEVICE_H_INCLUDED__ */

