#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "can/can.h"
#include "can/spi.h"

#include "dc_com.h"
#include "touchscreen.h"
#include "calibrate_touch.h"
#include "gui.h"

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

int main (void)
{
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

	init_menu();	
	
	while (1) {

		handle_touchscreen();
		can_handler();
	}
	return 0;
}

