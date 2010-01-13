/* -*- Mode: C; tab-width: 2 -*- */


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can/can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

#include "../include/PowerCommander.h"

#include "switch.h"

void init(void)
{

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)
	
	
	DDRA |= _BV(PA2) | _BV(PA3) | _BV(PA4); // Status LED G R B
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge HS, rcd
	DDRD &= ~(_BV(PD6) | _BV(PD7)); // Eingänge rcd licht, server
	DDRC &= ~_BV(PC2); // Eingang power good
		

	DDRB &= ~_BV(PB2); // Eingang Lounge Taster
	DDRD &= ~_BV(PD3); // Eingang Vortrag Taster
	
	//Pullups werden nichtmehr gebraucht
	//PORTB |= _BV(PB2);
	//PORTD |= _BV(PD3);

/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	read_can_addr();

	switch_timer_init();
	
	//turn on interrupts
	sei();
}
	 
int main(void)
{

	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		PORTA &= ~_BV(PA4);
		can_handler();
		switch_handler();
	}
	return 1;
};
