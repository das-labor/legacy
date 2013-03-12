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
#include "ds1631.h"
#include "motion.h"


static volatile uint16_t tickscounter = 0;
ISR(TIMER2_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//motion_init();

	init_io();

	// init twi
	if (!TWIM_Init())
	{
		while (1);
	}

	// Init twi Tempearture Sensor
	//init_ds1631(I2C_ADRESSE_DS1631);

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
	switch_netvars_init();
	lamp_out_init();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();

		if (tickscounter > 20)
		{
			tickscounter = 0;
			netvar_handle_events();
			switch_handler();
			//temp_sensor_read();
			//motion_tick();
		}
		wdt_reset();
	}
}

