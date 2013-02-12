#include <avr/io.h>
#include <avr/interrupt.h>

#include "gui_lib/graphics.h"
#include "com.h"
#include "lcd_hardware.h"
#include "draw_pixmap.h"
#include "hypnotoad.h"



icon_t icon = {16, 16, 5,{0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,}};

int main() {
	init_com();
	init_lcd_hardware();

	g_clear_screen();

	draw_color = PIXEL_ON;
	
	/*
	uint16_t xc = 0, yc = 0;
	for (xc = 0; xc < 40; xc += 8)
		for (yc = 0; yc < 240; yc += 64)
			copy_logo(yc, xc);
	*/
	
	sei();
	
	lcd_on();

	while (1) {
		if (activate_hypnotoad_flag) {
			activate_hypnotoad_flag = 0;
			g_clear_screen();
			hypnotoad();
		}
		handle_com();
	}
}
