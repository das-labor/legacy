#include <avr/io.h>

#include "rfm12.h"
#include "motor.h"
#include "locodoco.h"

int main (void)
{
	motor_init();
	rfm12_init();
	locodoco_init();
	
	while (42)
	{
		rfm12_tick();
	}
}
