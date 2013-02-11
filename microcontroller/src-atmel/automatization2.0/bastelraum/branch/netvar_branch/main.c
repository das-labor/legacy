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
#include "motion.h"

void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	motion_init();
//	DDRA &= ~(_BV(PA4)); // Eingänge Türkontakt


	DDRB |= _BV(PB0); // LED out

	// this stuff is now handled by timer2 (see io.c)
	// -> timer0 is used by the motion detectors (see motion.c)
	// TCCR0B = _BV(CS01) | _BV(CS00); /* clk / 64 */
	// TIMSK0 = _BV(TOIE0); 

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

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();

		switch_handler();
		wdt_reset();
		motion_tick();
	}
	return 1;
};

