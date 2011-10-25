#include <avr/io.h>

#include "rfm12_config.h"
#include "rfm12.h"
#include "motor.h"


int main (void)
{
	motor_init();
	rfm12_init();
	
	while (42)
	{
		rfm12_tick();
	}
}
