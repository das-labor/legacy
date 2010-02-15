#include <avr/io.h>
#include <avr/pgmspace.h>

#include "lcd_hardware.h"

uint8_t lsl_table[] =  {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};
//uint8_t nlsl_table[] = {(uint8_t)~(1<<0),(uint8_t)~(1<<1),(uint8_t)~(1<<2),(uint8_t)~(1<<3),(uint8_t)~(1<<4),(uint8_t)~(1<<5),(uint8_t)~(1<<6),(uint8_t)~(1<<7)};


/**
 * Plots a pixel at the specified coordinates.
 * @param x The x coordinante of the pixel.
 * @param y The y coordinante of the pixel.
 * @param state PIXEL_ON to set the pixel, otherwise pixel will be cleared.
 */
void lcd_graphics_plot_pixel(unsigned short x, unsigned short y, unsigned char state) {
	if (state) {
		pixmap[y * (X_SIZE / INTERFACE_BITS) + (x / INTERFACE_BITS)] |= lsl_table[(x % INTERFACE_BITS) + (INTERFACE_BITS == 4?4:0)];
	} else {
		pixmap[y * (X_SIZE / INTERFACE_BITS) + (x / INTERFACE_BITS)] &= ~lsl_table[(x % INTERFACE_BITS) + (INTERFACE_BITS == 4?4:0)];
	}
}


/**
 * Clears the LCD screen
 */
void g_clear_screen(void) {
	uint16_t i;
	/* Draw empty bytes to ocucpy the entire screen */
	volatile uint8_t * p = pixmap;

	for (i = 0; i < (((LCD_WIDTH * LCD_HEIGHT) / 8) * (8 / INTERFACE_BITS)); i++)
		*p++ = 0;
}


extern uint8_t laborlogo[];




void g_copy_logo(void * logo, uint8_t x_bytes, uint8_t y_size, uint16_t xc, uint16_t yc){
	uint8_t x,y;
	
	for(y=0;y<y_size;y++){
		for(x=0;x<x_bytes;x++){
#     if INTERFACE_BITS==4
			pixmap[(X_SIZE/INTERFACE_BITS)*(y+yc) + (x+xc)*(8/INTERFACE_BITS)] = pgm_read_byte(logo)<<4;
			pixmap[(X_SIZE/INTERFACE_BITS)*(y+yc) + (x+xc)*(8/INTERFACE_BITS) + 1] = pgm_read_byte(logo++);
#     endif
#     if INTERFACE_BITS==8
			pixmap[(X_SIZE/INTERFACE_BITS)*(y+yc) + (x+xc)*(8/INTERFACE_BITS)] = pgm_read_byte(logo++);
#     endif
		}
	}
}

