#include <avr/pgmspace.h>

#include <stdint.h>
#include "lcd.h"
#include "cron.h"
#include "menu.h"
#include "fan.h"
#include "washing.h"
#include "input.h"
#include "clock.h"

#define MTOP_DEFAULT 10

static uint8_t midx = 0, mtop = MTOP_DEFAULT;

const menuentry_t mymenu[] =
	{
		(menuentry_t) {
			"5  Minuten",
			1, 300,
			fan_kick
		},
		(menuentry_t) {
			"10 Minuten",
			2, 600,
			fan_kick
		},
		(menuentry_t) {
			"15 Minuten",
			3, 900,
			fan_kick
		},
		(menuentry_t) {
			"20 Minuten",
			4, 1200,
			fan_kick
		},
		(menuentry_t) {
			"25 Minuten",
			5, 1500,
			fan_kick
		},
		(menuentry_t) {
			"Duschprogramm",
			6, 0x0A0F, /* after 10 minutes run for 15 minutes */
			fan_delaykick
		},
		(menuentry_t) {
			"Waesche >>",
			7, 0,
			wstuff_custom
		},
		(menuentry_t) {
			"Aus",
			8, 0,
			fan_off
		},
		(menuentry_t) {
			"Dauer-An",
			9, 0,
			fan_on
		},
		(menuentry_t) {
			"debug >>",
			0, 0,
			mdebug_jump
		},


	/* DEBUG STUFF */
		(menuentry_t) {
			"clear cron",
			11, 0,
			cron_init
		},
		(menuentry_t) {
			"reboot",
			12, 0,
			dbg_reboot
		},
		(menuentry_t) {
			"set clock",
			13, 0,
			clk_set
		},
		(menuentry_t) {
			"<<<",
			10, 0,
			menu_init
		}
	};

void dbg_reboot ()
{
	const void ((*zfunc)()) = 0x00;
	zfunc();
}


void mdebug_jump (uint16_t foo)
{
	midx = 10;
	mtop = 12;
	menu_display();
}

void menu_display()
{
	lcd_clrscr();
	lcd_gotoxy (0,0);
	
	lcd_putc(0x7e); /* right arrow */
	
	lcd_puts (mymenu[midx].name);
	lcd_gotoxy (15,0);
	
	lcd_putc(0x7f); /* left arrow */

	lcd_gotoxy (0,1);
	lcd_puts(" ");
	lcd_puts(mymenu[mymenu[midx].next].name);
}

void menu_select() 
{
	if (mymenu[midx].func != NULL)
		(mymenu[midx].func)(mymenu[midx].func_param);
}

void menu_init()
{

	midx = 0;
	mtop = MTOP_DEFAULT;
	
	/* set up input hooks */
	input_hook (BTN_UP,     menu_last);
	input_hook (BTN_DOWN,   menu_next);
	input_hook (BTN_SELECT, menu_select);

	menu_display();
}

void menu_last()
{
	if (midx == MTOP_DEFAULT + 1 || midx == 0)
		midx = mtop;

	midx--;
	menu_display();
	return;
}

void menu_next()
{
	midx = mymenu[midx].next;
	menu_display();
}
