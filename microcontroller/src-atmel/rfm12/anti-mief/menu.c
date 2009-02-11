#include <stdint.h>
#include "lcd.h"
#include "cron.h"
#include "menu.h"
#include "fan.h"
#include "washing.h"
#include "input.h"

menuentry_t mymenu[9];
static uint8_t entactive = 0;
static uint8_t midx;

void menu_display()
{
	lcd_clrscr();
	lcd_gotoxy (0,0);
	
	if (entactive)
		lcd_putc('[');
	else
		lcd_putc(0x7e); /* right arrow */
	
	lcd_puts (mymenu[midx].name);
	lcd_gotoxy (15,0);
	
	if (entactive)
		lcd_putc(']');
	else
		lcd_putc(0x7f); /* left arrow */

	lcd_gotoxy (0,1);
	lcd_puts(" ");
	lcd_puts(mymenu[mymenu[midx].next].name);
}

void menu_setactive ()
{
	entactive = 1;
	menu_display();
}

void menu_setinactive ()
{
	entactive = 0;
	menu_display();
}



void menu_select() 
{
	if (mymenu[midx].func != NULL)
		(mymenu[midx].func)(mymenu[midx].func_param);
}

void menu_init()
{
	mymenu[0] =
		(menuentry_t) {
			"5  Minuten",
			1, 300,
			fan_kick
		};

	mymenu[1] =
		(menuentry_t) {
			"10 Minuten",
			2, 600,
			NULL
		};

	mymenu[2] =
		(menuentry_t) {
			"15 Minuten",
			3, 900,
			fan_kick
		};

	mymenu[3] =
		(menuentry_t) {
			"20 Minuten",
			4, 1200,
			fan_kick
		};

	mymenu[4] =
		(menuentry_t) {
			"25 Minuten",
			5, 1500,
			fan_kick
		};

	mymenu[5] =
		(menuentry_t) {
			"Duschprogramm",
			6, 0x0A0F, /* after 10 minutes run for 15 minutes */
			fan_delaykick
		};

	mymenu[6] =
		(menuentry_t) {
			"Waesche (+)",
			7, 0,
			wstuff_custom
		};

	mymenu[7] =
		(menuentry_t) {
			"Aus",
			8, 0,
			fan_off
		};

	mymenu[8] =
		(menuentry_t) {
			"Dauer-An",
			0, 0,
			fan_on
		};

	midx = 0;
	
	/* set up input hooks */
	input_hook (BTN_UP,     menu_last);
	input_hook (BTN_DOWN,   menu_next);
	input_hook (BTN_SELECT, menu_select);

	cron_add (2, menu_next, 0);
	lcd_puts("m init");
}

void menu_last()
{
	uint8_t i = 0;

	if (midx == 0)
		midx = 9;

	midx--;
	menu_display();
	return;
}

void menu_next()
{
	midx = mymenu[midx].next;
	menu_display();
}
