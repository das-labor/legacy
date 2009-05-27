/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>
 */

#include <string.h>
#include "i2cUsb.h"
#include "../common/opendevice.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"


//connect extended function
//returns != 0 on error
int i2cusb_ConnectEx(i2cusb_dev_handle ** handle, int vid, int pid, char *vendor, char *product)
{
	/* usb setup */
	usb_init();

	return usbOpenDevice ((usb_dev_handle **)handle, vid, vendor, pid, product, NULL, NULL, NULL);
}

//connect function
//returns != 0 on error
int i2cusb_Connect(i2cusb_dev_handle ** handle)
{
	int vid, pid;

	const unsigned char rawVid[2] =
	{
		USB_CFG_VENDOR_ID
	},
	rawPid[2] =
	{
		USB_CFG_DEVICE_ID
	};

	char vendor[] =
	{
		USB_CFG_VENDOR_NAME, 0
	},
	product[] =
	{
		USB_CFG_DEVICE_NAME, 0
	};

	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

    //try to open the device
	return i2cusb_ConnectEx(handle, vid, pid, vendor, product);
}


//move to common header soon
#define RADIO_TXBUFFER_HEADER_LEN 1
int i2cusb_WriteCard (i2cusb_dev_handle *udhandle, unsigned int len, unsigned char *data)
{
    //request to send packet and return result
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        I2CUSB_RQ_CARD_WRITE, 0, 0, (char *)data, len,
        DEFAULT_USB_TIMEOUT);
}


int i2cusb_ReadCard (i2cusb_dev_handle *udhandle, unsigned int len, unsigned char * data)
{
    
    //request raw packet and return length
    return usb_control_msg (udhandle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            I2CUSB_RQ_CARD_READ, 0, 0, (char *)data, len,
            DEFAULT_USB_TIMEOUT);
}


int i2cusb_GetCardStatus (i2cusb_dev_handle *udhandle, char *cardStatus)
{
    //request raw packet and return length
    return usb_control_msg (udhandle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            I2CUSB_RQ_CARD_STATUS, 0, 0, cardStatus, 1,
            DEFAULT_USB_TIMEOUT);
}


int i2cusb_ShowMeTheMoney (i2cusb_dev_handle *udhandle)
{
    //request to open cash tray
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        I2CUSB_RQ_TRAY_OPEN, 0, 0, (char *)NULL, 0,
        DEFAULT_USB_TIMEOUT);
}

int i2cusb_SetScannerPower (i2cusb_dev_handle *udhandle, int state)
{
    //request to open cash tray
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        I2CUSB_SET_SCANNER_POWER, 0, state, (char *)NULL, 0,
        DEFAULT_USB_TIMEOUT);
}
