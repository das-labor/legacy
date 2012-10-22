/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "io.h"
#include "i2c_funktionen.h"
#include "motion.h"

void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	motion_init();
//	DDRA &= ~(_BV(PA4)); // Eingänge Türkontakt


/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
/*	if (!TWIM_Init())
	{
		while (1);
	}
*/
	DDRB |= _BV(PB0); // LED out

	TCCR0B = _BV(CS01) | _BV(CS00); /* clk / 64 */
	TIMSK0 = _BV(TOIE0);

	init_io();

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
	uint16_t cnt = 0;

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
//			temp_sensor_read();
		switch_handler();
		wdt_reset();
		cnt++;
		if (cnt < 0xffff)
			continue;
		motion_tick();
	}
	return 1;
};

//lcap testgeshifte
void test(void)
{
}
