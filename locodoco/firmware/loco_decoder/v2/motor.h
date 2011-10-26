#include <avr/io.h>
#include "config.h"
#pragma once

#define MOTOR_FW() \
	PORT_MOTOR_RW &= ~(1<<PIN_MOTOR_RW); \
	PORT_MOTOR_FW |= (1<<PIN_MOTOR_FW)

#define MOTOR_RW() \
	PORT_MOTOR_FW &= ~(1<<PIN_MOTOR_FW); \
	PORT_MOTOR_RW |= (1<<PIN_MOTOR_RW)

#define MOTOR_OFF() \
	PORT_MOTOR_EN &= ~(1<<PIN_MOTOR_EN)

#define MOTOR_ON() \
	PORT_MOTOR_EN |= (1<<PIN_MOTOR_EN)

#if MOTOR_TIMER == 1
	/* clear on comp. match, fast pwm 10 bit */
	#define M_TIMER_INIT() \
		TCCR1A = (_BV(COM1B1) | _BV(WGM10) | _BV(WGM11)); \
		TCCR1B = (_BV(WGM12) | _BV(CS11))
	#define M_TIMER_MAX 0x3ff
	#define M_SET_PWM(a) (OCR1B = a)
#else
	#error "timer set in MOTOR_TIMER unknown, edit motor.h"
#endif

typedef motor_speed_t uint16_t;

/* initialize
 */
void motor_init(void);

/* stop the motor, eventually disable other
 * hardware to save some power
 */
void motor_sleep(void);

/* ...
 */
void motor_awake(void);

/* set the pwm value directly
 */
void motor_set_pwm (uint16_t in_v);

/* set the actual speed (unit: mm/s)
 */
void motor_set_speed (motor_speed_t in_speed);
