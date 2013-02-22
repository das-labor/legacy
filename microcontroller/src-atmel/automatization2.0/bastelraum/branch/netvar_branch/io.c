#include <avr/io.h>
#include <util/delay.h>

#include "config.h"
#include "io.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "Bastelcmd.h"


static struct t_pwm_parameter {
	volatile uint8_t *port;
} pwm_matrix[] = {
	{ O_PWM00_REG },
	{ O_PWM01_REG },
	{ O_PWM02_REG },
	{ O_PWM03_REG }
};

// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Taster            1 = pressed, 0 = released
// Türkontakt        1 = on, 0 = off

static struct t_pin_parameter {
	uint8_t state;
	int8_t debounce_count;
	uint8_t inverted;
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ 0, 0, 1, (&(I_PIN_0)), I_BV_0},
	{ 0, 0, 0, (&(I_PIN_1)), I_BV_1}
};

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

static void change_shift_reg(uint8_t sreg) {
	for (int i = 0; i < 8; i++)
	{
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

static void set_output(uint8_t port, uint8_t val) {
	static uint8_t sreg = 0;

	if (val)
		sreg |= 1 << port;
	else
		sreg &= ~(1 << port);
	change_shift_reg(sreg);
}

void init_io() {
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
}

static void set_pwm(uint8_t port, uint8_t value)
{
	(*pwm_matrix[port].port) = value;
}

enum {
	LAMP_OUT_FENSTER,
	LAMP_OUT_MITTE,
	LAMP_OUT_NISCHE,
	LAMP_OUT_BTISCHL_1,
	LAMP_OUT_BTISCHL_2,
	LAMP_OUT_BTISCHR_1,
	LAMP_OUT_BTISCHR_2
};

static void lamp_out(void *num, uint8_t val) {
	uint8_t i = (uint8_t) (uint16_t) num;
	switch (i) {
		case LAMP_OUT_FENSTER:
			set_pwm   (F_PWM_FENSTER, val);
			set_output(F_REG_FENSTER, val);
			break;
		case LAMP_OUT_MITTE:
			set_pwm   (F_PWM_MITTE, val);
			set_output(F_REG_MITTE, val);
			break;
		case LAMP_OUT_NISCHE:
			set_pwm   (F_PWM_NISCHE, val);
			set_output(F_REG_NISCHE, val);
			break;
		case LAMP_OUT_BTISCHL_1:
			set_output(F_REG_BTISCHL_1, val);
			break;
		case LAMP_OUT_BTISCHL_2:
			set_output(F_REG_BTISCHL_2, val);
			break;
		case LAMP_OUT_BTISCHR_1:
			set_output(F_REG_BTISCHR_1, val);
			break;
		case LAMP_OUT_BTISCHR_2:
			set_output(F_REG_BTISCHR_2, val);
			break;
	}
}

static netvar_desc *out_netvars[NUM_INPUTS];

void switch_netvars_init() {
	out_netvars[0] = netvar_register(0x0103, 0x2f, 1); // Taster
	out_netvars[1] = netvar_register(0x0005, 0x00, 1); // Türkontakt
}

#define NV_IDX_LAMP_CONTROLLER_BASTEL 0x0103

void lamp_out_init() {
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x50, lamp_out, (void *) LAMP_OUT_FENSTER);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x51, lamp_out, (void *) LAMP_OUT_MITTE);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x52, lamp_out, (void *) LAMP_OUT_NISCHE);
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x53, lamp_out, (void *) F_REG_BTISCHL_1);
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x54, lamp_out, (void *) F_REG_BTISCHL_2);
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x55, lamp_out, (void *) F_REG_BTISCHR_1);
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_BASTEL, 0x56, lamp_out, (void *) F_REG_BTISCHR_2);
}


static void input_changed_event(uint8_t num, uint8_t val) {

	netvar_write(out_netvars[num], &val);
}

#define DEBOUNCE_CYCLES 2

/*
*  debounce monitored inputs, invert them if wanted, and check for changes
*  on change: call input_changed_event
*/

void switch_handler() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if ((*pin_matrix[i].pin) & pin_matrix[i].bit) {
			pin_matrix[i].debounce_count++;
			if (pin_matrix[i].debounce_count > DEBOUNCE_CYCLES) {
				pin_matrix[i].debounce_count = DEBOUNCE_CYCLES;
				if (pin_matrix[i].state == pin_matrix[i].inverted) {
					pin_matrix[i].state = pin_matrix[i].inverted ^ 1;
					input_changed_event(i, pin_matrix[i].state);
				}
			}
		} else {
			pin_matrix[i].debounce_count--;
			if (pin_matrix[i].debounce_count < 0) {
				pin_matrix[i].debounce_count = 0;
				if (pin_matrix[i].state == (pin_matrix[i].inverted ^ 1)) {
					pin_matrix[i].state = pin_matrix[i].inverted;
					input_changed_event(i, pin_matrix[i].state);
				}
			}
		}
	}
}

