/*
 * PowerCommander - Ausführungs Controller
 */

#include <avr/io.h>
#include <avr/wdt.h>

#include "pc_init.h"

void init_io_ports()
{
	DDRC |= _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);	// relais ausgänge 1-4 küche licht, beamer, vortrag licht, lounge licht
	PORTC &= ~(_BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0));	// aus damit

	DDRB |= _BV(PB5) | _BV(PB4) | _BV(PB0);	// , relais ausgänge 5-7, tischsteckdose lounge, licht toilette
	PORTB &= ~(_BV(PB5)| _BV(PB4) | _BV(PB0));	// aus damit

	DDRD |= _BV(PD7) | _BV(PD4) | _BV(PD2) | _BV(PD1) | _BV(PD0);		// relais ausgang 8, hauptschütz, licht 3, licht 2, licht 1, licht 4
	PORTD &= ~(_BV(PD7) | _BV(PD4) | _BV(PD2) | _BV(PD1) | _BV(PD0));	// aus


	DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);                 // Pins mit pwm als ausgänge
	DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);                 // Pins mit pwm als ausgänge
}

void init_modi()
{
	/*
		 Disable Analog Comparator (power save)
	*/
	ACSR = _BV(ACD);
	wdt_enable(WDTO_250MS); // 2000 ms
}

void init_timer()
{
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (non inverting mode).
	TCCR2B |= _BV(CS22);										// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);			// FastPWM bit 2, clk/64

	TCCR0A |= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1);	// FastPWM, Set OC0X on Compare Match, clear OC0x at BOTTOM, (non inverting mode).
	TCCR0B |= _BV(CS01) | _BV(CS00);								// clk/64

	TCNT2 = 255;   // pwm timer clear
	//OCR2A = 255;   // pwm timer compare target
	//OCR2B = 255;   // pwm timer compare target

	TCNT1 = 255;   // pwm timer clear
	//OCR1A = 255;   // pwm timer compare target
	//OCR1B = 255;   // pwm timer compare target

	TCNT0 = 255;   // pwm timer clear
	//OCR0A = 255;   // pwm timer compare target
	//OCR0B = 255;   // pwm timer compare target
}

