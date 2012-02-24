#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rfm12.h"
#include "motor.h"
#include "locodoco.h"

ISR (TIMER1_COMPB_vect)
{

}

int main (void)
{
	uint8_t tst[4] = {0x13, 0x37, 0x23, 0x42};
	uint32_t c;
	
	motor_init();
	rfm12_init();
//	locodoco_init();
	sei();
	MOTOR_ON();
	MOTOR_FW();
	
	while (42)
	{
		if ((c & 0xffff) == 0x00)
		{
			if (c & 0x10000)
			{
				MOTOR_RW();
			}
			else
			{
				MOTOR_FW();
			}
			rfm12_tx (4, 0xbb, tst);
		}
		c++;
		rfm12_tick();
		locodoco_tick();
	}
}
