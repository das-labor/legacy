#include <avr/io.h>
#include "hgrobo_config.h"

/******************
*
*	COMMON MOTOR FUNCTIONS
*
*******************/

//motor initialization
void mot_init()
{
	//enable motor outputs
	MOT_PORT_DDR |= _BV(MOT_PIN_FWD1) | _BV(MOT_PIN_RWD1) | _BV(MOT_PIN_FWD2) | _BV(MOT_PIN_RWD2) | _BV(MOT_PIN_ENA2);

	//turn off all stuff
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_FWD1) | _BV(MOT_PIN_RWD1) | _BV(MOT_PIN_FWD2) | _BV(MOT_PIN_RWD2) /*| _BV(MOT_PIN_ENA2 //only for brake motors) */);
}


//pwm inititalization
void pwm_init()
{
	//clear compare registers
	OCR1A = 0;
	OCR1B = 0;

	//PWM ocr1a, ocr1b + fastPWM 8bit
	TCCR1A = (_BV(COM1A1) | _BV(COM1B1) | _BV(WGM10));

	//fastPWM 8bit + ioclk / 1024
	TCCR1B = (_BV(WGM12) | _BV(CS12) | _BV(CS10));

	//reset counter
	TCNT1 = 0;

	//enable pins
	PWM_PORT_DDR |= _BV(PWM_PIN_A) | _BV(PWM_PIN_B);
}
