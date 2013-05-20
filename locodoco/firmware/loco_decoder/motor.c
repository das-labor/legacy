#include "motor.h"

volatile static uint16_t m_sampleval = 0x0000;
volatile static uint16_t m_targetval = 0x0002;
volatile static uint8_t motor_state = 0x00;
volatile static uint8_t motor_dir = MOTOR_DIR_FW;

void motor_set_sampleval (uint16_t in_val)
{
	static uint8_t c = 0;
	m_sampleval += in_val;
	
	c++;

	if (c < 8)
		return;
	
	motor_state |= MOTOR_SAMPLE_READY;
	c = 0;
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
	static uint16_t c = 0, c2 = 0;
	uint8_t txbuf[sizeof(ldc_header_t) + 2];
	ldc_header_t *h = (ldc_header_t*) txbuf;

	if (motor_state & MOTOR_SAMPLE_READY)
	{
		m_sampleval >>= 3;
		motor_adjust_speed();
		motor_state &= ~(MOTOR_SAMPLE_READY);

		h->ltype = LDC_CMD_SPEED_GET;
		h->addr = 0xff;
		*((uint16_t *) (txbuf + sizeof(ldc_header_t))) = m_sampleval;
		//rfm12_tx (sizeof(ldc_header_t) + sizeof(uint16_t), LDC_TYPE, txbuf);
	}
	
	c++;
	
	if (c < 255)
		return;
	
	c2++;

	if (c2 < 250)
		return;

	c2 = 0;
	adcd_ctl (ADCD_SAMPLE_MOTOR);
}

void motor_set_target_speed (uint16_t in_val)
{
	m_targetval = 0;
}

void motor_set_speed_cmd (ldc_speed_t *in_s)
{
	motor_set_direction (in_s->dir);
	m_targetval = in_s->speed;
}

void motor_set_direction (uint8_t in_dir)
{
	if (in_dir == motor_dir)
		return;
	
	M_OCR = 0;
	
	M_EN_PORT &= ~(M_EN_PIN);
	
	if (in_dir == MOTOR_DIR_RW)
	{
		M_FW_PORT &= ~(M_FW_PIN);
		M_RW_PORT |= M_RW_PIN;
	} else
	{
		M_RW_PORT &= ~(M_RW_PIN);
		M_FW_PORT |= M_FW_PIN;
	}
	
	M_EN_PORT |= (M_EN_PIN);
}
