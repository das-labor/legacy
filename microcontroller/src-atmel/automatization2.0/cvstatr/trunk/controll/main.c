#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "twi_master/twi_master.h"
#include "can/spi.h"
#include "can/can.h"
#include "can_handler.h"
#include "ds1631.h"
#include "temp_read.h"
#include "temp_regler.h"
#include "switch_handler.h"

// int regler_tick = true

static volatile uint8_t tickscounter;

ISR(TIMER1_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	// Initiate TWI Master Interface
	if (!TWIM_Init())
	{
		while (1);
	}

	init_ds1631(I2C_ADRESSE_DS1631);

	//TCCR0 |= _BV(CS02) | _BV(CS00);
	//TIMSK |= _BV(TOIE0);

	ACSR   = _BV(ACD); // Disable Analog Comparator (power save)

	DDRD  &= ~(KLINGEL | TUER_KONTAKT | STANDBY);
	PORTD |= KLINGEL | TUER_KONTAKT | STANDBY;

	TCCR1B = _BV(CS12) | _BV(CS10);
	TCCR1A = 0;
	TIMSK |= _BV(TOIE1);


	spi_init(); // initialize spi port
	can_read_addr();
	can_init(); // initialize can communication

	sei(); // turn on interrupts
	wdt_enable(WDTO_250MS);
}

int main(void)
{
	uint16_t send_temp_counter = 0;

	init(); //system initialization

	while (1)
	{
		can_handler();
//		temp_regler();
		if (tickscounter > 9) {
			tickscounter = 0;
			switch_handler();
			send_temp_counter++;
			if (send_temp_counter > 1000) {
				temp_sensor_read();
				send_temp_counter = 0;
			}
		}
		wdt_reset();
	}
}

