/* -*- Mode: C; tab-width: 2 -*- */
#ifndef POWERCOMMANDER_MATRIX_H
#define POWERCOMMANDER_MATRIX_H


static struct t_switch_parameter {
	volatile uint8_t *port;
	uint8_t pin;
} out_matrix[] = {
	{ O_SW00_PORT , O_SW00_PIN},
	{ O_SW01_PORT , O_SW01_PIN},
	{ O_SW02_PORT , O_SW02_PIN},
	{ O_SW03_PORT , O_SW03_PIN},
	{ O_SW04_PORT , O_SW04_PIN},
	{ O_SW05_PORT , O_SW05_PIN},
	{ O_SW06_PORT , O_SW06_PIN},
	{ O_SW07_PORT , O_SW07_PIN},
	{ O_SW08_PORT , O_SW08_PIN},
	{ O_SW09_PORT , O_SW09_PIN},
	{ O_SW0A_PORT , O_SW0A_PIN},
	{ O_SW0B_PORT , O_SW0B_PIN}
};

// das gleiche fuer die PWMs
static struct t_pwm_parameter {
	volatile uint8_t *port;
} pwm_matrix[] = {
	{ O_PWM00_PORT },
	{ O_PWM01_PORT },
	{ O_PWM02_PORT },
	{ O_PWM03_PORT },
	{ O_PWM04_PORT },
	{ O_PWM05_PORT }
};


#endif
