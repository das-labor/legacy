#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
 // those macros get defined via menuconfig, now

 // 8x 74HC595 Shift-Registers on SPI with
 // additional load-pin

 // 4x ROW-Drivers

 */

#define ROW1_ON()     (PORTC &= ~(_BV(1)))
#define ROW2_ON()     (PORTA &= ~(_BV(1)))
#define ROW3_ON()     (PORTA &= ~(_BV(2)))
#define ROW4_ON()     (PORTC &= ~(_BV(2)))

#define ROW1_OFF()     (PORTC |= (_BV(1)))
#define ROW2_OFF()     (PORTA |= (_BV(1)))
#define ROW3_OFF()     (PORTA |= (_BV(2)))
#define ROW4_OFF()     (PORTC |= (_BV(2)))

#define LOAD_OFF()        (PORTA &= ~(_BV(4)))
#define LOAD_ON()         (PORTA |= (_BV(4)))

#define MUX_ROWS        4

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	/* more ifdef magic :-( */
	#define OCR0 OCR0A
	#define SIG_OUTPUT_COMPARE0 SIG_OUTPUT_COMPARE0A
#endif

#ifdef HC165_JOYSTICK_SUPPORT
	extern volatile uint8_t hc165_joystick_val;
#endif

// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

unsigned char rowbuf[4][8];

static inline unsigned short bitgeschubse1(unsigned short x_in) {
	unsigned short result;
	//
	asm volatile (
			"bst %A1, 0" "\n\t"
			"bld %A0, 7" "\n\t"
			"bst %A1, 1" "\n\t"
			"bld %A0, 5" "\n\t"
			"bst %A1, 2" "\n\t"
			"bld %A0, 3" "\n\t"
			"bst %A1, 3" "\n\t"
			"bld %A0, 1" "\n\t"

			"bst %A1, 4" "\n\t"
			"bld %B0, 7" "\n\t"
			"bst %A1, 5" "\n\t"
			"bld %B0, 5" "\n\t"
			"bst %A1, 6" "\n\t"
			"bld %B0, 3" "\n\t"
			"bst %A1, 7" "\n\t"
			"bld %B0, 1" "\n\t"

			"bst %B1, 4" "\n\t"
			"bld %B0, 6" "\n\t"
			"bst %B1, 5" "\n\t"
			"bld %B0, 4" "\n\t"
			"bst %B1, 6" "\n\t"
			"bld %B0, 2" "\n\t"
			"bst %B1, 7" "\n\t"
			"bld %B0, 0" "\n\t"

			"bst %B1, 0" "\n\t"
			"bld %A0, 6" "\n\t"
			"bst %B1, 1" "\n\t"
			"bld %A0, 4" "\n\t"
			"bst %B1, 2" "\n\t"
			"bld %A0, 2" "\n\t"
			"bst %B1, 3" "\n\t"
			"bld %A0, 0"
			: "=&r" (result)
			: "r" (x_in)
	);

	return result;
}

static inline unsigned short bitgeschubse2(unsigned short x_in) {
	unsigned short result;
	//
	asm volatile (
			"bst %A1, 0" "\n\t"
			"bld %A0, 1" "\n\t"
			"bst %A1, 1" "\n\t"
			"bld %A0, 3" "\n\t"
			"bst %A1, 2" "\n\t"
			"bld %A0, 5" "\n\t"
			"bst %A1, 3" "\n\t"
			"bld %A0, 7" "\n\t"

			"bst %A1, 4" "\n\t"
			"bld %B0, 1" "\n\t"
			"bst %A1, 5" "\n\t"
			"bld %B0, 3" "\n\t"
			"bst %A1, 6" "\n\t"
			"bld %B0, 5" "\n\t"
			"bst %A1, 7" "\n\t"
			"bld %B0, 7" "\n\t"

			"bst %B1, 4" "\n\t"
			"bld %B0, 0" "\n\t"
			"bst %B1, 5" "\n\t"
			"bld %B0, 2" "\n\t"
			"bst %B1, 6" "\n\t"
			"bld %B0, 4" "\n\t"
			"bst %B1, 7" "\n\t"
			"bld %B0, 6" "\n\t"

			"bst %B1, 0" "\n\t"
			"bld %A0, 0" "\n\t"
			"bst %B1, 1" "\n\t"
			"bld %A0, 2" "\n\t"
			"bst %B1, 2" "\n\t"
			"bld %A0, 4" "\n\t"
			"bst %B1, 3" "\n\t"
			"bld %A0, 6"
			: "=&r" (result)
			: "r" (x_in)
	);

	return result;
}

