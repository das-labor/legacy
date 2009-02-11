#include <stdint.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "clock.h"
#include "input.h"
#include "menu.h"
#include "numprint.h"

static uint8_t nh = 12, ns = 0, nm = 30;
static uint8_t hours = 0, minutes = 0, seconds = 0;
static uint8_t *valptr = &ns;


void clk_print ()
{
	lcd_clrscr();
	
	lcd_gotoxy(4,0);
	print_uint8_lz2 (hours);
	lcd_putc (':');
	print_uint8_lz2 (minutes);
	lcd_putc (':');
	print_uint8_lz2 (seconds);
}

void clk_tick()
{
	seconds++;

	if (seconds == 60)
	{
		minutes++;
		seconds = 0;
	}

	if (minutes == 60)
	{
		hours++;
		minutes = 0;
	}

	if (hours == 24)
	{
		hours = 0;
		/* day change foo here */
	}
	

}

void clk_inc ()
{
	(*valptr) = (*valptr) +1;
	if (valptr == &ns || valptr == &nm)
	{
		if (*valptr == 60)
			*valptr = 0;
	} else if (*valptr == 24)
	{
		*valptr = 0;
	}

	lcd_gotoxy (12,0);
	print_uint8_lz (*valptr);
}
void clk_dec ()
{
	if (valptr == &ns || valptr == &nm)
	{
		if (*valptr == 0)
			*valptr = 60;
	} else if (*valptr == 0)
	{
		*valptr = 24;
	}
	(*valptr) = (*valptr) -1;
	lcd_gotoxy (12,0);
	print_uint8_lz (*valptr);
}

void clk_set ()
{
	valptr = NULL;
	input_hook (BTN_SELECT, clk_next);
	input_hook (BTN_UP,     clk_inc);
	input_hook (BTN_DOWN,   clk_dec);
	clk_next();
}

void clk_next()
{
	static uint8_t mystate = 0;
	lcd_clrscr ();
	lcd_gotoxy (0,0);
	switch (mystate)
	{
		case 0:
			lcd_puts ("Hour:");
			valptr = &nh;
			nh = hours;
		break;
		case 1:
			lcd_puts ("Minute:");
			valptr = &nm;
			nm = minutes;
		break;
		case 2:
			lcd_puts ("Second:");
			valptr = &ns;
			ns = seconds;
		break;
		case 3:
			lcd_puts ("Clock set.");
			cli(); /* make sure no interrupt disturbs us. */
			hours = nh;
			minutes = nm;
			seconds = ns;
			sei();
			menu_init(); /* reset hooks, display menu again */
			return;
		break;
		default:
			lcd_puts ("E: CSUNDEF");
			mystate = 0;
			return;
	}
	lcd_gotoxy (12,0);
	print_uint8_lz (*valptr);
	mystate++;
	return;
}
