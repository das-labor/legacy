/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>
 */
#pragma unmanaged

#include <usb.h>
#include <string.h>
#include <stdio.h>
#include "RfmUsb.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"
#include "../common/opendevice.h"

//livectrl
#include "rfm12_livectrl.h"


//connect extended function
//returns != 0 on error
int rfmusb_ConnectEx(rfmusb_dev_handle **handle, int vid, int pid)
{
	/* usb setup */
	printf("F: %20s, %5i\n", __FILE__, __LINE__);
	usb_init();
	printf("F: %20s, %5i\n", __FILE__, __LINE__);

	return usbOpenDevice (handle, vid, pid);
}


//connect function
//returns != 0 on error
int rfmusb_Connect(rfmusb_dev_handle **handle)
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

	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

    //try to open the device
	return rfmusb_ConnectEx(handle, vid, pid);
}


int rfmusb_Close(rfmusb_dev_handle *handle)
{
	return usb_close (handle);
}


//move to common header soon
#define RADIO_TXBUFFER_HEADER_LEN 2
int rfmusb_TxPacket(rfmusb_dev_handle *handle, unsigned char type, unsigned char len, unsigned char *data)
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
    return usb_control_msg (handle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RADIO_PUT, 0, 0, (char *)&buf, packetLen,
        DEFAULT_USB_TIMEOUT);
}


int rfmusb_RxPacket(rfmusb_dev_handle *handle, rfmusb_packetbuffer * packetBuffer)
{
    //clear buffer
    memset (packetBuffer, 0x00, sizeof(rfmusb_packetbuffer));

    //request raw packet and return length
    return usb_control_msg (handle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            RFMUSB_RQ_RADIO_GET, 0, 0, (char *)packetBuffer, sizeof(rfmusb_packetbuffer),
            DEFAULT_USB_TIMEOUT);
}


int rfmusb_rfm12_config(rfmusb_dev_handle *handle, unsigned char cmd, unsigned short value)
{
	//update current value
	rfm12_livectrl(cmd, value);
		
	//send radio configuration request and retuurn result
    return usb_control_msg (handle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RADIO_CFG, 0, cmd, (char *)&value, 2,
        DEFAULT_USB_TIMEOUT);
}


void rfmusb_rfm12_cmd_to_string(uint8_t cmd, char * s)
{
	rfm12_cmd_to_string(cmd, s);
}


void rfmusb_rfm12_get_parameter_string(uint8_t cmd, char * str)
{
	rfm12_livectrl_get_parameter_string(cmd, str);
}


unsigned int rfmusb_rfm12_get_cmd_count()
{
	return NUM_LIVECTRL_CMDS;
}