// show a row
static void rowshow(unsigned char row, unsigned char plane) {
	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	static unsigned char const ocr_table[] = { 3, 4, 22 };
	unsigned char i;
	union u {
		unsigned short sValue;
		unsigned char cValue[2];
	} u1, u2;

	unsigned char *rowpointer = &rowbuf[row][0];

	OCR0 = ocr_table[plane];

#ifdef PROJECTION_MODE
	// upper half
	u1.cValue[0] = pixmap[plane][row][0];
	u1.cValue[1] = pixmap[plane][row + 4][0];
	u2.sValue = bitgeschubse2(u1.sValue);
	rowbuf[row][3] = u2.cValue[0];
	rowbuf[row][2] = u2.cValue[1];

	u1.cValue[0] = pixmap[plane][row][1];
	u1.cValue[1] = pixmap[plane][row + 4][1];
	u2.sValue = bitgeschubse2(u1.sValue);
	rowbuf[row][1] = u2.cValue[0];
	rowbuf[row][0] = u2.cValue[1];

	// lower half
	u1.cValue[0] = pixmap[plane][row + 8][0];
	u1.cValue[1] = pixmap[plane][row + 12][0];
	u2.sValue = bitgeschubse1(u1.sValue);
	rowbuf[row][4] = u2.cValue[0];
	rowbuf[row][5] = u2.cValue[1];

	u1.cValue[0] = pixmap[plane][row + 8][1];
	u1.cValue[1] = pixmap[plane][row + 12][1];
	u2.sValue = bitgeschubse1(u1.sValue);
	rowbuf[row][6] = u2.cValue[0];
	rowbuf[row][7] = u2.cValue[1];

#else
	// upper half
	u1.cValue[0] = pixmap[plane][row][0];
	u1.cValue[1] = pixmap[plane][row+4][0];
	u2.sValue = bitgeschubse1(u1.sValue);
	rowbuf[row][0] = u2.cValue[0];
	rowbuf[row][1] = u2.cValue[1];

	u1.cValue[0] = pixmap[plane][row][1];
	u1.cValue[1] = pixmap[plane][row+4][1];
	u2.sValue = bitgeschubse1(u1.sValue);
	rowbuf[row][2] = u2.cValue[0];
	rowbuf[row][3] = u2.cValue[1];

	// lower half
	u1.cValue[0] = pixmap[plane][row+8][0];
	u1.cValue[1] = pixmap[plane][row+12][0];
	u2.sValue = bitgeschubse2(u1.sValue);
	rowbuf[row][7] = u2.cValue[0];
	rowbuf[row][6] = u2.cValue[1];

	u1.cValue[0] = pixmap[plane][row+8][1];
	u1.cValue[1] = pixmap[plane][row+12][1];
	u2.sValue = bitgeschubse2(u1.sValue);
	rowbuf[row][5] = u2.cValue[0];
	rowbuf[row][4] = u2.cValue[1];
#endif

#ifdef HC165_JOYSTICK_SUPPORT
	HC165_JOYSTICK_PORT_LOAD &= ~(1 << HC165_JOYSTICK_BIT_LOAD);
	asm volatile("nop\r\t");
	HC165_JOYSTICK_PORT_LOAD |= (1 << HC165_JOYSTICK_BIT_LOAD);
#endif

	ROW1_OFF();
	ROW2_OFF();
	ROW3_OFF();
	ROW4_OFF();

	i = 8;
	while (i--) {
		SPDR = *(rowpointer++);
		while (!(SPSR & _BV(SPIF))) {
		}
#ifdef HC165_JOYSTICK_SUPPORT
		if (i == 7)
			hc165_joystick_val = SPDR;
#endif
	}

	/*	for(i=0;i<10;i++) {
	 nop();
	 }	*/

	LOAD_ON();

	LOAD_OFF();

	switch (row) {
	case 0:
		ROW4_ON();
		break;
	case 1:
		ROW3_ON();
		break;
	case 2:
		ROW2_ON();
		break;
	case 3:
		ROW1_ON();
		break;
	}
}

// depending on the plane this interrupt triggers at 50 kHz, 31.25 kHz or
// 12.5 kHz
ISR( TIMER0_COMP_vect) {
	static unsigned char plane = 0;
	static unsigned char row = 0;

	// reset watchdog
	wdt_reset();

	// increment both row and plane
	if (++plane == NUMPLANE) {
		plane = 0;
		if (++row == MUX_ROWS) {
			row = 0;
		}
	}

	// output current row according to current plane
	rowshow(row, plane);
}

void timer0_off() {
	cli();

	ROW1_OFF();
	ROW2_OFF();
	ROW3_OFF();
	ROW4_OFF();

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x00;
	TCCR0B = 0x00;
#else

	TCCR0 = 0x00;
#endif
	sei();
}

// initialize timer which triggers the interrupt
static void timer0_on() {
	/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	 CS02 CS01 CS00
	 0    0    0       stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/64
	 1    0    0       clk/256
	 1    0    1       clk/1024
	 */

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x02; // CTC Mode
	TCCR0B = 0x04; // clk/256
	TCNT0  =    0; // reset timer
	OCR0   =   20; // compare with this value
	TIMSK0 = 0x02; // compare match Interrupt on
#else
	TCCR0 = 0x0C; // CTC Mode, clk/256
	TCNT0 =    0; // reset timer
	OCR0  =   20; // compare with this value
	TIMSK = 0x02; // compare match Interrupt on
#endif
}

void borg_hw_init() {
	PORTA |= ((1 << 1) | (1 << 2)); // ROW2, ROW3, LOAD
	PORTA &= ~((1 << 4));
	PORTB |= ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)); // MOSI, MISO, SCK
	PORTC |= ((1 << 1) | (1 << 2)); // ROW1, ROW4
	PORTD |= ((1 << 0) | (1 << 1)); // RxD, TxD

	DDRA |= ((1 << 1) | (1 << 2) | (1 << 4)); // 1 = Output
	DDRB |= ((1 << 4) | (1 << 5) | (1 << 7));
	DDRB &= ~((1 << 6));
	DDRC |= ((1 << 1) | (1 << 2));
	DDRD |= ((1 << 1));
	DDRD &= ~((1 << 0));

	SPCR = 0b01010000;
	SPSR = 0x01;

	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
