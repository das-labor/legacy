#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "fan.h"
#include "input.h"
#include "lcd.h"
#include "menu.h"

//typedef (void ((*)())) fnptr; // myhooks[NUM_BTNS];
//static fnptr myhooks[NUM_BTNS]; // myhooks[NUM_BTNS];
static void (*myhooks[NUM_BTNS]) ();

/* @description set a function that's executed when a given button is pressed.
 */
void input_hook (uint8_t in_btn, void ((*in_func)()))
{
	myhooks[in_btn] = in_func;
}

void input_init ()
{
#if 0
	/* interrupt */
	MCUCR |= (_BV(ISC11)); // | _BV(ISC11));
	GICR |= _BV(INT1);
#endif
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
	static uint8_t upcnt = 0, dncnt = 0, selstate = 0, lasta = 0;
	
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
			lcd_gotoxy (0,1); lcd_putc ('D');
			input_exec (BTN_DOWN);
		} else
		{
			input_exec (BTN_UP);
			lcd_gotoxy (0,1); lcd_putc ('U');
		}
	}
	lasta = (PIN_RT_A & BV_RT_A);
}
