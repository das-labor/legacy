#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "rfm12.h"
#include "uart.h"


int main ( void )
{
	uint8_t *bufcontents;
	uint8_t i;
	uint8_t tv[] = "foobar";

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
		ticker++;
		if (ticker == 0) uart_putstr ("\r\n.");

		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			uart_putstr ("new packet:\r\n");

			bufcontents = rfm12_rx_buffer();

			// dump buffer contents to uart			
			for (i=0;i<rfm12_rx_len();i++)
			{
				uart_putc ( bufcontents[i] );
			}
			
			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();

		}


//		if (!(PINB & (_BV(PB0))) && (ticker % 3000 == 0))
//		{
//			uart_putc ('#');
			rfm12_tx (sizeof(tv), 0, tv);
//		}

		rfm12_tick();
	}
}
