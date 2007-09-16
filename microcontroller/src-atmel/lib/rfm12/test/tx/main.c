//transmit data

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../rfm12.h"
#include "uart.h"

uint8_t txTestData[] = {0,1,2,3,5,7,9,201};
rf_buffer_t * rxBuf;

int main()
{	
	DDRC = 0xff;
	
	uart_init();
	
	uint16_t i, x;
	
	//init rfm12
	rfm12_init();
	
	uart_putc('H');
	
	sei();
	
	PORTB = 1;
	
	while(1)
	{
		//wait for a transmission
		while(PINB & 0x01)
		{
			if(rfm12_rx_status() == 2)
			{
				//get buffer
				rxBuf = rfm12_rx_buffer();
				
				//send protocol type
				uart_putc('P');
				uart_putc(rxBuf->type);
				
				//send buffer len
				uart_putc('L');
				uart_putc(rxBuf->len);
				
				//indicate start of buffer
				uart_putc('R');
				
				for(x = 0; x < rxBuf->len; x++)
				{
					uart_putc(rxBuf->buffer[x]);
				}
				
				//clear buffer
				rfm12_rx_clear();
			}
		}
		
		//blow out data
		rfm12_tx(0x10, 5, txTestData);

		
		
		//wait some centuries
		for(x=0; x < 20; x++)
		
		for(i=0; i < 65535; i++)
		{
			
			asm volatile("NOP");
		}
		
	}

	return 0;
}
