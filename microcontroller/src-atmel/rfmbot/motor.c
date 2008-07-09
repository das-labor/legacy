#include <avr/io.h>
#include "motor.h"

void motor_ctrl_direct (uint8_t in_val)
{
	if (in_val & RB_LEFT)
	{
		if (in_val & RB_DIR_FW)
		{
			RB_EN_L_PORT &= ~(RB_EN_L_BIT);
			RB_RW_L_PORT &= ~(RB_RW_L_BIT);
			RB_FW_L_PORT |= (RB_FW_L_BIT);
			RB_EN_L_PORT |= (RB_EN_L_BIT);
		} else if (in_val & RB_DIR_RW)
		{
			RB_EN_L_PORT &= ~(RB_EN_L_BIT);
			RB_FW_L_PORT &= ~(RB_FW_L_BIT);
			RB_RW_L_PORT |= (RB_RW_L_BIT);
			RB_EN_L_PORT |= (RB_EN_L_BIT);
		} else
		{
			RB_EN_L_PORT &= ~(RB_EN_L_BIT);
			RB_FW_L_PORT &= ~(RB_FW_L_BIT);
			RB_RW_L_PORT &= ~(RB_RW_L_BIT);
		}
	}
	
	if (in_val & RB_RIGHT)
	{
		if (in_val & RB_DIR_FW)
		{
			RB_EN_R_PORT &= ~(RB_EN_R_BIT);
			RB_RW_R_PORT &= ~(RB_RW_R_BIT);
			RB_FW_R_PORT |= (RB_FW_R_BIT);
			RB_EN_R_PORT |= (RB_EN_R_BIT);
		} else if (in_val & RB_DIR_RW)
		{
			RB_EN_R_PORT &= ~(RB_EN_R_BIT);
			RB_FW_R_PORT &= ~(RB_FW_R_BIT);
			RB_RW_R_PORT |= (RB_RW_R_BIT);
			RB_EN_R_PORT |= (RB_EN_R_BIT);
		} else
		{
			RB_EN_R_PORT &= ~(RB_EN_R_BIT);
			RB_FW_R_PORT &= ~(RB_FW_R_BIT);
			RB_RW_R_PORT &= ~(RB_RW_R_BIT);
		}
	}
}

void motor_init (void)
{
	RB_EN_L_DRR |= RB_EN_L_BIT;
	RB_EN_R_DRR |= RB_EN_R_BIT;

	RB_RW_L_DRR |= RB_RW_L_BIT;
	RB_RW_R_DRR |= RB_RW_R_BIT;

	RB_FW_L_DRR |= RB_FW_L_BIT;
	RB_FW_R_DRR |= RB_FW_R_BIT;

	RB_FW_R_PORT &= ~(RB_FW_R_BIT);
	RB_RW_R_PORT &= ~(RB_RW_R_BIT);

	RB_FW_L_PORT &= ~(RB_FW_L_BIT);
	RB_RW_L_PORT &= ~(RB_RW_L_BIT);
	
	RB_EN_L_PORT &= ~(RB_EN_L_BIT);
	RB_EN_R_PORT &= ~(RB_EN_R_BIT);
}
