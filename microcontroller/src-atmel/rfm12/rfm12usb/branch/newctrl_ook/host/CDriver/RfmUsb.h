#include <usb.h>

/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>,
 */

#pragma once

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
int rfmusb_Connect(rfmusb_dev_handle **handle);

/* @description Connects to a rfmusb device with the given vid, pid, vendor and product string
 * returns != 0 on error */
int rfmusb_ConnectEx(rfmusb_dev_handle **handle, int vid, int pid);

/* @description Closes a device
 * returns < 0 on error */
int rfmusb_Close (rfmusb_dev_handle *handle);

/* @description Formats a given chunk of data accordingly and hands it over to the
 * transmitting function. */
int rfmusb_TxPacket (rfmusb_dev_handle *handle, unsigned char type, unsigned char len, unsigned char *data);

/* @description Receives a packet */
int rfmusb_RxPacket (rfmusb_dev_handle *handle, rfmusb_packetbuffer * packetBuffer);

//rfm12 live control
int rfmusb_rfm12_config(rfmusb_dev_handle *handle, unsigned char cmd, unsigned short value);
void rfmusb_rfm12_cmd_to_string(uint8_t cmd, char * s);
void rfmusb_rfm12_get_parameter_string(uint8_t cmd, char * str);
unsigned int rfmusb_rfm12_get_cmd_count();
