#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "can/can.h"
#include "can/spi.h"
#include "can_handler.h"

#include "dc_com.h"
#include "touchscreen.h"
#include "calibrate_touch.h"
#include "gui.h"
#include "main_window.h"

void init() {
	//Backlight on
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

	g_set_draw_color(1);


/*	rectangle_t r = {10,10,10,10};	
	g_draw_rectangle(&r);
	while(1);
*/

	g_clear_screen();
	

	if (read_calibration_data_from_eeprom())
		calibrate_touch();
	
	
	can_setled(0, 1);
	
	//draw_navigation_bar();
	//gui_test();
	//menu_test();

	init_main_window();
	
	
	TCCR1A = (1<<COM1A1) | (1<<WGM10);
	TCCR1B = (1<<WGM12) | 1;
	
	DDRB |= (1<<PB5);
	
	int16_t x, y;
	
	
	
	while (1) {

		for(x=255;x>100;x--){
			OCR1A = x;
//			_delay_ms(10);
			for(y=0;y<100;y++){
				handle_touchscreen();
				can_handler();
			}

		}

	}
	return 0;
}

