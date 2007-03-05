#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#include "radio.h"

/*
void radio_init ( void )
{
	cli();
	radio_txcount = 0;

	TCNT1 = 100;			// Load overflow counter of timer0
	TCCR1 = (1<<CS02);		// Set Timer1 to CPUCLK/256
	TIMSK |= 1<<TOIE1;		// Enable interrupt flag

//   Soerens (funktionierender) code
//	TCCR0B |= (1<<CS01);// | (1<<CS00);  // clk / 64
//	TCCR0A |= (1<<WGM01); // ctc mode
//	TIMSK  |= (1<<OCIE0A);
//	TIFR   |= (1<<OCF0A);
//	OCR0A = RADIO_SEND_TIME;
	sei();
}
*/

const uint8_t sendcode[5] = { 0x65, 0xb2, 0xcb, 0x2c, 0xb0 };

uint8_t radio_txcount;

// AVRX_SIGINT(SIG_INTERRUPT1)
void radio_tick()
{
	static uint8_t bytecounter = 0, bitmask = 0x80;

//	IntProlog();             // Switch to kernel stack/context

	if (bytecounter == 0)
	{
		if (bitmask == 0x01)
			PORTD |= _BV(PD6);

	} else if (bytecounter < 6)
	{
		if (sendcode[bytecounter-1] & bitmask)
		{
			PORTD |= _BV(PD6);
		} else
		{
			PORTD &= ~(_BV(PD6));
		}
	} else
	{
		PORTD &= ~(_BV(PD6));
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

//	Epilog();
}
