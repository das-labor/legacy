#include <avr/io.h>
#include <util/delay.h>

#include "can_handler.h"
#include "config.h"
#include "io.h"
#include "motion.h"
#include "Bastelcmd.h"


static struct t_pwm_parameter {
	volatile uint8_t *port;
} pwm_matrix[] = {
	{ O_PWM00_REG },
	{ O_PWM01_REG },
	{ O_PWM02_REG },
	{ O_PWM03_REG }
};

static uint8_t sreg = 0;

#define HOLD_THRESHOLD 26
#define CLICK_THRESHOLD 0

/*
  -
  arbeitstisch_r_1
  arbeitstisch_r_2
  arbeitstisch_l_1
  arbeitstisch_l_2
  fenster
  mitte
  nische
*/

static void change_shift_reg(uint8_t new_sreg)
{
	for (int i = 0; i < 8; i++)
	{
		if (new_sreg & (1 << i))
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


void set_output(uint8_t port, uint8_t val)
{
	if (port < F_REG_COUNT)
	{
		if (val)
			sreg |= 1 << port;
		else
			sreg &= ~(1 << port);
		change_shift_reg(sreg);
	}
}


uint8_t get_outputs()
{
	return sreg;
}


void init_io()
{
	// Init shiftregister
	DDRA |= SREG_STROBE;
	PORTA |= SREG_STROBE;
	DDRC |= SREG_CK | SREG_DATA;
	SREG_PORT |= SREG_CK;

	change_shift_reg(0); // set default status

	DDRD |= _BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7); // Pins mit pwm als Ausgang

	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (non inverting mode).
	TCCR2B |= _BV(CS22);						// clk/64
	TIMSK2 |= _BV(TOIE2);						// Overflow interrupt für timer

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);		// FastPWM bit 2, clk/64

	// PWM default values

	TCNT2 = 255;   // pwm timer clear
	OCR2A = 255;   // pwm timer compare target
	OCR2B = 255;   // pwm timer compare target

	TCNT1 = 255;   // pwm timer clear
	OCR1A = 255;   // pwm timer compare target
	OCR1B = 255;   // pwm timer compare target

	DDRA  &= ~(_BV(PA4) | _BV(PA7)); // Eingänge Türkontakt / Taster
	PORTA |= _BV(PA4);	// PULLUP Türkontakt
	send_status();
}


void set_pwm(uint8_t port, uint8_t value)
{
	if (port < F_PWM_COUNT)
		(*pwm_matrix[port].port) = value;
	send_status();
}


uint8_t get_pwm(uint8_t port)
{
	return (*pwm_matrix[port].port);
}


void switch_handler()
{
	static uint8_t counter, last_held;
	uint8_t clicked = 0, held = 0;

	if (!(PINA & _BV(PA7)))
	{
		counter ++;
		if (counter > HOLD_THRESHOLD)
		{
			held = 1;
			counter = HOLD_THRESHOLD;
		}
	} else
	{
		if (counter > CLICK_THRESHOLD)
		{
			if (counter < HOLD_THRESHOLD)
			{
				clicked = 1;
			}
		}
		counter = 0;
	}

	if (clicked)
	{
		if (sreg) {
			sreg = 0;
			change_shift_reg(0);
		}
		else {
			sreg = 212;
			change_shift_reg(212);
			set_pwm(F_PWM_FENSTER, 200);
			set_pwm(F_PWM_MITTE, 200);
			set_pwm(F_PWM_NISCHE, 200);
		}
	}
	if (held)
	{
		// dim
	}
	else if (last_held)
	{
		// dimmdir
	}
	last_held = held;
}

