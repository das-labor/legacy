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

void send_testcode ()
{
	uint8_t byte;
	uint8_t tx_bytes[3] = {0x41, 0x45, 0x51};
	uint8_t msk = 0x80;


	rfm12_ask_tx_mode(1);
	rfm12_tx_off();
	//#define DLY 78
	#define DLY 360

	for (byte = 0; byte < 3;)
	{
		rfm12_tx_on();
		_delay_us (240);
		_delay_us(DLY);

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
		_delay_us (240);
		_delay_us (DLY);

		DEBUG_LED(0);
		rfm12_tx_off();
		_delay_us (140);
		_delay_us (DLY);

		msk >>= 1;
		if (msk == 0x00)
		{
			msk = 0x80;
			byte++;
		}
	}
	
	rfm12_tx_on();
	_delay_us (240);
	_delay_us (DLY);
	rfm12_tx_off();

	for (byte = 12; byte != 0; byte--)
	{
		_delay_us(240);
		_delay_us(DLY);
	}
	
	rfm12_ask_tx_mode(0);
}

int main ()
{
	HW_INIT();
	DEBUG_LED(1);
	usbstuff_init ();
	rfm12_init();
	FSK_INIT();
	DEBUG_LED(0);
	wdt_disable();
#if 0
	_delay_ms(200);
	rfm12_ask_tx_mode(1);
	_delay_ms(50);
	rfm12_ask_tx_mode(0);
	_delay_ms(50);
#endif
	sei();

	while (1)
	{
		send_testcode();
		send_testcode();
		send_testcode();
		rfm12_tick();
		usbPoll();
		handle_rx();
	}
}
