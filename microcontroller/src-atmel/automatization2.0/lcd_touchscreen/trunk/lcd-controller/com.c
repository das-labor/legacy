
#include <avr/io.h>
#include "graphics.h"
#include "../include/dc_commands.h"

#define PIN_DATA        PINB
#define DDR_DATA        DDRB
#define PORT_DATA       PORTB

#define PORT_HANDSHAKE  PORTD
#define DDR_HANDSHAKE   DDRD
#define PIN_HANDSHAKE   PIND

#define BIT_ATN PD0
#define BIT_ACK PD1

#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ACK))==0);}

#define WAIT_ATN_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ATN))==0);}

#define TEST_ATN()        (PIN_HANDSHAKE & _BV(BIT_ATN))

void init_com(){
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN)|_BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN)|_BV(BIT_ACK));
}


static uint8_t read_byte(){
	uint8_t b;

	WAIT_ATN_LOW();
	b = PIN_DATA;
	ACK_PULL();
	WAIT_ATN_HIGH();
	ACK_RELEASE();
	return b;	
}

static uint16_t read_uint16_t(){
	uint16_t i;
	i = read_byte();
	i |= read_byte()<<8;
	
	return i;
}


void handle_com(){
	uint8_t cmd;
	
	if(TEST_ATN() == 0){
	
		cmd = read_byte();
		switch(cmd){
			case DC_SET_COLOR:
				draw_color = read_byte();
				break;
			case DC_DRAW_CROSS:
				{
					uint16_t x = read_uint16_t();
					uint16_t y = read_uint16_t();
					g_draw_cross(x,y);
				}
				break;
			case DC_CLEAR_SCREEN:
				g_clear_screen();
				break;
		}
	
	}


}
