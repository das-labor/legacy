#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "rfm12.h"
#include "uart.h"


int main ( void )
{
	uint8_t *bufp;
	uint16_t ticker = 0;
		
//	uart_init();

	_delay_ms(800);
	
	rfm12_init();
	
	sei();

//	uart_putstr ("Hello\r\n");

	uint8_t joy = 0;
	
	while (42)
	{
		ticker++;
//		if (ticker == 0) uart_putstr ("\r\n.");

		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

//			uart_putstr ("new packet:\r\n");

			bufp = rfm12_rx_buffer();

			// dump buffer contents to uart			
			if(rfm12_rx_len() >= 1){
				joy = *bufp;

				//PB0 Up
				//PB1 Down
				//PB2 Left
				//PB3 Right
				//
				//PB4 Fire1

				//-> F2 F1 RT LF DN UP				
				
				DDRB = (DDRB & 0xe0) | (joy & 0x1f);
			
			}	
				
			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();

		}
		
		rfm12_tick();
	}
}
