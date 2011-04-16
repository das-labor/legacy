#include "lights.h"
#include <avr/io.h>

void lights_init()
{
	DDRC |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	PORTC |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3); /* all on */
}
void lights_set_raw (uint8_t in_bits)
{
	PORTC &= (in_bits | 0xF0);
	PORTC |= (in_bits & 0x0F);
}
