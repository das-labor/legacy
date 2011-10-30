/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>






void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	PORTB |= _BV(PB4);
}
	 
int main(void)
{
	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		if (!(PIND & _BV(PD3)))
		{
			DDRC |= _BV(PC3) | _BV(PC4) | _BV(PC5) | _BV(PC6);
			PORTC |= _BV(PC3) | _BV(PC4);
			PORTC &= ~(_BV(PC5) | _BV(PC6));
		}
		else if (!(PINB & _BV(PB2)))
		{
			DDRC |= _BV(PC3) | _BV(PC4) | _BV(PC5) | _BV(PC6);
			PORTC |= _BV(PC5) | _BV(PC6);
			PORTC &= ~(_BV(PC3) | _BV(PC4));
		}
		else
		{
			DDRC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5) | _BV(PC6));
			PORTC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5) | _BV(PC6));
		}
	}
	return 1;
}

