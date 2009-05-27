/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Hans-Gert Dahmen <sexyludernatascha@gmail.com>, Soeren Heisrath <forename@surname.org>,
 */

#pragma once

#include <usb.h>

//FIXME: make this settable
#define DEFAULT_USB_TIMEOUT 1000

//define handle type
typedef usb_dev_handle i2cusb_dev_handle;



/* @description Connects to a rfmusb device
 * returns != 0 on error */
int i2cusb_Connect(i2cusb_dev_handle ** handle);

/* @description Connects to a rfmusb device with the given vid, pid, vendor and product string
 * returns != 0 on error */
int i2cusb_ConnectEx(i2cusb_dev_handle ** handle, int vid, int pid, char *vendor, char *product);

/* @description Formats a given chunk of data accordingly and hands it over to the
 * transmitting function. */
int i2cusb_WriteCard (i2cusb_dev_handle *udhandle, unsigned int len, unsigned char *data);

/* @description Receives a packet */
int i2cusb_ReadCard  (i2cusb_dev_handle *udhandle, unsigned int len, unsigned char *data);

/* @description Receives the card (present or not?) status */
int i2cusb_GetCardStatus (i2cusb_dev_handle *udhandle, char *cardStatus);

/* @description Open the cash tray*/
int i2cusb_ShowMeTheMoney (i2cusb_dev_handle *udhandle);

int i2cusb_SetScannerPower (i2cusb_dev_handle *udhandle, int state);