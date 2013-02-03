#include <avr/io.h>
#include <util/delay.h>

#include "dc_com.h"
#include "../include/dc_commands.h"
#include "config.h"



#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while (PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while ((PIN_HANDSHAKE & _BV(BIT_ACK)) == 0);}

#define WAIT_ATN_LOW()     {while (PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while ((PIN_HANDSHAKE & _BV(BIT_ATN)) == 0);}

#define ATN_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ATN))==0)
#define ACK_IS_LOW()   ((PIN_HANDSHAKE & _BV(BIT_ACK))==0)


void dc_byte_put(uint8_t b) {
	PORT_DC_DATA = b;

	WAIT_ATN_HIGH();
	ATN_PULL();
	WAIT_ACK_LOW();
	ATN_RELEASE();
	WAIT_ACK_HIGH();
	WAIT_ATN_HIGH();
}

uint8_t dc_byte_get(){
	uint8_t b;
	WAIT_ACK_HIGH();
	DDR_DC_DATA = 0;
	PORT_DC_DATA = 0xff;
	ACK_PULL();
	WAIT_ATN_LOW();
	b = PIN_DC_DATA;
	ACK_RELEASE();
	WAIT_ATN_HIGH();
	DDR_DC_DATA = 0xff;
	return b;
}

int16_t dc_int16_get(){
	uint16_t i;
	i =  dc_byte_get();
	i |= (uint16_t) dc_byte_get() << 8;
	return ((int16_t) i);
}

void init_dc_com() {
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN) | _BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN) | _BV(BIT_ACK));
	DDR_DC_DATA = 0xff;
	int x;
	for(x = 0; x < 32; x++) {
		dc_byte_put(DC_RESET);
	}
}

void transmit_to_dc(uint8_t command, uint16_t size, void *data) {
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

void transmit_to_dc_raw(void *data, uint16_t size) {
	uint16_t i;
	for (i = 0; i < size; i++) {
		dc_byte_put(((uint8_t*)data)[i]);
	}
}

void transmit_to_dc_string(const char *data) {
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


void g_draw_rectangle(rectangle_t *r) {
	transmit_to_dc(DC_DRAW_RECTANGLE, 8, r);
}


void g_fill_rectangle(rectangle_t *r) {
	transmit_to_dc(DC_FILL_RECTANGLE, 8, r);
}

void g_draw_string(uint16_t x, uint16_t y, const char *str) {
	dc_byte_put(DC_DRAW_STRING);
	transmit_to_dc_raw(&x, 2);
	transmit_to_dc_raw(&y, 2);
	transmit_to_dc_string(str);
}

void g_draw_string_in_rect(rectangle_t *r, const char *str) {
	dc_byte_put(DC_DRAW_STRING_IN_RECT);

	transmit_to_dc_raw(r, 8);
	transmit_to_dc_string(str);
}


void g_draw_string_in_rect_vert(rectangle_t *r, const char *str) {
	dc_byte_put(DC_DRAW_STRING_IN_RECT_VERT);

	transmit_to_dc_raw(r, 8);
	transmit_to_dc_string(str);
}

void g_draw_icon(uint16_t x, uint16_t y, icon_t * i) {
	dc_byte_put(DC_DRAW_ICON);
	transmit_to_dc_raw(&x, 2);
	transmit_to_dc_raw(&y, 2);
	transmit_to_dc_data(i, i->size);
}

void g_draw_pixel(uint16_t x, uint16_t y) {
	dc_byte_put(DC_DRAW_PIXEL);
	transmit_to_dc_raw(&x, 2);
	transmit_to_dc_raw(&y, 2);
}

int g_get_last_text_height() {
	dc_byte_put(DC_GET_LAST_TEXT_HEIGHT);
	return dc_int16_get();
	return 10;
}

int16_t get_string_width(const char *str) {
	dc_byte_put(DC_GET_STRING_WIDTH);
	transmit_to_dc_string(str);
	return dc_int16_get();
}
