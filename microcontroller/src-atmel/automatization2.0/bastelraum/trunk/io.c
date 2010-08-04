#include <avr/io.h>
#include <util/delay.h>
#include "io.h"


void change_shift_reg(uint8_t sreg);

void init_io() {
	DDRA |= SREG_STROBE;
	PORTA |= SREG_STROBE;
	DDRC |= SREG_CK | SREG_DATA;
	SREG_PORT |= SREG_CK;
	
	change_shift_reg(0);
	DDRD |= _BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7); // Pins mit pwm als Ausgänge
	
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (non inverting mode).
	TCCR2B |= _BV(CS22);						// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);		// FastPWM bit 2, clk/64

	//PWM

	TCNT2 = 255;   // pwm timer clear
	OCR2A = 255;   // pwm timer compare target
	OCR2B = 255;   // pwm timer compare target
	
	TCNT1 = 255;   // pwm timer clear
	OCR1A = 255;   // pwm timer compare target
	OCR1B = 255;   // pwm timer compare target

	DDRA &= ~(_BV(PA4)); // Eingänge Türkontakt
	PORTA |= _BV(PA4);	// PULLUP

}

void change_shift_reg(uint8_t sreg) {
	for (int i = 0; i < 8; i++) {
		if (sreg & (1 << i))
			SREG_PORT |= SREG_DATA;
		else
			SREG_PORT &= ~SREG_DATA;
		SREG_PORT &= ~SREG_CK;
		_delay_us(10);
		SREG_PORT |= SREG_CK;
	}
	PORTA &= ~SREG_STROBE;
	_delay_us(10);
	PORTA |= SREG_STROBE;
}

void switch_handler() {
	
}


void pwm_set(volatile uint8_t *port, uint8_t value)
{
	(*port) = value;
}


void pwm_get(volatile uint8_t *port, uint8_t *result)
{
	(*result) = (*port);
}

