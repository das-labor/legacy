#include "motor.h"
void motor_init()
{
	DDR_MOTOR_EN |= _BV(PIN_MOTOR_EN);
	MOTOR_OFF();
	DDR_MOTOR_FW |= _BV(PIN_MOTOR_FW);
	DDR_MOTOR_RW |= _BV(PIN_MOTOR_RW);
	MOTOR_FW(); /* set a defined state */
}

void motor_sleep()
{
	MOTOR_OFF();
	#ifdef PRR
	PRR |= _BV(PRTIM1);
	#endif
}

void motor_awake()
{
	#ifdef PRR
	PRR &= ~(_BV(PRTIM1));
	#endif
}
