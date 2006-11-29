#include "lcd.h"
#include <inttypes.h>

void lcd_wait(int wait)
{
	volatile int i;

	for(i=wait; i>0; i++);
}

void lcd_init()
{
	lcd_wait( 15000 );
	lcd_nibble( 1, 0x3 );
	lcd_wait( 4100 );
	lcd_nibble( 1, 0x32 );
}

