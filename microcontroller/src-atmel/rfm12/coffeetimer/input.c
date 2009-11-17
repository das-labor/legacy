#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "input.h"

static void (*myhooks[NUM_BTNS]) ();

void *input_hook (uint8_t in_btn, void ((*in_func)()))
{
	void (*oldhook)() = myhooks[in_btn];
	myhooks[in_btn] = in_func;
	return oldhook;
}

void input_init ()
{
	DDR_RT_A &= ~(BV_RT_A);
	DDR_RT_B &= ~(BV_RT_B);
	DDR_RT_SELECT &= ~(BV_RT_SELECT);
	
	/* pullups */
	PORT_RT_A      |= (BV_RT_A);
	PORT_RT_B      |= (BV_RT_B);
	PORT_RT_SELECT |= (BV_RT_SELECT);
}

void input_exec (uint8_t in_btn)
{
	myhooks[in_btn]();
}

void rot_poll ()
{
	static uint8_t selstate = 0, lasta = 0;
	
	if (!(PIN_RT_SELECT & BV_RT_SELECT) && selstate != 1)
	{
		input_exec (BTN_ANY);
		input_exec (BTN_SELECT);
		selstate = 1;
	}

	if (PIN_RT_SELECT & BV_RT_SELECT)
		selstate = 0;
	
	if ((PIN_RT_A & BV_RT_A) != lasta)
	{
		input_exec (BTN_ANY);
		if (((PIN_RT_A & BV_RT_A) && (PIN_RT_B & BV_RT_B))
			|| (!(PIN_RT_A & BV_RT_A) && !(PIN_RT_B & BV_RT_B)))
		{
			input_exec (BTN_DOWN);
		} else
		{
			input_exec (BTN_UP);
		}
	}
	lasta = (PIN_RT_A & BV_RT_A);
}
