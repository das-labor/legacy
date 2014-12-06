#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "opendevice.h"
#include "lib-host/debug.h"


/*
	int vid, pid;
	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];
*/
static unsigned int is_interesting(libusb_device *device) {
	struct libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(device, &desc);
	debug_assert(r >= 0, "USB: failed to get device descriptor: %s\r\n", libusb_error_name(r));
	if (desc.idVendor == 0x16c0 && desc.idProduct == 0x05dc)
		return 1;
	else
		return 0;
}

libusb_device_handle *usbGetDeviceHandle(int usb_param) {
	libusb_device **list, **listMatch;
	libusb_device_handle *handle = NULL;
	int err = 0;
	unsigned labDevCount = 0;

	int r = libusb_init(NULL);
	debug_assert(r >= 0, "USB_init error: %s\r\n", libusb_error_name(r));

	ssize_t devcnt = libusb_get_device_list(NULL, &list);
	debug_assert(devcnt >= 0, "USB_get_device_list error: %s\r\n", libusb_error_name(devcnt));
	listMatch = calloc(devcnt, sizeof(struct libusb_device *));
	debug_assert(listMatch != NULL, "Can't allocate memory.\r\n");
	for (unsigned i = 0; i < devcnt; i++) {
		libusb_device *device = list[i];

		if (is_interesting(device)) {
			listMatch[labDevCount++] = device;
		}
	}
	debug_assert(labDevCount != 0, "Can't find RfmUSB / CANGW Device\r\n");
	printf("Found %i devices, using device %i\n", labDevCount, usb_param);
	debug_assert(usb_param >= labDevCount, "usb_param to big\r\n");

	if (labDevCount) {
		err = libusb_open(listMatch[usb_param - 1], &handle);
		debug_assert(err >= 0, "Can't open RfmUSB / CANGW Device\r\n");
	}
	libusb_free_device_list(list, 1);
	libusb_free_device_list(listMatch, 0);
	return handle;
}
