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
#include "../common/rfm12usb_config.h"
#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "rfm12.h"
#include "usbstuff.h"
#include "oddebug.h"
#include "usbdrv.h"
#include "ook.h"

#define DELAY_FN mydelay
inline void mydelay (uint16_t in_delay)
{
	uint16_t tdelay = in_delay << 1;
	tdelay += in_delay >> 1; /* + 0.5 */
	TCCR1B = 0x00;      /* stop timer */
	TCNT1 =  1;         /* set value to 1 */
	TCCR1B = _BV(CS11); /* normal mode, clk/8 */

	while (TCNT1 < tdelay)
		asm volatile ("nop");
}

void send_testcode ()
{
	uint8_t byte;
	uint8_t tx_bytes[3] = {0x41, 0x45, 0x51};
	uint8_t msk = 0x80;


	rfm12_ask_tx_mode(1);
	rfm12_tx_off();
	//#define DLY 78
	#define DLY 100

	for (byte = 0; byte < 3;)
	{
		rfm12_tx_on();
		DELAY_FN (240);
		DELAY_FN (160);
		DELAY_FN(DLY);

		if (tx_bytes[byte] & msk)
		{
			/* send 1 (..1..) */
			rfm12_tx_on();
			DEBUG_LED(1);
		} else
		{
			/* send 0 (..0..) */
			rfm12_tx_off();
			DEBUG_LED(0);
		}
		DELAY_FN (240);
		DELAY_FN (160);
		DELAY_FN (DLY);

		DEBUG_LED(0);
		rfm12_tx_off();
		DELAY_FN (140);
		DELAY_FN (160);
		DELAY_FN (DLY);

		msk >>= 1;
		if (msk == 0x00)
		{
			msk = 0x80;
			byte++;
		}
	}
	
	rfm12_tx_on();
	DELAY_FN (240);
		DELAY_FN (160);
	DELAY_FN (DLY);
	rfm12_tx_off();

	for (byte = 12; byte != 0; byte--)
	{
		DELAY_FN(240);
		DELAY_FN (160);
		DELAY_FN(DLY);
	}
	
	rfm12_ask_tx_mode(0);
}

int main ()
{
	uint8_t i=0;
	uint8_t testcode[3] = {0x41, 0x45, 0x51};

	HW_INIT();
	DEBUG_LED(1);
	usbstuff_init ();
	FSK_INIT();
	DEBUG_LED(0);
	wdt_disable();

	/* crude bugfix */
	_delay_ms(100);
	rfm12_init();
	rfm12_tx_on();
	_delay_ms(10);
	rfm12_init();
	sei();
	//send_ook (24, 250, testcode, OOK_2722);

	for (i=0; i<8; i++)
	{
	//	send_ook (24, 400, testcode, OOK_2722);
	//	send_testcode();
	}

	while (1)
	{
		send_ook (24, 500, testcode, OOK_2722);
		rfm12_tick();
		usbPoll();
		handle_rx();
	}
}
