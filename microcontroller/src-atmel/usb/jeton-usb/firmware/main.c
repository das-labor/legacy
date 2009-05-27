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
#include "i2csw.h"


#define CARD_POWER_ON() PORTC |= (1<<4)
#define CARD_POWER_OFF() PORTC &= ~(1<<4)


#define MAX_TRANSFER_SIZE 0x1000


////////
// global variables
////////

uint16_t address;

//transfer counters
uint16_t i2cusb_usbRxLen = 0, i2cusb_usbRxCnt = 0;
uint16_t i2cusb_usbTxLen = 0, i2cusb_usbTxCnt = 0;

//buffer for the card data
uint8_t i2cusb_cardBuf[8];

//the card insertion status
uint8_t	i2cusb_cardStatus;

uint8_t open_tray;

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
		//host wants to write a card
		case I2CUSB_RQ_CARD_WRITE:
			//if a card is inserted
			if(! (i2cusb_cardStatus & 0x80))
			{
				CARD_POWER_ON();
				address = rq->wIndex.word;
	
				// store the amount of data to be received
				i2cusb_usbRxLen = rq->wLength.word;

				// limit to buffer size
				if(i2cusb_usbRxLen > MAX_TRANSFER_SIZE)
					i2cusb_usbRxLen = MAX_TRANSFER_SIZE;

				// tell driver to use usbFunctionWrite()
				return USB_NO_MSG;
			}
			else
			{
				//use default return value
				break;
			}
					
		//host wants to read a card
		case I2CUSB_RQ_CARD_READ:
			//if a card is inserted
			if(! (i2cusb_cardStatus & 0x80) )
			{
				CARD_POWER_ON();

				address = rq->wIndex.word;

			// store the amount of data to be transferred
				i2cusb_usbTxLen = rq->wLength.word;
				
				// limit to buffer size
				if(i2cusb_usbTxLen > MAX_TRANSFER_SIZE)
					i2cusb_usbTxLen = MAX_TRANSFER_SIZE;

				// tell driver to use usbFunctionRead()
				return USB_NO_MSG;
			}
			else
			{
				//use default return value
				break;
			}


		//host polls for status
		case I2CUSB_RQ_CARD_STATUS:
			//set usb message pointer
			usbMsgPtr = (uchar *)&i2cusb_cardStatus;

			//tell driver to send 1 byte
			return 1;


		//host wants to open cash tray
		case I2CUSB_RQ_TRAY_OPEN:
			//open tray
			open_tray = 1;
			
			//return no data
			return 0;

		case I2CUSB_SET_SCANNER_POWER:
			
			if(rq->wIndex.word == 1){
				PORTD |= 0x80;
			}else{
				PORTD &= ~0x80;
			}
			
			//return no data
			return 0;


		//use default return value
		default:
			break;
	}

	return 0;   /* default for not implemented requests: return no data back to host */
}

//receive raw card data from the host
//FIXME: allow "async"/chunked writes to card
uchar usbFunctionWrite(uchar *data, uchar len)
{
  uchar i;

  //if this is the last incomplete chunk
  if(len > i2cusb_usbRxLen)
  {
			//limit to the amount we can store
      len = i2cusb_usbRxLen;
	}

	//copy data
	i2cusb_usbRxLen -= len;
  for(i = 0; i < len; i++)
  {
  	i2cusb_cardBuf[i] = data[i];
	}

	PORTC |= (1<<2);
	i2cEeWrite(address, i2cusb_cardBuf, len);
	address += len;

  if(i2cusb_usbRxLen == 0)
  {
		CARD_POWER_OFF();
				
		//return 1 if we have all data
		return 1;
	}
	else
	{
    	return 0;
	}
}


uchar usbFunctionRead(uchar *data, uchar len)
{
    if(len > i2cusb_usbTxLen)                // len is max chunk size
        len = i2cusb_usbTxLen;               // send an incomplete chunk
		    
    i2cusb_usbTxLen -= len;
    
		i2cEeRead(data, address, len);
		address+=len;
				
		if(i2cusb_usbTxLen == 0){
			CARD_POWER_OFF();
		}

    return len;                             // return real chunk size
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

	//LED_PORT_DDR |= (LED_BIT_RED | LED_BIT_GREEN);   /* make the LED bit an output */
}



int main(void)
{
	uint16_t tray_timer = 0;
	//main initialization
	init();

	PORTC |= (1<<5); //Pullup for card switch
	DDRC  |= (1<<4);// output for card Vcc
	DDRC  |= (1<<2);// output for LED

	DDRB |= 0x01;//output for tray solenoid
	DDRD |= 0x80;//output for scanner power


	i2cInit();

	//enable ints
	sei();

	while (42)
	{
		usbPoll();
		
		if(!(PINC & (1<<5))){ //card inserted switch
			if(i2cusb_cardStatus == 0x80){
				CARD_POWER_ON();
				_delay_ms(2);
				i2cusb_cardStatus = i2cEeDetect();
				CARD_POWER_OFF();
			}
		}else{
			i2cusb_cardStatus = 0x80;
		}
		
		
		if(open_tray){
			open_tray = 0;
			tray_timer = 6000;
			PORTB |= (1<<0); //power solenoid
		}
		
		if(tray_timer){
			tray_timer--;
		}else{
			PORTB &= ~(1<<0); //solenoid off
		
		}
		
		
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
