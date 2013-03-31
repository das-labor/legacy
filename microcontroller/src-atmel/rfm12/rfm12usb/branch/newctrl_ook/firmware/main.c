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

#include "../common/rfm12usb_config.h"
#include "rfm12.h"
#include "usbstuff.h"

int main ()
{
	usbstuff_init ();
	rfm12_init();
	sei();

	while (23)
	{
		rfm12_tick();
		handle_rx();
	}
}
