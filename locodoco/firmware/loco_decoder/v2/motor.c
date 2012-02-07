#include "motor.h"
volatile static uint16_t motor_step_table[MOTOR_ADC_STEPS];
volatile static uint16_t target_speed = 0x00;

void motor_init()
{
	DDR_MOTOR_EN |= _BV(PIN_MOTOR_EN);
	DDR_MOTOR_FW |= _BV(PIN_MOTOR_FW);
	DDR_MOTOR_RW |= _BV(PIN_MOTOR_RW);
	MOTOR_FW(); /* set a defined state */
	//M_TIMER_INIT();
	MOTOR_OFF();

	// XXX read table from eeprom
}

void motor_set_pwm (uint16_t in_v)
{
	M_SET_PWM(in_v);
}

void motor_set_speed (motor_speed_t in_speed)
{
	uint8_t i,k;
	uint16_t d;
	
	for (i=0;i<MOTOR_ADC_STEPS;i++)
	{
		if (motor_step_table[i] > in_speed)
			break;
	}

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

