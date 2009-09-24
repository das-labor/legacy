/* -*- Mode: C; tab-width: 2 -*- */
/*
 * PowerCommander - Ausführungs Kontroller
 */

#include <avr/io.h>


inline void init_relais()
{
	DDRC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);      // relais ausgänge 1-4 küche licht, beamer, vortrag licht, lounge licht
	PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0));  // aus damit

	DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB0);                 // relais ausgänge 5-7 tischsteckdose lounge, geschaltete serverschrank steckdose,
	PORTB &= ~(_BV(PB4) | _BV(PB0));             						// aus damit
	PORTB |= _BV(PB5);	

	DDRD |= _BV(PD7) | _BV(PD1) | _BV(PD0);                 // relais ausgang 8, 230V 1-2
	PORTD &= ~(_BV(PD7) | _BV(PD1) | _BV(PD0));             // aus

}

inline void init_modi()
{
	/* 
		 Disable Analog Comparator (power save)
	*/
	ACSR = _BV(ACD);

	/* 
		 Disable Analog to Digital converter (power save)
	*/
	ADCSRA = 0;

	DDRD  &= ~(_BV(PD4) | _BV(PD2));	/* Pin PD2 und PD4 als Eingange für Taster blau und gelb */
	PORTD |= _BV(PD4) | _BV(PD2);			/* Pull Ups setzen */

	DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);                 // Pins mit pwm als ausgänge
	DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);                 // Pins mit pwm als ausgänge

	EICRA |= _BV(ISC00);	// Trigger Interrupt on any logical change on pin pd2
	EIMSK |= _BV(INT0);								// Enable External Interrupt Request 0
	
	PCICR  |= _BV(PCIE2);							// Enable Pin Change Interrupt 2
	PCMSK2 |= _BV(PCINT20);						// Enable PCI18 (pin PD4) as Pin Change Interrupt
	
	TIMSK0 |= _BV(TOIE0);							// Enable Timer0 Overflow Interrupt

}

inline void init_timer()
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

