


static struct t_switch_parameter {
	volatile uint8_t *port;
	uint8_t pin;
} sw_matrix[] = {
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


void switch_on(volatile uint8_t *port, uint8_t pin)
{
	(*port) |= _BV(pin);
}


void switch_off(volatile uint8_t *port, uint8_t pin)
{
	(*port) &= ~_BV(pin);
}


void switch_status(volatile uint8_t *port, uint8_t pin, uint8_t *result)
{
	(*result) = ((*port) >> pin) & 1;
}


void pwm_set(volatile uint8_t *port, uint8_t value)
{
	(*port) = value;
}


void pwm_get(volatile uint8_t *port, uint8_t *result)
{
	(*result) = (*port);
}
