#include <avr/io.h>
#include <avr/interrupt.h>

#pragma once

#if \
	defined(__AVR_ATtiny13__) || \
	defined(__AVR_ATtiny13A__)
	/*
		tiny13 support *NOT TESTED YET*
	*/
	#error wtf
	#define TIMER_INIT \
		TCCR0A = _BV(WGM1); /* clear on compare match */ \
		OCRA = 125; /* 4.8M / 38400 -> 125 cycles/bit */ \
		TCCR0B = (_BV(CS00)); /* clk/1;  */ \
		TIMSK0 = _BV(OCIE0A) /* OCR0A int */

	#define BIT_ISR ISR(TIMER0_COMPA_vect)

	#define PIN_LED0  _BV(PB0)
	#define DDR_LED0  DDRB
	#define PORT_LED0 PORTB

	#define PRESCALER_INIT /* system clock prescaler */ \
		CLKPR = _BV(CLKPCE); \
		CLKPR = _BV(CLKPS0) /* /2 */

#elif \
	defined(__AVR_ATtiny25__) || \
	defined(__AVR_ATtiny45__) || \
	defined(__AVR_ATtiny85__)

	#define TIMER_INIT \
		TCCR0A = _BV(WGM01); /* clear on compare match */ \
		OCR0A = 208; /* 4800, 1 MHz -> 208.333 cycles/bit -> 0.48% timing error */ \
		TCCR0B = (_BV(CS00)); /* clk/1;  */ \
		TIMSK = _BV(OCIE0A) /* OCR0A int */
		
	#define BIT_ISR ISR(TIMER0_COMPA_vect)
	
	#define PIN_LED0  _BV(PB0)
	#define DDR_LED0  DDRB
	#define PORT_LED0 PORTB
	
	#define PRESCALER_INIT /* system clock prescaler */ \
		CLKPR = _BV(CLKPCE); \
		CLKPR = (_BV(CLKPS1) | _BV(CLKPS0)) /* / 16 */
#else
	#error "can't find valid config for your mcu. please edit config.h"
#endif

/* general config */
#if 0
#define OUT0_0 PORT_LED0 &= ~(PIN_LED0)
#define OUT0_1 PORT_LED0 |= (PIN_LED0)
#else
#define OUT0_1 PORT_LED0 &= ~(PIN_LED0)
#define OUT0_0 PORT_LED0 |= (PIN_LED0)
#endif
#define OUT0_INIT DDR_LED0 |= PIN_LED0

/* additional stop bits at the end of each transmission */
#define NUM_STOPBITS 8
