#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "rfm12.h"
#include "uart.h"


int main ( void )
{
	uint8_t i = 0;
	uint16_t ticker = 0;

	
	uart_init();
	DDRD |= (_BV(PD7));
	PORTD |= _BV(PD7);

	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	
	rfm12_init();
	
	sei();

	uart_putstr ("\r\n");
	uart_putstr ("Hello\r\n");

	while (42)
	{
		i++;

		if((ticker++ % 0xff) == 0)
		{
			rfm12_tx (1, 0x23, &i);
		}

		rfm12_tick();
	}
}
