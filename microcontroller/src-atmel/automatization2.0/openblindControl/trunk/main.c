/* -*- Mode: C; tab-width: 2 -*- */

//#include <util/delay.h>

#include <avr/io.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "i2c_temp.h"


void init(void)
{

	//PORTC |= _BV(PC0) | _BV(PC1);

	/*
	** Initiate TWI Master Interface with bitrate of 100000 Hz
	*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}
	
	init_sensor();
	
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
