#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "io.h"
#include "motion.h"
#include "uart/uart.h"
#include "uart_handler.h"

static volatile uint16_t tickscounter = 0;
ISR(TIMER2_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	motion_init();

	DDRB |= _BV(PB0); // lapnode red LED output


	spi_init(); // initialize spi port
	can_read_addr();

	can_init(); // initialize can communication
	uart_init();
	io_init();

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
		uart_handler();
		if (tickscounter > 15)
		{
			tickscounter = 0;
			switch_handler();
			motion_tick();
		}
		wdt_reset();
	}
}

