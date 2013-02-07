#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "can/can.h"
#include "can/spi.h"
#include "can_handler.h"

#include "netvar/netvar.h"


volatile uint8_t ticks_in_ms;

ISR (TIMER0_COMP_vect) {
	static uint8_t ticks;
	ticks_in_ms += 2;
	ticks++;
	if (ticks == 5) {
		ticks = 0;
		//backlight();
	}
}

void init_timer() {
	//2ms Timer0
	TCCR0 =  (1<<WGM01) | 4; //CTC, clk/256
	OCR0  =  125;
	TIMSK |= (1<<OCIE0);
}

void init() {
	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();
	read_can_addr();
}

void nv_handler(netvar_desc *nd, void *ref) {
	//lap_button_t *s = (lap_button_t*) ref;
	
	if(nd->data[0]){
		PORTB |= _BV(PB0); // LED on
	
	}else{
		PORTB &= ~_BV(PB0); // LED off
	}
}


int main(void) {
	DDRB |= _BV(PB0); // LED out
	
	PORTB |= _BV(PB3); //set CS for RFM12 high in case it is present
	DDRB |= _BV(PB3);

	init();
	init_timer();

	sei();

	can_setled(0, 1);
	
	netvar_desc * nd = netvar_register(0x1111, 0, 1);
	netvar_add_handler(nd, nv_handler, 0);
	
	while (1) {
		if (ticks_in_ms >= 10) {
			cli();
			ticks_in_ms -= 10;
			sei();

			can_handler();
			netvar_handle_events();
			
		}
	}
	return 0;
}
