#include <avr/io.h>

#include "dc_com.h"
#include "../include/dc_commands.h"


#define PORT_HANDSHAKE  PORTC
#define DDR_HANDSHAKE   DDRC
#define PIN_HANDSHAKE   PINC


#define BIT_ATN PC7
#define BIT_ACK PC6

#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ACK))==0);}

#define WAIT_ATN_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ATN))==0);}


void init_dc_com() {
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN)|_BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN)|_BV(BIT_ACK));
}


void dc_byte_put(uint8_t b) {
	DDRA = 0xff;
	PORTA = b;

	WAIT_ATN_HIGH();
	ATN_PULL();
	WAIT_ACK_LOW();
	ATN_RELEASE();
	WAIT_ACK_HIGH();
}



void transmit_to_dc(uint8_t command, uint16_t size, void * data) {
	uint16_t i;

	dc_byte_put(command);

	for (i = 0; i < size; i++) {
		dc_byte_put(((uint8_t*)data)[i]);
	}
}


void transmit_to_dc_data(void * data, uint16_t size) {
	uint16_t i;

	dc_byte_put(size);
	dc_byte_put(size >> 8);

	for (i = 0; i < size; i++) {
		dc_byte_put(((uint8_t*)data)[i]);
	}
}



void transmit_to_dc_raw(void * data, uint16_t size) {
	uint16_t i;
	for (i = 0; i < size; i++) {
		dc_byte_put(((uint8_t*)data)[i]);
	}
}


void transmit_to_dc_string(const char * data) {
	char c;
	do {
		c = *data++;
		dc_byte_put(c);
	} while (c != 0);
}


void g_draw_cross(uint16_t x, uint16_t y) {
	uint16_t data[2];
	data[0] = x;
	data[1] = y;

	transmit_to_dc(DC_DRAW_CROSS, 4, data);
}


void g_set_draw_color(uint8_t color) {
	uint8_t data[1];
	data[0] = color;

	transmit_to_dc(DC_SET_COLOR, 1, data);
}


void g_clear_screen() {
	transmit_to_dc(DC_CLEAR_SCREEN, 0, 0);
}


void g_draw_rectangle(rectangle_t * r) {
	transmit_to_dc(DC_DRAW_RECTANGLE, 8, r);
}


void g_fill_rectangle(rectangle_t * r) {
	transmit_to_dc(DC_FILL_RECTANGLE, 8, r);
}


void g_draw_string_in_rect(rectangle_t * r, const char *str) {
	dc_byte_put(DC_DRAW_STRING_IN_RECT);

	transmit_to_dc_raw(r, 8);
	transmit_to_dc_string(str);
}


void g_draw_string_in_rect_vert(rectangle_t * r, const char *str) {
	dc_byte_put(DC_DRAW_STRING_IN_RECT_VERT);

	transmit_to_dc_raw(r, 8);
	transmit_to_dc_string(str);
}

void g_draw_icon(uint16_t x, uint16_t y, icon_t * i) {	
	dc_byte_put(DC_DRAW_ICON);
	transmit_to_dc_raw(&x      , 2);
	transmit_to_dc_raw(&y      , 2);
	transmit_to_dc_data(i      , i->size);
}
