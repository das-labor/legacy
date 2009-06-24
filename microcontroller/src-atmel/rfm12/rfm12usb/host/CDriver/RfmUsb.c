/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>
 */

#include <usb.h>
#include <string.h>
#include <stdio.h>
#include "RfmUsb.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"


//connect extended function
//returns != 0 on error
int rfmusb_ConnectEx(usb_dev_handle ** handle, int vid, int pid, char *vendor, char *product)
{
	/* usb setup */
	printf("F: %20s, %5i\n", __FILE__, __LINE__);
	usb_init();
	printf("F: %20s, %5i\n", __FILE__, __LINE__);

	return usbOpenDevice (handle, vid, vendor, pid, product, NULL, NULL, NULL);
}

//connect function
//returns != 0 on error
int rfmusb_Connect(usb_dev_handle ** handle)
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
	return rfmusb_ConnectEx(handle, vid, pid, vendor, product);
}


//move to common header soon
#define RADIO_TXBUFFER_HEADER_LEN 2
int rfmusb_TxPacket (rfmusb_dev_handle *udhandle, unsigned char type, unsigned char len, unsigned char *data)
{
    rfmusb_packetbuffer buf;
    int packetLen;

    //trim packet length
    packetLen = RADIO_TXBUFFER_HEADER_LEN + ((len > RFM12_MAX_PACKET_LENGTH)?RFM12_MAX_PACKET_LENGTH:len);

    //setup buffer
    buf.len = len;
    buf.type = type;

    //copy data
    memcpy(buf.buffer, data, len);

    //request to send packet and return result
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RFM12_PUT, 0, 0, (char *)&buf, packetLen,
        DEFAULT_USB_TIMEOUT);
}


int rfmusb_RxPacket (rfmusb_dev_handle *udhandle, rfmusb_packetbuffer * packetBuffer)
{
    //clear buffer
    memset (packetBuffer, 0x00, sizeof(rfmusb_packetbuffer));

    //request raw packet and return length
    return usb_control_msg (udhandle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            RFMUSB_RQ_RFM12_GET, 0, 0, (char *)packetBuffer, sizeof(rfmusb_packetbuffer),
            DEFAULT_USB_TIMEOUT);
}
