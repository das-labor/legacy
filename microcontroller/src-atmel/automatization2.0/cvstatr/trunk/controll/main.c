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

	DDRD  &= ~(_BV(PD7) | _BV(PD5));
	PORTD |= _BV(PD7) | _BV(PD5);

	TCCR1B = _BV(CS12) | _BV(CS10);
	TCCR1A = 0;
	TIMSK |= _BV(TOIE1);


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
	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
//		temp_regler();
		if (tickscounter > 9) {
			temp_sensor_read();
			switch_handler();
			tickscounter = 0;
		}
		wdt_reset();
	}
}

