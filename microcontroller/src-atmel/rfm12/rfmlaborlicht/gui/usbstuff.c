/* Dump packets onto the console.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */


#include "usbstuff.h"
static usb_dev_handle *udhandle = NULL;

int radio_tx(unsigned char len, unsigned char type, unsigned char *data)
{
	printf("sending packet\n");
	return rfmusb_TxPacket (udhandle, RLIGHT_PACKETTYPE, len, data);
}

int usbstuff_init ()
{
	if (rfmusb_Connect(&udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device!\r\n");
		return __LINE__ * -1;
	}

	return 1;
}

void rlight_setcolor (uint8_t in_red, uint8_t in_green, uint8_t in_blue)
{
	uint8_t txbuf[4] = {RLIGHT_FADETO, in_red, in_green, in_blue};

	radio_tx (4, RLIGHT_PACKETTYPE, txbuf);
}
