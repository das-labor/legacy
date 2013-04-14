#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "twi_master/twi_master.h"
#include "can/spi.h"
#include "can/can.h"
#include "netvar/can_handler.h"
#include "netvar/netvar.h"
#include "ds1631.h"
#include "io.h"

static volatile uint8_t tickscounter;

ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	TCCR0 = _BV(CS01) | _BV(CS00); /* clk / 64 */
	TIMSK = _BV(TOIE0);

	// ############ Küchenlicht ################
	// RGB LED im Taster
	DDRC |= R_LED | G_LED | B_LED; // Ausgang
	// Taster
	DDRC &= ~_BV(PC0); // Eingang
	PORTC |= _BV(PC0); // pullup

	// ############ Alarm ################
	// 3 Taster LEDs
	DDRD |= _BV(PD5) | _BV(PD6) | _BV(PD7); // Ausgang
	// Taster
	DDRB &= ~_BV(PB1);      // Eingang
	PORTB |= _BV(PB1);      // pullup


	// init twi
	if (!TWIM_Init())
	{
		while (1);
	}

	// Init twi Tempearture Sensor
	init_ds1631(I2C_ADRESSE_DS1631);

	// initialize spi port
	spi_init();

	// initialize can communication
	can_init();

	read_can_addr();

	// turn on interrupts
	sei();
	wdt_enable(WDTO_250MS); // 250 ms
}


int main(void)
{
	// system initialization
	init();
#ifndef NO_NETVAR
	switch_netvars_init();
	lamp_out_init();
#endif
	while (1)
	{
		can_handler();
		if (tickscounter > 9) {
			switch_handler();
			//temp_sensor_read();
#ifndef NO_NETVAR
			netvar_handle_events();
#endif
			tickscounter = 0;
		}
		wdt_reset();
	}
}

