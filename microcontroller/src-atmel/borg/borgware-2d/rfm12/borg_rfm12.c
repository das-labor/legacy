
#include <avr/io.h>

#include "rfm12.h"

volatile uint8_t rfm12_joystick_val;

void borg_rfm12_tick(){
	if (rfm12_rx_status() == STATUS_COMPLETE)
	{

		//uart_putstr ("new packet:\r\n");

		uint8_t * bufp = rfm12_rx_buffer();

		// dump buffer contents to uart			
		if(rfm12_rx_len() >= 1){
			//-> F2 F1 RT LF DN UP				

			rfm12_joystick_val = *bufp;
		}

		// tell the implementation that the buffer
		// can be reused for the next data.
		rfm12_rx_clear();
	}
	
	rfm12_tick();

}

void borg_rfm12_init(){
	rfm12_init();
}
