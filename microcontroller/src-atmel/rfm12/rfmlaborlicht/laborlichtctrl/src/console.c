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

#include "../common/protocol.h"
#include "usb_id.h"
#include "../../rfm12usb/common/requests.h"
#include "../../rfm12usb/common/rfm12_buffer_size.h"

#define DEFAULT_USB_TIMEOUT 1000
#define RADIO_TXBUFFER_HEADER_LEN 2

usb_dev_handle *udhandle = NULL;
int mytty;

typedef struct{
	unsigned char len;			                       //length byte - number of bytes in buffer
	unsigned char type;			                       //type field for airlab
	unsigned char buffer[RFM12_BUFFER_SIZE];     //generic buffer
} radio_packetbuffer;

void console_exit (int in_signal)
{
        printf ("\nCansole closing...\n");
        close (mytty);
	reset_keypress ();
        exit (in_signal);
}

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

int main (int argc, char* argv[])
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
		console_exit(__LINE__ * -1);
		return __LINE__ * -1;
	}

	signal (SIGINT, console_exit);
	signal (SIGKILL, console_exit);
	signal (SIGHUP, console_exit);

	tmp=0;
	i=0;


	printf ("Connection established.\r\n");

	uint8_t txbuf[4] = {RLIGHT_FADETO, 0xff, 0x80, 0x00}; 

	radio_tx (4, RLIGHT_PACKETTYPE, txbuf);
	printf("sending %02X %02X %02X %02X\n", txbuf[0], txbuf[1], txbuf[2], txbuf[3]);
	while (43);

}
