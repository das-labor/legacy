#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "can/spi.h"
#include "can/can.h"
#include "netvar/can_handler.h"
#include "netvar/netvar.h"
#include "dimmer/dimmer.h"


static volatile uint8_t tickscounter;

ISR(TIMER1_COMPA_vect)
{
	tickscounter++;
}


static void init()
{
	DDRB |= _BV(PB0); // Labnode LED output
	//DDRD |= _BV(PD5); // EVG: 0-10V

	TCCR1B |= _BV(WGM12) | _BV(CS10) | _BV(CS11); // CTC, clk/64
	// 1000 Hz
	OCR1A = 250; // pwm timer compare target
	TIMSK |= _BV(OCIE1A); // Enable Timer1 Overflow Interrupt

	// initialize spi port
	spi_init();

	// initialize can communication
	can_init();

	read_can_addr();
	wdt_enable(WDTO_250MS); // 250 ms
	dimmer_init();

	//allow interrupts
	sei();
}

int main(void)
{
	// system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();

		if (tickscounter >= 20) {
			tickscounter = 0;
			netvar_handle_events();
		}

		wdt_reset();
	}
}

