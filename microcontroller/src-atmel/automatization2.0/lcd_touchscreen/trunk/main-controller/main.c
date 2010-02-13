#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "dc_com.h"
#include "touchscreen.h"
#include "calibrate_touch.h"
#include "gui.h"

void init() {
	//Backlight on
	DDRF |= _BV(PF4);
	PORTF |= _BV(PF4);
}

int main (void)
{

	init();
	init_dc_com();

		
	g_set_draw_color(1);


/*
	rectangle_t r ={10,10,10,10};	
	g_draw_rectangle(&r);
	while(1);
*/

	g_clear_screen();
	
	
	
	calibrate_touch();
	
	
	menu_test();
	
	while (1) {

		handle_touchscreen();

	}
	return 0;
}

