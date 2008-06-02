#include <stdint.h>
#include <avr/io.h>
#include "../door.h"

int main (void)
{
	uint16_t i = 0;

	mctl_init();
	mctl_ctl (MCTL_CLOSE_DOOR);
	while (42)
	{
		mctl_tick();
		
		for (i=0;i<12000;i++)
			asm volatile ("nop");
	}
}
