/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "i2c_temp.h"
#include "temp_regler.h"

// int regler_tick = true


void init(void)
{
	// Initiate TWI Master Interface

	if (!TWIM_Init())
	{
		while (1);
	}
	
	//init_sensor(0x9e);
	init_sensor(0x96);

  //TCCR0 |= _BV(CS02) | _BV(CS00);
  //TIMSK |= _BV(TOIE0);

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	DDRD &= ~(_BV(PD7) | _BV(PD5));
	PORTD |= _BV(PD7) | _BV(PD5);

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	read_can_addr();
	//turn on interrupts
  //sei();
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
		temp_regler();
	}
	return 1;
}

