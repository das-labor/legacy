#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "io.h"
#include "motion.h"


static volatile uint16_t tickscounter = 0;
ISR(TIMER2_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	motion_init();

	DDRB |= _BV(PB0); // lapnode red LED out

	init_io();

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();

	read_can_addr();

	//turn on interrupts
	sei();
	wdt_enable(WDTO_250MS); // 250 ms
}

int main(void)
{
	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		if (tickscounter > 15)
		{
			tickscounter = 0;
			switch_handler();
			motion_tick();
		}
		wdt_reset();
	}
}

