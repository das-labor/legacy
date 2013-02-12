#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "can/can.h"
#include "can/spi.h"
#include "netvar/can_handler.h"

#include "gui_lib/graphics.h"
#include "touchscreen.h"
#include "calibrate_touch.h"
#include "gui_lib/gui.h"
#include "main_window.h"
#include "backlight.h"
#include "adc.h"
#include "netvar/netvar.h"
#include "dc_com.h"


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
	TCCR0 =  (1<<WGM01) | 6; //CTC, clk/256
	OCR0  =  125;
	TIMSK |= (1<<OCIE0);
}

void init() {
	//LED Backlight on
	//DDRF |= _BV(PF4);
	//PORTF |= _BV(PF4);

	// SS als Ausgang sonst geht SPI nicht
	DDRB |= _BV(PB0);

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();
	read_can_addr();
	
	uart_init();
}


/*
void test() {
	pixel p;
	static uint16_t x;
	uint16_t y;

	
	p = read_touch_raw();
	y = p.y;
	y -= 512;
	if(y>239) y = 239;
	g_draw_pixel(x, y);
	x++;
	if(x == 320) x = 0;
	

	//p = read_touch_screen_coordinates();

	//g_draw_cross(p.x, p.y);
}*/


extern icon_t room_icon;

void print(char * s) {
	static uint8_t line = 0;
		g_set_draw_color(1);

	g_draw_string(120, line*8, s);
	line++;	
}

int main(void) {
	init();
	init_dc_com();
	init_timer();
	init_backlight();

	init_adc();

	sei();

	g_set_draw_color(1);
	g_clear_screen();


	if (read_calibration_data_from_eeprom())
		calibrate_touch();


	can_setled(0, 1);

	init_main_window();

	while (1) {
		if (ticks_in_ms >= 10) {
			cli();
			ticks_in_ms -= 10;
			sei();

			handle_touchscreen();
			handle_status();
			can_handler();
			netvar_handle_events();
		}
	}
	return 0;
}

