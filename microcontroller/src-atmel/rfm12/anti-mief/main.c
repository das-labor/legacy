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
#include "fan.h"
#include "input.h"
#include "numprint.h"

#define RESET_TIMEOUT 15

static uint8_t seconds, hours, minutes, timeout;
void run_clk ();
void reset_timeout ();

ISR (TIMER0_OVF_vect)
{
	static uint8_t cyclecount = 0;

	cyclecount++;
//	if (!(cyclecount & 0x03))

	if (cyclecount < 65) return;

	cyclecount = 0;

	if (seconds & 0x01)
		TCNT0 = 123;
	else
		TCNT0 = 124;

	seconds++;
	cron_tick();

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
	
	if (timeout)
	{
		timeout--;
	} else /* timeout hit, enqueue clock program */
	{
		run_clk();
	}

}

void run_clk ()
{
	lcd_clrscr();
	
	lcd_gotoxy(3,0);
	print_uint8_lz2 (hours);
	lcd_putc (':');
	print_uint8_lz2 (minutes);
	lcd_putc (':');
	print_uint8_lz2 (seconds);
	

}

int main (void)
{
	uint8_t i = 0;
	
	TCCR0 |= ( _BV(CS00) | _BV(CS02) ); /* clk/1024 */
	TIMSK |= _BV(TOIE0);
	sei();


	cron_init();
	lcd_init (LCD_DISP_ON);
	input_init();
	input_hook (BTN_ANY, reset_timeout);

	lcd_clrscr();
	lcd_puts("Anti-Mief v. 0.1\n");

	
	for (i=0;i<4;i++)
		_delay_ms(250);
	
	timeout = RESET_TIMEOUT;

	menu_init();
	menu_display();	

	while (42)
	{
		asm volatile ("nop");
		rot_poll();
		_delay_ms(1);
	}
}

void reset_timeout ()
{
	timeout = RESET_TIMEOUT;
}
