/* -*- Mode: C; tab-width: 2 -*- */

//#include <util/delay.h>

#include <avr/io.h>

#include "config.h"


#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"


void init(void)
{



	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	read_can_addr();
	//turn on interrupts
  //	sei();
}
	 
int main(void)
{

	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		switch_handler();		
	}
	return 1;
};
