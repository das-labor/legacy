#define MAIN_C

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "uart.h"
#include "pixel.h"
#include "anim3d.h"

/*

PD3 z=0
PD4 z=1
PB0 z=2

PD0  x=0 y=0
PD1  x=1 y=0
PD2  x=2 y=0

PC3  x=0 y=1
PC4  x=1 y=1
PC5  x=2 y=1

PC0  x=0 y=2
PC1  x=1 y=2
PC2  x=2 y=2

 */

#define SET3BIT(var, PORTB_0, B_0, PORTB_1, B_1, PORTB_2, B_2)  \
		switch (var) { \
			case 0: \
				PORTB_0 |=  (1 << B_0); \
				PORTB_1 &= ~(1 << B_1); \
				PORTB_2 &= ~(1 << B_2); \
				break; \
			case 1: \
				PORTB_0 &= ~(1 << B_0); \
				PORTB_1 |=  (1 << B_1); \
				PORTB_2 &= ~(1 << B_2); \
				break; \
			case 2: \
				PORTB_0 &= ~(1 << B_0); \
				PORTB_1 &= ~(1 << B_1); \
				PORTB_2 |=  (1 << B_2); \
				break;	\
		}

unsigned char pixmap[MAXBRITH][MAX_Z][MAX_Y];

static inline void planeShow(unsigned char plane, unsigned char brith) {	
	if (plane == 2) {
		switch (brith) {
			case 0:
				OCR2 = 8;
				break;
			case 1:
				OCR2 = 8;
				break;
			case 2:
				OCR2 = 20;
		}
	}
	SET3BIT(plane, PORTD, 3, PORTD, 4, PORTB, 0);
	SET3BIT(pixmap[brith][plane][0], PORTD, 0, PORTD, 1, PORTD, 2);
	SET3BIT(pixmap[brith][plane][1], PORTC, 3, PORTC, 4, PORTC, 5);
	SET3BIT(pixmap[brith][plane][2], PORTD, 0, PORTD, 1, PORTD, 2);
}

SIGNAL(SIG_OVERFLOW2)
{
	static unsigned char plane = 3;
	static unsigned char brith = 0;	

	planeShow(plane, brith);
	
	if (plane-- == 0) {
		plane = MAX_Z;
		if (++brith == MAXBRITH) 
			brith = 0;
	}
}

void timer0_on() {
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR2 = 0x0B;	// CTC Mode, clk/8
	TCNT2 = 0;		// reset timer
	OCR2  = 0x10;	// Compare with this value
	TIMSK = (1 << OCIE2);	// Compare match Interrupt on
}

void timer0_off() {
	TIMSK = 0;
}

void init_Ports() {
	DDRB  = 0xFF;
	DDRC  = 0xFF;
	DDRD  = 0XFF;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
}

int main(void) {
	//display_init();
	//display_cursor(0, 0);
	//display_printChar('#');
	
	init_Ports();
	timer0_on();
	sei();
	uart_init();
  	uart_putstr("starting \n");
	while (1) {
		snake();
		schachBlinking2();
		cool(90, 1);
		schachBlinking();
		test3();
		cool(60, 3);
		test1(); 
		cool(30, 9);
		test3();		
	}
}
