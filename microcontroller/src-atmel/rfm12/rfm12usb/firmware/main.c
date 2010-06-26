/* This is the firmware for the RFMUSB device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 *
 * Parts of this code were extracted from the example project
 * developed by Christian Starkjohann under the copyright (c) 2008 by
 * OBJECTIVE DEVELOPMENT Software GmbH.
 */


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <string.h>

#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "../common/requests.h"       /* The custom request numbers we use */
#include "rfm12.h"
#include "rfmusb.h"
#include "rfmusb_hw.h"

//FIXME: rework packet transmission system (full packet, short/fast (1byte packet))
#define USB_SENDCHAR 0x23


////////
// global variables
////////

//notify buffer
rfmusb_notifyPacket rfmusb_notifyBuf;

//buffer for transmitting usb data to the host
uint8_t rfmusb_usbTxLen = 0;
uint8_t rfmusb_usbTxBuf[RFMUSB_USBTXBUFFER_SIZE];

//buffer for receiving usb data from the host
uint8_t rfmusb_usbRxLen = 0, rfmusb_usbRxCnt = 0;
uint8_t rfmusb_usbRxBuf[RFMUSB_USBRXBUFFER_SIZE];


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

	//switch through requests
	//don't expect the cases to break, they may also return
	switch(rq->bRequest)
	{
		//host wants to write a raw packet to the rfm12
		case RFMUSB_RQ_RFM12_PUT:
			// initialize position index
			rfmusb_usbRxCnt = 0;

			// store the amount of data to be received
			rfmusb_usbRxLen = rq->wLength.bytes[0];

			// limit to buffer size
			if(rfmusb_usbRxLen > RFMUSB_USBRXBUFFER_SIZE)
				rfmusb_usbRxLen = RFMUSB_USBRXBUFFER_SIZE;

			//toggle status led
			LED_STATUS_TOGGLE;

			// tell driver to use usbFunctionWrite()
			return USB_NO_MSG;
			
		//send a single character
		//FIXME: please cleanup define name and location
		case USB_SENDCHAR:
			//copy data
			rfmusb_usbRxBuf[0] = rq->wIndex.bytes[0];

			//send
			rfm12_tx (1, 0, rfmusb_usbRxBuf);

			//toggle led
			LED_STATUS_TOGGLE;

			//use default return value
			break;			

		//host wants to read rfm12 packet data
		case RFMUSB_RQ_RFM12_GET:
			//if there is data to transmit
			if(rfmusb_usbTxLen)
			{
				uint8_t tmp;

				//set usb message pointer
				usbMsgPtr = (uchar *)rfmusb_usbTxBuf;

				//free buffer
				tmp = rfmusb_usbTxLen;
				rfmusb_usbTxLen = 0;

				//switch led
				LED_STATUS_TOGGLE;

				//tell the driver to send n bytes
				return tmp;
			}
			else
			{
				//use default return value
				break;
			}

		//host polls for notifications
		case RFMUSB_RQ_GETNOTIFICATION:
			//if there is any
			if(rfmusb_notifyBuf.len)
			{
				uint8_t tmp;

				//set usb message pointer
				usbMsgPtr = (uchar *)&rfmusb_notifyBuf;

				//free buffer
				tmp = rfmusb_notifyBuf.len;
				rfmusb_notifyBuf.len = 0;

				//tell driver to send n bytes
				return tmp;
			}

		//use default return value
		default:
			break;
	}

	return 0;   /* default for not implemented requests: return no data back to host */
}


//receive raw packet data from the host
//FIXME: to allow different modes, one should check the current mode and call appropriate subfunctions
uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;

	LED_STATUS_TOGGLE;

    //if this is the last incomplete chunk
    if(len > rfmusb_usbRxLen)
    {
		//limit to the amount we can store
        len = rfmusb_usbRxLen;
	}

	//copy data
	rfmusb_usbRxLen -= len;
    for(i = 0; i < len; i++)
    {
        rfmusb_usbRxBuf[rfmusb_usbRxCnt++] = data[i];
	}

    if(rfmusb_usbRxLen == 0)
    {
		//tx packet
		//FIXME: test if the transmit buffer is free and issue tx rate limit notification to host if not
		rfm12_tx(rfmusb_usbRxBuf[0], rfmusb_usbRxBuf[1], &rfmusb_usbRxBuf[2]);

		//return 1 if we have all data
		return 1;
	}
	else
	{
    	return 0;
	}
}



/* ------------------------------------------------------------------------- */


//send a notification to the host
uint8_t usbrfm_notifyHost(uint8_t notifyType, uchar *data, uint8_t len)
{
	//validate range
	if((len > RFMUSB_NOTIFYBUFFER_SIZE) || (len == 0))
	{
		return USBRFM_ERR_RANGE;
	}

	//only send if the buffer is free and if we have data
	if(rfmusb_notifyBuf.len == 0)
	{
		memcpy(rfmusb_notifyBuf.data, data, len);
		rfmusb_notifyBuf.len = len + RFMUSB_NOTIFYBUFFER_OVERHEAD;
	}
	else
	{
		return USBRFM_ERR_OCCUPIED;
	}

	return USBRFM_ERR_OK;
}


//send a raw rfm12 packet to the host
uint8_t usbrfm_usbTxRfmBuf(uint8_t packetType, uchar *data, uint8_t len)
{
	//validate range
	if((len > RFM12_RX_BUFFER_SIZE) || (len == 0))
	{
		return USBRFM_ERR_RANGE;
	}

	//only send if the buffer is free and if we have data
	if(rfmusb_usbTxLen == 0)
	{
		memcpy(&rfmusb_usbTxBuf[2], data, len);
		rfmusb_usbTxBuf[0] = len;
		rfmusb_usbTxBuf[1] = packetType;
		rfmusb_usbTxLen = len + 2; //add overhead; FIXME: move constant value to define.
	}
	else
	{
		return USBRFM_ERR_OCCUPIED;
	}

	return USBRFM_ERR_OK;
}



/* ------------------------------------------------------------------------- */


void init()
{
	uchar i;

	/* RESET status: all port bits are inputs without pull-up.
	* That's the way we need D+ and D-. Therefore we don't need any
	* additional hardware initialization.
	*/
	usbInit();

	usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
 	i = 0;
	while(--i)
	{             /* fake USB disconnect for > 250 ms */
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();

	//init rfm12
	rfm12_init();

	LED_PORT_DDR |= (LED_BIT_RED | LED_BIT_GREEN);   /* make the LED bit an output */
}


int main(void)
{
	uint32_t led_cnt_red;	//count led turnon time

	//main initialization
	init();

	//enable ints
	sei();

	//power led on
	LED_STATUS_SET(1);

	//reset led on count
	led_cnt_red = 0;

	while (42)
	{
		rfm12_tick();
		usbPoll();

		//if rx buffer is full
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			//toggle receive led
			LED_STATUS_TOGGLE;

			//copy packet into usb transmit buffer
			//TODO: check return value and issue rx traffic limit exeeded notification if the buffer is full
			usbrfm_usbTxRfmBuf(rfm12_rx_type(), (uint8_t *) rfm12_rx_buffer(), rfm12_rx_len());

			//clear rfm12 buffer
			rfm12_rx_clear();
		}

		//if the red led is on for some time
		if((LED_PORT_OUTPUT & LED_BIT_RED) && (led_cnt_red++ >= LED_OFFTIME_RED))
		{
			//turn it off
			LED_STATUS_SET(1);

			//reset counter
			led_cnt_red = 0;
		}
    }

    return 0;
}


/* ------------------------------------------------------------------------- */
