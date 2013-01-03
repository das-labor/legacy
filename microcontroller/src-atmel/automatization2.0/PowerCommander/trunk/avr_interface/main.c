#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "../include/PowerCommander.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "switch.h"
#include "i2c_funktionen.h"
#include "statusled.h"

volatile uint8_t tickscounter;

ISR(TIMER1_COMPA_vect)
{
	//976,5625 Hz

	// ueberlaeufe sind ok!
	tickscounter++;
}

void init(void)
{
	TCCR1B |=  _BV(WGM12) | 3; // CTC, clk/64

	//1000 Hz
	OCR1A = 250; // pwm timer compare target

	TIMSK |= _BV(OCIE1A); // Enable Timer1 Overflow Interrupt

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	DDRA |= LED_GRUEN | LED_ROT | LED_BLAU; // Status LED G R B
#ifdef TESTBOARD
	PORTA &= ~(LED_GRUEN | LED_ROT | LED_BLAU);
#else
	PORTA |= LED_GRUEN | LED_ROT | LED_BLAU;
#endif
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge Hauptschalter, RCD Steckdosen
	DDRD &= ~(_BV(PD6) | _BV(PD7)); // Eingänge RCD Licht, Server
	DDRC &= ~_BV(PC2); // Eingang 24V Schützspannung ok


	DDRB &= ~_BV(PB2); // Eingang Lounge Taster
	DDRD &= ~_BV(PD3); // Eingang Vortragsraum Taster

/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init())
	{
		while (1);
	}

	// get output states
	sync_stat_cache();

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

	set_led( (rgb){ .r = 1, .g = 1, .b = 1 , .fade=1 , .blink=0} );

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();

		/* execute every 20 ms */
		if (tickscounter >= 20) {
			tickscounter = 0;
			switch_handler();
		}
		wdt_reset();
	}
	return 1;
}
