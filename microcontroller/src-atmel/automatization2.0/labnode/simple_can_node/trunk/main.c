#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

static void init(void)
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
