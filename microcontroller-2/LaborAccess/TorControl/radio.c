#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#include "radio.h"

const uint8_t sendcode[5] = { 0x65, 0xb2, 0xcb, 0x2c, 0xb0 };

volatile uint8_t radio_txcount = 0;
AVRX_GCC_TASKDEF(sesame, 50, 5) /* name, stackspace, pri */
{
	while (1)
	{
		if (radio_txcount)
		{
			radio_tick();
			radio_txcount--;
		}
	}
}

void radio_tick()
{
	static uint8_t bytecounter = 0, bitmask = 0x80;

	if (bytecounter == 0)
	{
		if (bitmask == 0x01)
			PORTD |= _BV(PD0);

	} else if (bytecounter < 6)
	{
		if (sendcode[bytecounter-1] & bitmask)
		{
			PORTD |= _BV(PD0);
		} else
		{
			PORTD &= ~(_BV(PD0));
		}
	} else
	{
		PORTD &= ~(_BV(PD0));
		if (bytecounter == 10)
		{
			bytecounter = 0;
			bitmask = 0x80;
		}
	}
	
	bitmask = bitmask >> 1;

	if (bitmask == 0x00)
	{
		bitmask = 0x80;
		bytecounter++;
	}
}
