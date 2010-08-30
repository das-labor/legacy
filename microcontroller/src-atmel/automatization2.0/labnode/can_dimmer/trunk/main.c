/* -*- Mode: C; tab-width: 2 -*- */


#include <avr/io.h>
//#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

void init(void)
{
	DDRB |= _BV(PB0); // LED out
		
	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	read_can_addr();
}
	 
int main(void)
{
	volatile uint16_t foo = 0;
	//system initialization
	init();

	DDRA |= (1<<PA4)|(1<<PA5);
	DDRC |= (1<<PC4)|(1<<PC5);
	PORTA |= (1<<PA4)|(1<<PA5);
	PORTC |= (1<<PC4)|(1<<PC5);
	
	//dimmer_init();

	sei();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		
		
		/*

		//periodically send test message
				
		if(foo == 0){
			static can_message msg = {1, 2, 3, 4, 2, {0x55, 0xaa}};
			msg.addr_src = myaddr;
			can_transmit(&msg);
		}
		foo++;
		
		*/

	}

}
