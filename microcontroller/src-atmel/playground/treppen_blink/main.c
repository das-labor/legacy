#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define DATA PD6
#define CLK  PD7
#define LAMPS 5




int main(void)
{
	DDRD |= _BV(DATA) | _BV(CLK);
	PORTD |= _BV(DATA) | _BV(CLK);
	int i, j, k, x = 0;
	while (1)
	{
		for (k = 0; k < LAMPS; k++)
		{
			for (i = 0; i < 3; i++)
			{
				x++;
				for (j = 0; j < 12; j++)
				{
					if (((i == 0) && (x & 1)) || ((i == 1) && (x & 2)) || ((i == 2) && (x & 4)))
						PORTD &= ~_BV(DATA);
					else
						PORTD |= _BV(DATA);
					PORTD |= _BV(CLK);
//					_delay_us(2);
					PORTD &= ~_BV(CLK);
				}
				x *= 3;
			}
		}
	
		// Trigger uebernehmen
		_delay_us(10);
		for (j = 0; j < 5; j++)
		{
			PORTD |= _BV(DATA);
//			_delay_us(2);
			PORTD &= ~_BV(DATA);
		}
		_delay_ms(300);
	/*
		for (k = 0; k < LAMPS; k++)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 12; j++)
				{
					PORTD &= ~_BV(DATA);
					PORTD |= _BV(CLK);
//					_delay_us(2);
					PORTD &= ~_BV(CLK);
				}
			}
		}
		_delay_us(10);
		for (j = 0; j < 5; j++)
		{
			PORTD |= _BV(DATA);
//			_delay_us(2);
			PORTD &= ~_BV(DATA);
		}
		_delay_ms(300);*/
	}
	return 0;
}
