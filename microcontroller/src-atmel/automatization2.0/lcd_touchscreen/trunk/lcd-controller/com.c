#include <avr/io.h>

#include "graphics.h"
#include "draw_pixmap.h"
#include "../include/dc_commands.h"
#include "config.h"


#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ACK))==0);}

#define WAIT_ATN_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ATN))==0);}


void init_com() {
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN)|_BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN)|_BV(BIT_ACK));
}


static uint8_t read_byte() {
	uint8_t b;

	WAIT_ATN_LOW();
	b = PIN_DATA;
	ACK_PULL();
	WAIT_ATN_HIGH();
	ACK_RELEASE();
	return b;
}

static uint16_t read_uint16_t() {
	uint16_t i;
	i = read_byte();
	i |= read_byte() << 8;
	
	return i;
}

static void read_bytes(uint8_t size, void * dest) {
	while (size--) {
		*((uint8_t*)dest) = read_byte();
		dest = dest + 1;
	}
}

static void read_string(char * dest) {
	char b;
	do {
		b = read_byte();
		*dest++ = b;
	} while (b);
}



void handle_com() {
	uint8_t cmd;
	char buf[128];
	
	
	if (TEST_ATN() == 0) {
	
		cmd = read_byte();
		switch (cmd) {
			case DC_SET_COLOR:
				draw_color = read_byte();
				break;
			case DC_DRAW_CROSS:
				{
					uint16_t x = read_uint16_t();
					uint16_t y = read_uint16_t();
					g_draw_cross(x, y);
				}
				break;
			case DC_CLEAR_SCREEN:
				g_clear_screen();
				break;
			case DC_DRAW_RECTANGLE:
				{
					rectangle_t r;
					
					read_bytes(8, &r);
					
					g_draw_rectangle(&r);
					break;
				}
			case DC_FILL_RECTANGLE:
				{
					rectangle_t r;
					read_bytes(8, &r);
					g_fill_rectangle(&r);
					break;
				}
			case DC_DRAW_STRING_IN_RECT:	
				{
					rectangle_t r;
					read_bytes(8, &r);
					read_string(buf);
					g_draw_string_in_rect(&r, buf);
					break;
				}
			case DC_DRAW_STRING_IN_RECT_VERT:	
				{
					rectangle_t r;
					read_bytes(8, &r);
					read_string(buf);
					g_draw_string_in_rect_vert(&r, buf);
					break;
				}		
		}	
	}
}
