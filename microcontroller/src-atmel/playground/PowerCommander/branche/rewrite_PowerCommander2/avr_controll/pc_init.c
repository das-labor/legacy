/* -*- Mode: C; tab-width: 2 -*- */
/*
 * PowerCommander - Ausführungs Kontroller
 */

#include <avr/io.h>
#include "pc_init.h"

void init_relais()
{
	DDRC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);      // relais ausgänge 1-4 küche licht, beamer, vortrag licht, lounge licht
	PORTC &= ~(_BV(PC3) | _BV(PC2));												// aus damit

	DDRB |= _BV(PB7) | _BV(PB5) | _BV(PB4) | _BV(PB0);      // licht 4, relais ausgänge 5-7, tischsteckdose lounge, licht toilette
	PORTB &= ~(_BV(PB4));             											// aus damit
	PORTB |= _BV(PB5) | _BV(PB0) | _BV(PB7);

	DDRD |= _BV(PD7) | _BV(PD4) | _BV(PD2) | _BV(PD1) | _BV(PD0);   	// relais ausgang 8, hauptschütz, licht 3, licht 2, licht 1
	PORTD &= ~(_BV(PD7));																							// aus
	PORTD |= _BV(PD2) | _BV(PD1) | _BV(PD0);


	DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);                 // Pins mit pwm als ausgänge
	DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);                 // Pins mit pwm als ausgänge
}

void init_modi()
{
	/* 
		 Disable Analog Comparator (power save)
	*/
	ACSR = _BV(ACD);

	/* 
		 Disable Analog to Digital converter (power save)
	*/
	ADCSRA = 0;
}

void init_timer()
{
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (inverting mode).
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);																								// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0); // FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS11);                                 // FastPWM bit 2, clk/64

	TCCR0A |= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0); // FastPWM, Set OC0X on Compare Match, clear OC0x at BOTTOM, (inverting mode).
	TCCR0B |= _BV(CS01) | _BV(CS00);                                                           // clk/64

	TCNT2 = 0;   // pwm timer clear
	OCR2A = 0;   // pwm timer compare target
	OCR2B = 0;   // pwm timer compare target
	
	TCNT1 = 0;   // pwm timer clear
	OCR1A = 0;   // pwm timer compare target
	OCR1B = 0;   // pwm timer compare target

	TCNT0 = 0;   // pwm timer clear
	OCR0A = 0;   // pwm timer compare target
	OCR0B = 0;   // pwm timer compare target

}
