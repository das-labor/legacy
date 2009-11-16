
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main (void)
{
	DDRB |= _BV(PB0);
	while (1)
	{
		PORTB |= _BV(PB0);
		_delay_us(950);
		PORTB &= ~_BV(PB0);
		_delay_us(50);
	}
}
