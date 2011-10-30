/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "i2c_temp.h"
#include "temp_regler.h"
#include "temp_read.h"

// int regler_tick = true

volatile uint8_t tickscounter;

ISR(TIMER1_OVF_vect)
{
	//1 Hz
	
	// ueberlaeufe sind ok!	
	tickscounter++;
	
}

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

	TCCR1B = _BV(CS12) | _BV(CS10);
	TCCR1A = 0;
	TIMSK |= _BV(TOIE1);


	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	read_can_addr();
	//turn on interrupts
	sei();
	wdt_enable(5); // 500ms
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
//		temp_regler();
		if (tickscounter == 10) {
			temp_sensor_read();
			tickscounter = 0;
		}
		wdt_reset();
	}
	return 1;
}

