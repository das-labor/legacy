// #include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "menu.h"
#include "cron.h"

#include "rfm12_config.h"
#include "rfm12.h"

#include "lcd.h"
#include "input.h"
#include "numprint.h"
#include "clock.h"
#include "coffee.h"
#include "powerplug.h"

#define RESET_TIMEOUT 15
#define VER_MAJOR 0
#define VER_MINOR 1

static uint8_t timeout;
static inline void reset_timeout ();

ISR (TIMER0_OVF_vect)
{
	static uint8_t cyclecount = 0, oddeven = 0;

	cyclecount++;

	rot_poll();
	if (cyclecount < 63) return;

	cyclecount -= 63;

	if (oddeven & 0x01)
		TCNT0 = 133;
	else
		TCNT0 = 132;

	oddeven++;
	cron_tick();
	clk_tick();
	
	if (timeout)
	{
		timeout--;
	} else /* timeout hit, enqueue clock program */
	{
		clk_print();
	}
}


int main (void)
{
	uint8_t i = 0;
	
	TCCR0 |= ( _BV(CS00) | _BV(CS02) ); /* clk/1024 */
	TIMSK |= _BV(TOIE0);

	lcd_init (LCD_DISP_ON);
	rfm12_init();
	cron_init();
	input_init();
	input_hook (BTN_ANY, reset_timeout);

	lcd_clrscr();
	lcd_puts("CoffeeTimer");
	lcd_gotoxy (0,1);
	lcd_puts ("Version ");
	print_uint8_lz (VER_MAJOR);
	lcd_putc('.');
	print_uint8_lz (VER_MINOR);


	sei();
	
	for (i=0;i<6;i++)
		_delay_ms(250);

	pp_init();
	
	timeout = RESET_TIMEOUT;

	menu_init();
	send_c_on(0);
	_delay_ms(250);
	send_c_off(0);

	while (42)
	{
		task_queue (0, 0); /* execute tasks in the queue */
		rfm12_tick();
	}
}

static inline void reset_timeout ()
{
	timeout = RESET_TIMEOUT;
}
