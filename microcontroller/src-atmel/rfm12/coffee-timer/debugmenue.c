#include <stdint.h>

#include "lcd.h"
#include "input.h"
#include "menu.h"
#include "cron.h"
#include "washing.h"
#include "fan.h"
#include "numprint.h"


void debugmenue ()
{
	input_hook (BTN_SELECT, dbg_exec);
	input_hook (BTN_UP,     wstuff_inc);
	input_hook (BTN_DOWN,   wstuff_dec);
	wstuff_query();
}

void wstuff_runprog (uint8_t in_runtime, uint8_t in_delay, uint8_t in_cycles)
{
	
}


/* query the user for the variables */
void wstuff_query ()
{
	lcd_clrscr();
	switch (mystate)
	{
		case WSTATE_INIT:
			lcd_puts ("Laufzeit: ");
			mystate = WSTATE_SET_RUNTIME;
		break;

		case WSTATE_SET_RUNTIME:
			runtime = tmp;
			lcd_puts ("Wartezeit: ");
			mystate = WSTATE_SET_WAITDELAY;
		break;
		
		case WSTATE_SET_WAITDELAY:
			delaytime = tmp;
			lcd_puts ("Zyklen: ");
			mystate = WSTATE_DONE;
		break;
		case WSTATE_DONE:
			numcycles = tmp;
			cron_add (runtime, wstuff_tick, 0);
			mystate = WSTATE_RUNNING;
			fan_on(0);
			menu_init(); /* reset hooks, display menu again */
		break;
	}
	tmp = 10;
}

void wstuff_inc()
{
	tmp++;
	lcd_gotoxy (12,0);
	print_uint8_lz (tmp);
}

void wstuff_dec()
{
	tmp--;
	lcd_gotoxy (12,0);
	print_uint8_lz (tmp);
}

/* reentrant function that is executed by cron.
 */
void wstuff_tick(volatile uint16_t in_dummy)
{
	if (!numcycles) return; /* rather unlikely */

	if (mystate == WSTATE_RUNNING)
	{
		numcycles--;
		fan_off(0);
		mystate = WSTATE_WAITING;
		cron_add ((60 * runtime), wstuff_tick, 0);
	} else if (mystate == WSTATE_WAITING)
	{
		fan_on(0);
		mystate = WSTATE_RUNNING;
		cron_add ((60 * delaytime), wstuff_tick, 0);
	}
}
