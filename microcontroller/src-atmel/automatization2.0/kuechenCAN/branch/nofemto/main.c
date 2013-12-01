#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "twi_master/twi_master.h"
#include "can/spi.h"
#include "can/can.h"
#include "can_handler.h"
#include "netvar/netvar.h"
#include "io.h"
#include "ds1631.h"
#include "temp_read.h"


static volatile uint8_t tickscounter;

ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	TCCR0 = _BV(CS01) | _BV(CS00); // clk / 64
	TIMSK = _BV(TOIE0);

	io_init();

	// init twi
	if (!TWIM_Init())
	{
		while (1);
	}

	ds1631_init(I2C_ADRESSE_DS1631); // Init twi Tempearture Sensor

	spi_init(); // initialize spi port
	can_read_addr();
	can_init(); // initialize can communication

	sei(); // turn on interrupts
	wdt_enable(WDTO_250MS);

#ifndef NO_NETVAR
	switch_netvars_init();
	lamp_out_init();
#endif
}

int main(void)
{
	uint16_t send_temp_counter = 0;

	init(); // system initialization

	while (1)
	{
		can_handler();
		if (tickscounter > 9)
		{
			tickscounter = 0;
			switch_handler();
#ifndef NO_NETVAR
			netvar_handle_events();
#endif
			send_temp_counter++;
			if (send_temp_counter > 1000) {
				temp_sensor_read();
				send_temp_counter = 0;
			}
		}
		wdt_reset();
	}
}

