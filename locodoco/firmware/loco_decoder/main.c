#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "adc.h"

int main (void)
{
	motor_init();

	while (23)
	{
		motor_tick();
	}
}
