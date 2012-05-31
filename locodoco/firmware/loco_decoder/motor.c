#include "motor.h"

volatile static uint16_t m_sampleval = 0x0000;
volatile static uint16_t m_targetval = 0x0020;
volatile static uint8_t motor_state = 0x00;

void motor_set_sampleval (uint16_t in_val)
{
	m_sampleval = in_val;
	motor_state |= MOTOR_SAMPLE_READY;
}

void motor_init ()
{
	M_EN_DDR  |= M_EN_PIN;
	//M_EN_PORT &= ~(M_EN_PIN);
	M_EN_PORT |= (M_EN_PIN);

	M_FW_DDR  |= M_FW_PIN;
	M_FW_PORT &= ~(M_FW_PIN);
	
	M_RW_DDR  |= M_RW_PIN;
	M_RW_PORT |= M_RW_PIN;
	
	M_TIMER_INIT();
}

void motor_adjust_speed ()
{
	
	if (m_sampleval < m_targetval) /* increase speed */
	{
		if (M_OCR < M_OCR_MAX)
			M_OCR++;
	}

	if (m_sampleval > m_targetval) /* decrease speed */
	{
		if (M_OCR)
			M_OCR--;
	}
}

/* motor control "daemon"
 */
void motor_tick ()
{
	static uint16_t c = 0;

	if (motor_state & MOTOR_SAMPLE_READY)
	{
		motor_adjust_speed();
		motor_state &= ~(MOTOR_SAMPLE_READY);
	}
	
	c++;
	
	if (c < 50)
		return;
	
	c = 0;
	adcd_ctl (ADCD_SAMPLE_MOTOR);
}
