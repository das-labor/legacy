#include <avr/io.h>

#include <avr/interrupt.h>
#include "lcd_hardware.h"


#define PORT_CONTROL PORTD
#define DDR_CONTROL DDRD


#define BIT_DISPLAY_ON PD2
#define BIT_LP PD3
#define BIT_FLM PD4
#define BIT_XCK_ENABLE PD5

#define DDR_M DDRE
#define PORT_M PORTE
#define BIT_M PE2

#define DDR_POWER DDRE
#define PORT_POWER PORTE
#define BIT_POWER PE0


#define FRAME_RATE 75 //in Hz


void init_lcd_hardware() {
	MCUCR = _BV(SRE) | _BV(ISC01); //xram on	

	DDR_CONTROL |= _BV(BIT_DISPLAY_ON) |_BV(BIT_LP) |_BV(BIT_FLM) |_BV(BIT_XCK_ENABLE) ;
	
	//power off
	PORT_POWER |= _BV(BIT_POWER);
	DDR_POWER |= _BV(BIT_POWER);

	DDR_M |= _BV(BIT_M);


	TCCR0 = _BV(WGM01) | 2; //clk/8, ctc								
	OCR0 = (F_CPU / 8 / (FRAME_RATE * Y_SIZE)) - 1;

	TIMSK |= _BV(OCIE0); //overflow interrupt on
}

void lcd_on() {
	PORT_CONTROL |= _BV(BIT_DISPLAY_ON);
	PORT_POWER &= ~_BV(BIT_POWER);
}


uint8_t display_line;

ISR(TIMER0_COMP_vect){
	volatile uint8_t* mempt;
	
	uint8_t cd;

	mempt = &pixmap[display_line * (X_SIZE/INTERFACE_BITS) ];

	PORT_M ^= _BV(BIT_M);

	if(display_line==1)	PORT_CONTROL |=  _BV(BIT_FLM);

	PORT_CONTROL |=  _BV(BIT_LP);
	PORT_CONTROL &= ~_BV(BIT_LP);
	PORT_CONTROL &= ~_BV(BIT_FLM);

	for (cd = 0; cd < (X_SIZE / (INTERFACE_BITS * 8)); cd++) {
		*mempt++;
		*mempt++;
		*mempt++;
		*mempt++;
		*mempt++;
		*mempt++;
		*mempt++;
		*mempt++;
	}

	display_line++;
	if(display_line == Y_SIZE) display_line=0;	
}
