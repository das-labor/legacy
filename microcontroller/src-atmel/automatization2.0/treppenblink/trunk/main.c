#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "twi_master/twi_master.h"
#include "can/spi.h"
#include "can/can.h"
#include "netvar/can_handler.h"
#include "netvar/netvar.h"
#include "io.h"
#include "animationen.h"
#include "ds1631.h"
#include "motion.h"


static volatile uint8_t tickscounter;

ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//motion_init();

	TCCR0 = _BV(CS01) | _BV(CS00); // clk / 64
	TIMSK = _BV(TOIE0);

	init_io();
//	PORT_LED |= B_LED;
	// init twi
	if (!TWIM_Init())
	{
		while (1);
	}

	// Init twi Tempearture Sensor
	//init_ds1631(I2C_ADRESSE_DS1631);

	// initialize spi port
	spi_init();

	// initialize can communication
	can_init();

	read_can_addr();

	// turn on interrupts
	sei();
	wdt_enable(WDTO_250MS); // 250 ms

#ifndef NO_NETVAR
	switch_netvars_init();
	lamp_out_init();
#endif
}

int main(void)
{
	// system initialization
	init();

	while (1)
	{
		can_handler();
		do_ani();
		if (tickscounter > 10)
		{
			tickscounter = 0;
			switch_handler();
//			PORT_LED ^= R_LED;
#ifndef NO_NETVAR
			netvar_handle_events();
#endif
			//temp_sensor_read();
			//motion_tick();
		}
		wdt_reset();
	}
}

