#include "lights.h"
#include <avr/io.h>

void lights_init()
{
	DDRC |= 0x0F;
	PORTC |= 0x0F; /* all on */
}
void lights_set_raw (uint8_t in_bits)
{
	PORTC &= (in_bits | 0xF0);
	PORTC |= (in_bits & 0x0F);
}
