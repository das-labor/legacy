#include <avr/io.h>
#include <util/delay.h>

#define SREG_PORT	PORTC
#define SREG_CK		_BV(PC6)
#define SREG_DATA	_BV(PC7)
#define SREG_STROBE	_BV(PA5)

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


void init_io() {
	DDRA |= SREG_STROBE;
	PORTA |= SREG_STROBE;
	DDRC |= SREG_CK;
	SREG_PORT |= SREG_CK;

	TCCR2A |= _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);	// FastPWM, Set OC2X on Compare Match, clear OC2X at BOTTOM, (non inverting mode).
	TCCR2B |= _BV(CS22);										// clk/64

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);			// FastPWM bit 2, clk/64

	//PWM

	TCNT2 = 255;   // pwm timer clear
	OCR2A = 255;   // pwm timer compare target
	OCR2B = 255;   // pwm timer compare target
	
	TCNT1 = 255;   // pwm timer clear
	OCR1A = 255;   // pwm timer compare target
	OCR1B = 255;   // pwm timer compare target

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
