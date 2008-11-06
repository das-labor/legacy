/* Dump packets onto the console.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */



#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <usb.h>
#include <unistd.h>

#include "opendevice.h"

#include "../../../../common/protocol.h"
#include "usb_id.h"
#include "../../../../../rfm12usb/common/requests.h"
#include "../../../../../rfm12usb/common/rfm12_buffer_size.h"
#include "usbstuff.h"

#define DEFAULT_USB_TIMEOUT 1000
#define RADIO_TXBUFFER_HEADER_LEN 2

static usb_dev_handle *udhandle = NULL;


int radio_tx(unsigned char len, unsigned char type, unsigned char *data)
{
    radio_packetbuffer buf;
    int packetLen;

    //trim packet length
    packetLen = RADIO_TXBUFFER_HEADER_LEN + ((len > RFM12_BUFFER_SIZE)?RFM12_BUFFER_SIZE:len);

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

int usbstuff_init ()
{
	uint8_t tmpchar = 0x00;
	uint_fast16_t i;
	ssize_t l;
	uint8_t buf[512];
	int vid, pid, tmp;
	uint_fast32_t packetcounter = 0;

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
	
	usb_init();
	
	/* usb setup */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	if (usbOpenDevice (&udhandle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		printf ("Can't find USB Device w/ uid %04X, pid %04X\r\n", vid, pid);
		exit(__LINE__ * -1);
		return __LINE__ * -1;
	}
}


void rlight_setcolor (uint8_t in_red, uint8_t in_green, uint8_t in_blue)
{
	uint8_t txbuf[4] = {RLIGHT_FADETO, in_red, in_green, in_blue};

	radio_tx (4, RLIGHT_PACKETTYPE, txbuf);
}
