#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "can/can.h"
#include "can/spi.h"
#include "can_handler.h"

#include "dc_com.h"
#include "touchscreen.h"
#include "calibrate_touch.h"
#include "gui.h"
#include "main_window.h"
#include "backlight.h"


volatile uint8_t ticks_in_ms;

ISR (TIMER0_COMP_vect){
	static uint8_t ticks;
	ticks_in_ms += 2;
	ticks++;
	if(ticks == 5){
		ticks = 0;
		backlight();		
	}
	
	
}

void init_timer(){
	//2ms Timer0
	TCCR0 =  (1<<WGM01) | 6; //CTC, clk/256
	OCR0  =  125;
	TIMSK |= (1<<OCIE0);
}

void init() {
	//LED Backlight on
	DDRF |= _BV(PF4);
	PORTF |= _BV(PF4);

	// SS als Ausgang sonst geht SPI nicht
	DDRB |= _BV(PB0);

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();
	read_can_addr();
}



int main(void) {
	init();
	init_dc_com();
	init_timer();
	init_backlight();
	
	sei();

	g_set_draw_color(1);
	g_clear_screen();
	

	if (read_calibration_data_from_eeprom())
		calibrate_touch();
	
	
	can_setled(0, 1);
	
	//draw_navigation_bar();
	//gui_test();
	//menu_test();

	init_main_window();
	
	DDRB |= (1<<PB5);

	uint8_t foo;
	while (1) {
		if(ticks_in_ms >= 10){
			cli();
			ticks_in_ms -= 10;
			sei();
						
			handle_touchscreen();
			can_handler();
		}
	}
	return 0;
}

