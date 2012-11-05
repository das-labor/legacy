/* -*- Mode: C; tab-width: 2 -*- */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "config.h"

#include "dimmer.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"


ISR(TIMER0_COMP_vect) {
  PORTD |= _BV(PD5);
}

ISR(TIMER0_OVF_vect) {
  PORTD &= ~_BV(PD5);
}

void init(void)
{
	DDRB |= _BV(PB0); // LED out
	DDRD |= _BV(PD5); // 0-10V

	TCCR0 = _BV(CS02);
	TIMSK |= _BV(OCIE0) | _BV(TOIE0);
	OCR0 = 50;
	
	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	read_can_addr();
	wdt_enable(5); // 500ms
}
	 
int main(void)
{
	//system initialization
	init();

/*	
	DDRA |= (1<<PA4)|(1<<PA5);
	DDRC |= (1<<PC4)|(1<<PC5);
	PORTA |= (1<<PA4)|(1<<PA5);
	PORTC |= (1<<PC4)|(1<<PC5);
	*/
	dimmer_init();
	
	set_dimmer(0, 255);
	set_dimmer(1, 255);
	set_dimmer(2, 255);
	set_dimmer(3, 255);

	sei();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		wdt_reset();
	}
}

