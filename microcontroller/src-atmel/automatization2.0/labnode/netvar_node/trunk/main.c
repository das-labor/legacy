#include <avr/io.h>
#include <avr/interrupt.h>

#include "can/can.h"
#include "can/spi.h"
#include "netvar/can_handler.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "dimmer/dimmer.h"
#include "lamp_controller.h"


volatile uint8_t ticks_in_ms;

ISR(TIMER0_COMP_vect) {
	static uint8_t ticks;
	ticks_in_ms += 2;
	ticks++;
	if (ticks == 5) {
		ticks = 0;
		//backlight();
	}
}

static void init_timer() {
	//2ms Timer0
	TCCR0 = (1 << WGM01) | 4; //CTC, clk/256
	OCR0  = 125;
	TIMSK |= (1 << OCIE0);
}

static void init() {
	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();
	read_can_addr();
}

static void set_led(void *num_led, uint8_t val) {
	if (val) {
		PORTB |= _BV(0);
	} else {
		PORTB &= ~_BV(0);
	}
}

int main(void) {
	DDRB |= _BV(PB0); // LED out

	PORTB |= _BV(PB3); //set CS for RFM12 high in case it is present
	DDRB |= _BV(PB3);

	init();
	init_timer();
	dimmer_init();

	sei();

	new_netvar_output_8(0x0000, 0x00, set_led,    (void *) 0);

	new_netvar_output_8(0x0100, 0x50, set_dimmer, (void *) 0);
	new_netvar_output_8(0x0100, 0x51, set_dimmer, (void *) 1);
	new_netvar_output_8(0x0100, 0x52, set_dimmer, (void *) 2);
	new_netvar_output_8(0x0100, 0x53, set_dimmer, (void *) 3);

	new_lamp_controller(0x0100, 4);


	while (1) {
		can_handler();
		if (ticks_in_ms >= 10) {
			cli();
			ticks_in_ms -= 10;
			sei();

			netvar_handle_events();
		}
	}
}
