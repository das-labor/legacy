#include <avr/io.h>
#include <util/delay.h>

#include "gui_lib/graphics.h"
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

#define ATN_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ATN))==0)
#define ACK_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ACK))==0)

void init_com() {
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN)|_BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN)|_BV(BIT_ACK));
}


#define TX_DATA_SIZE 2

uint8_t tx_data_idx;
uint8_t tx_data[TX_DATA_SIZE];

static uint8_t read_byte() {
	uint8_t b;

	while(1){
		if(ATN_IS_LOW()){ //read byte from master
			b = PIN_DATA;
			ACK_PULL();
			WAIT_ATN_HIGH();
			ACK_RELEASE();
			_delay_us(1); //wait for ack line to settle
			return b;
		}
		if(ACK_IS_LOW()){//master requests byte
			DDR_DATA = 0xff;
			PORT_DATA = tx_data[tx_data_idx];
			tx_data_idx  = (tx_data_idx + 1) % TX_DATA_SIZE;
			ATN_PULL();
			WAIT_ACK_HIGH();
			DDR_DATA = 0;
			PORT_DATA = 0xff;
			ATN_RELEASE();
			_delay_us(1); //wait for atn line to settle
		}
	}
}

static uint16_t read_uint16_t() {
	uint16_t i;
	i = read_byte();
	i |= read_byte() << 8;
	
	return i;
}

static void read_bytes(uint16_t size, void * dest) {
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

static void return_int16(int16_t i){
	tx_data[0] = i & 0xff;
	tx_data[1] = ((uint16_t)i) >> 8;
	tx_data_idx = 0;
}


void handle_com() {
	uint8_t cmd;
	char buf[128];
	
	
	if(ACK_IS_LOW() || ATN_IS_LOW()){
	
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
			case DC_DRAW_STRING:	
				{
					uint16_t x,y;
					read_bytes(2, &x);
					read_bytes(2, &y);
					read_string(buf);
					g_draw_string(x, y, buf);
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
			case DC_DRAW_ICON:	
				{
					uint16_t x,y,size;
					read_bytes(2, &x);
					read_bytes(2, &y);
					read_bytes(2, &size);
					read_bytes(size, buf);
					g_draw_icon(x, y, (icon_t*)buf);
					break;	
				}
				
			case DC_DRAW_PIXEL:
				{
					uint16_t x,y;
					read_bytes(2, &x);
					read_bytes(2, &y);
					g_draw_pixel(x,y);	
					break;
				}
			case DC_GET_STRING_WIDTH:
				{
					read_string(buf);
					return_int16(g_get_string_width(buf));
					break;				
				}
			case DC_GET_LAST_TEXT_HEIGHT:
				{
					return_int16(g_get_last_text_height());
					break;
				}
			
		}
	}
}
