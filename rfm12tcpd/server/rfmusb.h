#include <usb.h>
#include "common.h"
#include "opendevice.h"

/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>,
 */
#ifndef RFMUSB_H
#define RFMUSB_H

//FIXME: this shouldn't be defined here.... put it somewhere else
//common header? dynamically gettable from device? dynamically negotiated between rfm12peer<->rfm12peer<->hostpc ?!)
//put to common header first (also in cpp driver!!)
#define RFM12_MAX_PACKET_LENGTH 30

//FIXME: make this settable
#define DEFAULT_USB_TIMEOUT 1000

//define handle type
typedef usb_dev_handle rfmusb_dev_handle;

 //tx and rx raw packet buffer struct
typedef struct{
	unsigned char len;			                       //length byte - number of bytes in buffer
	unsigned char type;			                       //type field for airlab
	unsigned char buffer[RFM12_MAX_PACKET_LENGTH];     //generic buffer
} rfmusb_packetbuffer;


/* @description Connects to a rfmusb device
 * returns != 0 on error */
int rfmusb_connect(usb_dev_handle **);

/* @description Connects to a rfmusb device with the given vid, pid, vendor and product string
 * returns != 0 on error */
int rfmusb_connectex(usb_dev_handle **, int vid, int pid, char *vendor, char *product);

/* @description Formats a given chunk of data accordingly and hands it over to the
 * transmitting function. */
int rfmusb_txpacket (rfmusb_dev_handle *udhandle, unsigned char type, unsigned char len, unsigned char *data);

/* @description Receives a packet */
int rfmusb_rxpacket (rfmusb_dev_handle *udhandle, rfmusb_packetbuffer * packetBuffer);

#endif /* RFMUSB_H */
