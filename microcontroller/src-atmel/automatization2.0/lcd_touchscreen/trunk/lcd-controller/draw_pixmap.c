#include <avr/io.h>
#include <avr/pgmspace.h>

//define BIG DISPLAY in here
#include "../include/personal_config.h"
#include "config.h"

#include "lcd_hardware.h"
#include "../include/icon.h"
#include "graphics.h"

#ifdef DATABUS_REVERSED
	uint8_t lsl_table[] =  {(1<<7), (1<<6), (1<<5), (1<<4), (1<<3), (1<<2), (1<<1), (1<<0)};
#else
	uint8_t lsl_table[] =  {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};
#endif
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
#     	ifdef DATABUS_REVERSED
				uint8_t d;
				d = pgm_read_byte(logo++);
				d = (d<<4) | (d>>4);
				d = ((d & 0xCC) >>2) | ((d & 0x33)<<2);
				d = ((d & 0xAA) >>1) | ((d & 0x55)<<1);
				
				pixmap[(X_SIZE/INTERFACE_BITS)*(y+yc) + (x+xc)*(8/INTERFACE_BITS)] = d; 

#				else
				pixmap[(X_SIZE/INTERFACE_BITS)*(y+yc) + (x+xc)*(8/INTERFACE_BITS)] = pgm_read_byte(logo++);
#				endif
#     endif
		}
	}
}

extern uint8_t draw_color;
void g_draw_icon(uint16_t x, uint16_t y, icon_t * i){
	uint16_t w;
	uint16_t h;
	
	uint16_t byte_cnt = 0;
	uint8_t msk;
	uint8_t d = 0;
	
	h = i->height;
	while(h--){
		uint16_t xi = x;
		msk = 0x00;
		w = i->width;
		while(w--){
			if(msk == 0x00){
				d = i->data[byte_cnt++];
				msk = 0x01;
			}
			if(d & msk){
				lcd_graphics_plot_pixel(xi,y,draw_color);
			}
			xi++;
			msk <<= 1;
		}
		y++;
	}
}

/**
 * Draws a horizontal line, left to right, at the specified coordinates and of
 * the specified length.
 * @param x The x coordinante of the line's origin.
 * @param y The y coordinante of the line's origin.
 * @param length The length of the line, in pixels.
 */
void g_draw_horizontal_line(unsigned short x, unsigned short y, unsigned short length) {
	uint16_t  addr = y * (X_SIZE / INTERFACE_BITS) + (x / INTERFACE_BITS);
	uint16_t eaddr = y * (X_SIZE / INTERFACE_BITS) + ((x + length-1) / INTERFACE_BITS);
	
	#ifdef DATABUS_REVERSED
		uint8_t emsk = 0xff << (7-((x+length-1)%INTERFACE_BITS));
		uint8_t msk = 0xff >> (x%INTERFACE_BITS);
	#else
		uint8_t emsk = 0xff >> (((INTERFACE_BITS==4)?3:7)-((x+length-1)%INTERFACE_BITS));
		uint8_t msk = ((INTERFACE_BITS==4)?0xf0:0xff) << (x%INTERFACE_BITS );	
	#endif
		
	if(draw_color){
		for( ;addr <= eaddr ; addr++){
			if(addr == eaddr) msk &= emsk;
			pixmap[addr] |= msk;
			msk = 0xff;
		}
	}else{
		msk = ~msk;
		for( ;addr <=eaddr ; addr++){
			if(addr == eaddr) msk |= ~emsk;
			pixmap[addr] &= msk;
			msk = 0;
		}	
	}
}

void g_fill_rectangle(rectangle_t *r) {
	uint16_t x = r->x;
	uint16_t y = r->y;
	uint16_t w = r->w - 1;
	uint16_t h = r->h;
	
	uint16_t saddr = y * (X_SIZE / INTERFACE_BITS) + (x / INTERFACE_BITS);
	uint16_t eaddr = y * (X_SIZE / INTERFACE_BITS) + ((x + w-1) / INTERFACE_BITS);
	
	#ifdef DATABUS_REVERSED
		uint8_t emsk = 0xff << (7-((x+w-1)%INTERFACE_BITS));
		uint8_t smsk = 0xff >> (x%INTERFACE_BITS);
	#else
		uint8_t emsk = 0xff >> (((INTERFACE_BITS==4)?3:7)-((x+w-1)%INTERFACE_BITS));
		uint8_t smsk = ((INTERFACE_BITS==4)?0xf0:0xff) << (x%INTERFACE_BITS );	
	#endif
	
	if(draw_color){
		while (h--) {
			uint8_t msk;
			uint16_t addr;
			for(msk = smsk, addr=saddr; addr <= eaddr ; addr++){
				if(addr == eaddr) msk &= emsk;
				pixmap[addr] |= msk;
				msk = 0xff;
			}
			saddr += (X_SIZE / INTERFACE_BITS);
			eaddr += (X_SIZE / INTERFACE_BITS);
		}
	}else{
		smsk = ~smsk;
		emsk = ~emsk;
		while (h--) {
			uint8_t msk;
			uint16_t addr;
			for(msk = smsk, addr=saddr; addr <=eaddr ; addr++){
				if(addr == eaddr) msk |= emsk;
				pixmap[addr] &= msk;
				msk = 0;
			}		
			saddr += (X_SIZE / INTERFACE_BITS);
			eaddr += (X_SIZE / INTERFACE_BITS);
		}
	}
}
