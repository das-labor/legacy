#include "cron.h"
#include "config.h"
#include "powerplug.h"
#include "coffee.h"
#include "numprint.h"
#include "input.h"
#include "lcd.h"
#include "menu.h"

static uint8_t tv;

void send_c_on (uint16_t in_time)
{
	if (in_time > 0)
	{
		cron_add (in_time, send_c_on, 0);
		return;
	}

	/* set power plug to on */
	pp_send (1, COFFEE_CODE);
}

void send_c_off (uint16_t in_time)
{
	if (in_time > 0)
	{
		cron_add (in_time, send_c_off, 0);
		return;
	}

	/* set power plug to off */
	pp_send (0, COFFEE_CODE);
}


void timer_inc ()
{
	tv++;
	time_c_custom(1);
}

void timer_dec ()
{
	tv--;
	time_c_custom(1);
}

void leave_timer()
{
	if (tv > 0)
		send_c_on (tv * 60);
	menu_init();
}

void tmr_init()
{
	input_hook (BTN_SELECT, leave_timer);
	input_hook (BTN_UP,     timer_inc);
	input_hook (BTN_DOWN,   timer_dec);
	tv = 0;
}

void time_c_custom (uint16_t in_val)
{
	if (in_val == 0)
		tmr_init();
	lcd_clrscr();
	lcd_puts ("Start in ");
	print_uint8_lz (tv);
	lcd_puts ("m");
}
