#include <avr/io.h>
#include <stdint.h>
#include "motor.h"

static motor_state_t mstate[2];

uint8_t motor_getstate (uint8_t in_num)
{
	return mstate[in_num].dststate;
}

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

	mstate[IDX_R].dststate = 0;
	mstate[IDX_R].cstate = 0;
	
	mstate[IDX_L].dststate = 0;
	mstate[IDX_L].cstate = 0;
}

void motor_ctrl (uint8_t in_val)
{
	if (in_val & RB_LEFT)
		mstate[IDX_R].dststate = in_val;
	
	if (in_val & RB_RIGHT)
		mstate[IDX_L].dststate = in_val;
}

void motor_pwm (void)
{
	static uint8_t mycnt;
	static uint8_t stateoffset[2] = {0, 0};
	uint8_t i;
	
	for (i=0;i<2;i++)
	{
		if (stateoffset[i] >= mycnt && mstate[i].dststate != mstate[i].cstate)
		{
			motor_ctrl_direct (mstate[i].dststate);

			if (stateoffset[i] == 0xff) mstate[i].cstate = mstate[i].dststate;
			if (mycnt == 0) stateoffset[i]++;
		} else if (mstate[i].dststate != mstate[i].cstate) // && stateoffset > mycnt
		{
			motor_ctrl_direct (RB_RIGHT | RB_LEFT);
			if (mycnt == 0xff) stateoffset[i]++;
		} else if (mstate[i].dststate == mstate[i].cstate)
		{
			stateoffset[i] = 0;
		}
	}
	mycnt++;
}
