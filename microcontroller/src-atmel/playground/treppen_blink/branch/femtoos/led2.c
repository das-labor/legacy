/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "config.h"

#include <avr/pgmspace.h>
#include "util/delay.h"


typedef struct {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
} element_t;


void update()
{
	uint8_t i;
	_delay_us(10);
	for (i = 0; i < 5; i++)
	{
		PORTD |= _BV(DATA);
		PORTD &= ~_BV(DATA);
	}
}

void element_set(element_t *myel)
{
	uint8_t k;
	for (k = 0 ; k< BITSPERLAMP; k++)
	{
		OUTPORT = ( ( (myel->blue >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = ( ( (myel->green >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
	for (k = 0; k < BITSPERLAMP; k++)
	{
		OUTPORT = ( ( (myel->red >> (BITSPERLAMP-k-1)) & 1 ) << DATA);
		PORTD |= _BV(CLK);
		PORTD &= ~_BV(CLK);
	}
}

const uint16_t bar[9] PROGMEM = {0x002F, 0x01FF , 0x0C10, 0x0AFF, 0x0FFF, 0x0FFF, 0x0AFF, 0x0C10, 0x01FF, 0x002F};

uint16_t calc_dot(uint_t pos)
{
	pgm_read_word(&bar + i);
}

void moving_bar()
{
	int i, j;
	element_t myel;
	
	for (i = 0; i < LAMPS; i++)
	{
		for (j = 0; ; j++)
		{
			myel->blue = calc_dot(pos);
//			myel->red = calc_dot(pos);
//			myel->green = calc_dot(pos);
		}	
		

		update();
	}
}


AVRX_GCC_TASKDEF(led, 200, 4)
{
	moving_bar();
}
