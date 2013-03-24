#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "can/spi.h"
#include "can/can.h"
#include "netvar/can_handler.h"
#include "netvar/netvar.h"
#include "dimmer.h"

static void init()
{
	DDRB |= _BV(PB0); // LED out
	//DDRD |= _BV(PD5); // EVG: 0-10V

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();

	read_can_addr();
	wdt_enable(WDTO_250MS); // 250 ms
	dimmer_init();
}

int main(void)
{
	//system initialization
	init();


	//allow interrupts
	sei();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		if (ticks_in_ms >= 10)
		{
			cli();
			ticks_in_ms -= 10;
			sei();

			netvar_handle_events();
		}
		wdt_reset();
	}
}

