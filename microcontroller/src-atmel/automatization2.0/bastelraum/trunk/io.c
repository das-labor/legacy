#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
#include <avr/interrupt.h>

#include "motion.h"
#include "Bastelcmd.h"

uint8_t sreg;

void change_shift_reg(uint8_t sreg);

uint8_t stat_licht = 0;

volatile uint16_t tickscounter = 0;
ISR(TIMER2_OVF_vect)
{
	tickscounter++;
}

#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0


void init_io() {
	DDRA |= SREG_STROBE;
	PORTA |= SREG_STROBE;
	DDRC |= SREG_CK | SREG_DATA;
	SREG_PORT |= SREG_CK;
	
	change_shift_reg(sreg);
	DDRD |= _BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7); // Pins mit pwm als Ausgänge
	
	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (non inverting mode).
	TCCR2B |= _BV(CS22);						// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);		// FastPWM bit 2, clk/64
	TIMSK2 |= _BV(TOIE2);

	//PWM

	TCNT2 = 255;   // pwm timer clear
	OCR2A = 255;   // pwm timer compare target
	OCR2B = 255;   // pwm timer compare target
	
	TCNT1 = 255;   // pwm timer clear
	OCR1A = 255;   // pwm timer compare target
	OCR1B = 255;   // pwm timer compare target

	DDRA &= ~(_BV(PA4) | _BV(PA7)); // Eingänge Taster / Türkontakt
	PORTA |= _BV(PA4);	// PULLUP Türkontakt
	send_status();
}

/*
  -
  helmer1
  helmer2
  drucker1
  drucker2
  fenster
  banner
  orga
*/

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
	send_status();
}

void switch_handler() {
	static uint16_t last_tickscounter;

	cli();
	uint16_t tc = tickscounter;
	sei();

	// alle 32 ticks ... 0.032 sekunden
	if ((tc - last_tickscounter) >= 20)
	{
		last_tickscounter = tc;



		static uint8_t counter_0;
		uint8_t clicked_0 = 0;
		uint8_t held_0    = 0;
		static uint8_t last_held_0;

		if (!(PINA & _BV(PA7)))
		{
			counter_0 ++;
			if (counter_0 > HOLD_THRESHOLD)
			{
				held_0 = 1;
				counter_0 = HOLD_THRESHOLD;
			}
		} else
		{
			if (counter_0 > CLICK_THRESHOLD)
			{
				if (counter_0 < HOLD_THRESHOLD)
				{
					clicked_0 = 1;
				}
			}
			counter_0 = 0;
		}

		if (clicked_0)
		{
			if (stat_licht) {
				sreg = 0;
				change_shift_reg(sreg);
				stat_licht = 0;
			}
			else {
				sreg = 212;
				change_shift_reg(sreg);
				pwm_set(pwm_matrix[F_PWM_FENSTER].port, 200);
				pwm_set(pwm_matrix[F_PWM_BANNER].port, 200);
				pwm_set(pwm_matrix[F_PWM_ORGATISCH].port, 200);
				stat_licht = 1;
			}
		}
		
		if (held_0)
		{

		}
		else if (last_held_0)
		{

		}
		
		last_held_0 = held_0;
	}
}


void pwm_set(volatile uint8_t *port, uint8_t value) {
	(*port) = value;
	send_status();
}


uint8_t pwm_get(volatile uint8_t *port) {
	return (*port);
}

