#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "can/can.h"
#include "can/spi.h"
#include "uart/uart.h"
#include "twi_master/twi_master.h"
#include "beamer_rs232.h"
#include "teufel-i2c.h"
#include "can_handler.h"

static volatile uint8_t tickscounter;

ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

//system initialization
static void init(void)
{
	// disable analog comparator (to save power)
	ACSR |= _BV(ACD);

	// debug LED output
	DDRD |= _BV(PD7);

	TCCR0 = _BV(CS02) | _BV(CS00); // clk / 256
	TIMSK = _BV(TOIE0);

	// initialize spi port
	spi_init();

	// initialize uart
	uart_init();

	// initialize twi
	if (!TWIM_Init())
		while (1);

	can_read_addr();

	//initialize can communication
	can_init();

	// turn on interrupts
	sei();
}


int main(void)
{
	// system initialization
	init();

	setDefaultAfterPoweron(); // teufel
	lap_get_status();

	// the main loop continuously handles can messages
	while (1) {
		can_handler();
		beamer_receive_handler();
		if (tickscounter > 10) {
			tickscounter = 0;
			TeufelPoweronTick();
			beamer_poll_state();
		}
		//PORTD &= ~_BV(PD7); // Disable debug LED
	}
}
