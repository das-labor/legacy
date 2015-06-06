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
#include "i2c_pca9555.h"
#include "i2c_tea6420.h"

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

	pca9555_write_byte(0xE0E0, PCA9555_CFG0);
	pca9555_write_byte(0x1c1c, PCA9555_OUT0);

	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_1);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_2);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_3);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_4);


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
