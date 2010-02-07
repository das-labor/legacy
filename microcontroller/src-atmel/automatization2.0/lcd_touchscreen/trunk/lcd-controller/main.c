
#include <avr/io.h>
#include <avr/interrupt.h>

#include "graphics.h"
#include "gui.h"
#include "com.h"
#include "lcd_hardware.h"
#include "draw_pixmap.h"

int main(){
	init_com();
	init_lcd_hardware();

	g_clear_screen();

	draw_color = PIXEL_ON;
	
	
	uint16_t xc=0,yc=0;
	for(xc=0;xc<40;xc+=8)
		for(yc=0;yc<240;yc+=64)
			copy_logo(yc,xc);


	sei();
	
	lcd_on();

	//while(1);

	
	while(1){
		handle_com();
	}

}
