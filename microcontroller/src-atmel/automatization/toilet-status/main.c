#include <avr/io.h>


#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

#include "static_scripts.h"
#include "testscript.h"
#include "lights.h"


void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	read_can_addr();
	//turn on interrupts
	sei();
}


int main(void)
{
	init();
	lights_init();
	
	
	while (1)
	{
		can_handler();
	}
}
