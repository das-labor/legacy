#ifndef _IO_H
#define _IO_H


// ports - addressen
#define O_SW00_PORT (&(PORTC))
#define O_SW01_PORT (&(PORTC))
#define O_SW02_PORT (&(PORTC))
#define O_SW03_PORT (&(PORTC))
#define O_SW04_PORT (&(PORTB))

// PINs - uin8_t
#define O_SW00_PIN (PC3)
#define O_SW01_PIN (PC2)
#define O_SW02_PIN (PC1)
#define O_SW03_PIN (PC0)
#define O_SW04_PIN (PB5)

#define O_PWM00_PORT (&(OCR0A))
#define O_PWM01_PORT (&(OCR0B))
#define O_PWM02_PORT (&(OCR1AL))
#define O_PWM03_PORT (&(OCR1BL))

static struct t_switch_parameter {
	volatile uint8_t *port;
	uint8_t pin;
} sw_matrix[] = {
	{ O_SW00_PORT , O_SW00_PIN},
	{ O_SW01_PORT , O_SW01_PIN},
	{ O_SW02_PORT , O_SW02_PIN},
	{ O_SW03_PORT , O_SW03_PIN},
	{ O_SW04_PORT , O_SW04_PIN}
};

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

#endif // _IO

