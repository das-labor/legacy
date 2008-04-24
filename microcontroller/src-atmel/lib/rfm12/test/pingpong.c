#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>


// #include "../rfm12_hw.h"
#include "../rfm12.h"
#include "uart.h"


void main ( void )
{
	uint8_t *bufcontents;
	uint8_t tmp;
	uint8_t i;

	uint16_t txbuf = 0x00;
	uint16_t ticker = 0;

	const char tv = "testvektor\r\n";
	
	/* Laborboard setup */
	DDRC = 0xFF;
	PORTC = 0xAA;


	//spi_init();

	rfm12_init();
	uart_init();
	
	sei();
	
	DDRB &= ~((_BV(PB0)) | (_BV(PB1)) | (_BV(PB2)) | (_BV(PB3)));
	PORTB |= 0x0f;


	uart_putstr ((const) "\r\n");
	uart_putstr ((const) "Hello\r\n");

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
				uart_putc ((char) ((&bufcontents) + i));
				PORTC = ((&bufcontents) + i);
			}
			
			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();

			tmp >>= 1;
			if (!tmp) tmp = 0x80;
		}


//		rfm12_tx (1, 0, tmp);
		
		if (!(PINB & (_BV(PB0))) && (ticker % 3000 == 0))
		{
			uart_putc ('#');
			rfm12_tx (sizeof(tv), 0, tv);
		}

		rfm12_tick();
	}
}
