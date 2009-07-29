#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

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
	_delay_ms(250);
	rfm12_set_wakeup_timer(10);	
			
	sei();
	
	uart_putstr ("\r\n");
	uart_putstr ("Hello\r\n");
	
	DDRB   &= ~(_BV(1));

	while (42)
	{
		if(!(PINB & (_BV(1))))
		{
			ticker = 1;
		}
		
		if(ticker && (PINB & (_BV(1))))
		{
			ticker = 0;
			
			//uart_putstr ("tx ");
			rfm12_tx (1, 0x23, &i);
			
			i++;
		}

		rfm12_tick();
		
		sleep_mode();
	}
}
