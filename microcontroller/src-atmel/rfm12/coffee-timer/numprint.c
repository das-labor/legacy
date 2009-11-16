#include <stdint.h>
#include "lcd.h"

void print_uint8_lz (uint8_t in_int)
{
	lcd_putc ('0' + (in_int / 100));
	in_int -= (in_int / 100) * 100;
	lcd_putc ('0' + (in_int / 10));
	in_int -= (in_int / 10) * 10;
	lcd_putc ('0' + in_int);
}

void print_uint8_lz2 (uint8_t in_int)
{
	in_int -= (in_int / 100) * 100;
	lcd_putc ('0' + (in_int / 10));
	in_int -= (in_int / 10) * 10;
	lcd_putc ('0' + in_int);
}
