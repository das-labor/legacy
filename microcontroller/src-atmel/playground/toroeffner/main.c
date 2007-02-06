#include <stdlib.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include <avr/io.h>

#include "radiocontrol.h"

void main ( void )
{
	radio_init();
	DDRD |= _BV(PD6);
	DDRB = 0x00;
	sei();
	while (42 == 42); /* as long as this universe exists. */
}
