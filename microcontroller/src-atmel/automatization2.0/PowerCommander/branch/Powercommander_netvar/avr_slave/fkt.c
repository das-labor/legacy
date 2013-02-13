/*
	set-ter funktionen fuer den i2cslave
*/

#include "../include/PowerCommander.h"

#include "fkt.h"


//Tabellen mit den passenden Registern und Bitmaksen für die Schaltausgänge
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


//void switch_set(uint8_t output, uint8_t value);
//void pwm_set(uint8_t port, uint8_t value);

t_outputdata outputdata = {0, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

static void switch_set(uint8_t output, uint8_t value)
{
	if (value == 1)
		(*sw_matrix[output].port) |= sw_matrix[output].pin;
	else
		(*sw_matrix[output].port) &= ~(sw_matrix[output].pin);
}

static void pwm_set(uint8_t port, uint8_t value)
{
	(*pwm_matrix[port].port) = value;
}

// set DDR to output and PORT to 0
static void output_init(uint8_t output)
{
	(*sw_matrix[output].port) &= ~(sw_matrix[output].pin); //port off

	//the DDR-Register is one memmory address before port on AVR
	volatile uint8_t * ddr = sw_matrix[output].port - 1; 
	*ddr |= sw_matrix[output].pin; //set the ddr
}


static void init_pwm()
{
	// Use all six PWM outputs of the three timers for fast 8 bit PWM
	// with clk/64 = 250kHz which divided by 256 gives a 976Hz pwm cycle

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

	DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3); // Pins mit pwm als ausgänge
	DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6); // Pins mit pwm als ausgänge
}

void init_outputs()
{
	init_pwm();
	
	for (uint8_t i = 0; i < NUM_OUTPUTS; i++)
		output_init(i); //set all the DDRs and clear outputs
}


void set_outputs() {
	for (uint8_t i = 0; i < NUM_OUTPUTS; i++)
		switch_set(i, ((outputdata.ports) >> i) & 0x01);

	for (uint8_t i = 0; i < PWM_CHAN; i++)
		pwm_set(i, outputdata.pwmval[i]);
}
