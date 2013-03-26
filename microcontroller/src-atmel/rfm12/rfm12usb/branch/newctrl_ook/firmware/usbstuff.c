/* This is the firmware for the RFMUSB device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) 2013 Soeren Heisrath, Peter Fuhrmann, Hans-Gert Dahmen
 *
 * Parts of this code were extracted from the example project
 * developed by Christian Starkjohann under the copyright (c) 2008 by
 * OBJECTIVE DEVELOPMENT Software GmbH.
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "../common/requests.h"
#include "vusb/usbdrv.h"
#include "usbdebug.h"

/* modes for the internal state machine
 */
#define MODE_IDLE 0
#define MODE_RX   1
#define MODE_TX   2

/* states of the usb rx/tx buffer */
#define BUFSTATE_IDLE 0  /* buffer may be used */
#define BUFSTATE_RX   1  /* buffer contains received data from the radio interface */
#define BUFSTATE_TX   2  /* buffer contains data that is to be sent through the radio interface */

static uint8_t usb_rx_cnt = 0;                    /* usb rx c(o)unt */
static uint8_t usb_rx_target_cnt = 0;             /* amount of bytes to be received */
static uint8_t rfm12usb_mode = MODE_IDLE;         /* current mode */
static uint8_t usb_buf_state = BUFSTATE_IDLE;     /* state of the usb buffer */
static uint8_t usb_buf[RFMUSB_USB_BUF_SIZE];      /* buffer for data from/to host */
static uint8_t usb_buf_len = 0;                   /* fill state of usb buffer */


void usbstuff_init ()
{

}

/* handle host requests - mostly like the old handler
 */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

	USB_LED(1);

	switch(rq->bRequest)
	{
		/* host -> rfm12 */
		case RFMUSB_RQ_RADIO_PUT:
			usb_rx_cnt = 0;
			usb_rx_target_cnt = rq->wLength.bytes[0];
			/* note: this will terminate any ongoing receiption */
			rfm12usb_mode = MODE_TX; 

			if(usb_rx_target_cnt > RFMUSB_USB_BUF_SIZE)
				usb_rx_target_cnt = RFMUSB_USB_BUF_SIZE;

			/* use usbFunctionWrite() */
			return USB_NO_MSG;

		/* rfm12 -> host (polling) */
		case RFMUSB_RQ_RADIO_GET:
			if (bufstate == BUFSTATE_RX)
			{
				uint8_t tmp;
				
				usbMsgPtr = (uchar *) usb_buf;

				tmp = usb_buf_len;
				usb_buf_len = 0;
				usb_buf_state = BUFSTATE_IDLE;

				/* send tmp bytes to host */
				return tmp;
			}
		break;

		case RFMUSB_RQ_NOTIFY_GET:
			/* TODO: debug printing */
			break;

		//host wants to configure radio
		case RFMUSB_RQ_RADIO_CFG:
			//use the first byte as configure command index
			usbrfm_configureRfm12(rq->wIndex.bytes[0], rq->wValue.word);

			//switch led
			LED_STATUS_TOGGLE;

			//use default return value
			break;

		//use default return value
		default:
			break;
	}

	return 0;   /* default for not implemented requests: return no data back to host */

}

uchar usbFunctionWrite(uchar *data, uchar len)
{
}
