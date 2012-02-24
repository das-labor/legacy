#include "motor.h"
volatile static uint16_t motor_step_table[MOTOR_ADC_STEPS+2];
volatile static uint16_t target_speed = 0x00;

void motor_init()
{
	DDR_MOTOR_EN |= _BV(PIN_MOTOR_EN);
	DDR_MOTOR_FW |= _BV(PIN_MOTOR_FW);
	DDR_MOTOR_RW |= _BV(PIN_MOTOR_RW);
	MOTOR_FW(); /* set a defined state */
	//M_TIMER_INIT();
	MOTOR_OFF();
	eeprom_read_block (&motor_step_table, EEP_STEP_TABLE, sizeof(motor_step_table));
}

void motor_set_pwm (uint16_t in_v)
{
	M_SET_PWM(in_v);
}

void motor_set_speed (motor_speed_t in_speed)
{
	uint8_t i;
	uint16_t d, v = MOTOR_STEP_SIZE;
	uint16_t ts;
			break;
		}
		v += MOTOR_STEP_SIZE;
	}
	ts = motor_step_table[i] * (d >> MOTOR_STEP_SHIFT);
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

