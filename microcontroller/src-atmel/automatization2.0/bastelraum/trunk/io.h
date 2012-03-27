#ifndef _IO_H
#define _IO_H


#define SREG_PORT	PORTC
#define SREG_CK		_BV(PC4)
#define SREG_DATA	_BV(PC5)
#define SREG_STROBE	_BV(PA5)


#define O_PWM00_PORT (&(OCR2A))
#define O_PWM01_PORT (&(OCR2B))
#define O_PWM02_PORT (&(OCR1AL))
#define O_PWM03_PORT (&(OCR1BL))


extern volatile uint8_t sreg;

// das gleiche fuer die PWMs
static struct t_pwm_parameter {
	volatile uint8_t *port;
} pwm_matrix[] = {
	{ O_PWM00_PORT },
	{ O_PWM01_PORT },
	{ O_PWM02_PORT },
	{ O_PWM03_PORT }
};


void switch_handler();
void state_to_output();
void change_shift_reg(uint8_t sreg);
void pwm_set(volatile uint8_t *port, uint8_t value);
uint8_t pwm_get(volatile uint8_t *port);

void init_io();

#endif // _IO

