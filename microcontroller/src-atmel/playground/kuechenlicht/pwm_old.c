#include "pwm.h"

volatile static uint8_t col_c[3], col_d[3];

inline void init_timer1(void)
{
	/* no prescaler */
	TCCR1B = _BV(CS10) | _BV(WGM12);
	//TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);
	TCCR1A = 0; 
#ifdef _ATMEGA88
	TCCR1C = 0;
#endif

	/* enable timer1 overflow (=output compare 1a)
	* and output compare 1b interrupt */
	TIMSK |= _BV(OCIE1A);

	/* set value for start of new cycle*/
	//a little larger, so this happens just after the last possible regular int
	OCR1A = 20;

	/* load initial delay, trigger an overflow */
	OCR1B = 65000;
}


void pwm_init()
{
	init_timer1();
}



void fadeto (uint8_t r, uint8_t g, uint8_t b)
{
	col_d[0] = r;
	col_d[1] = g;
	col_d[2] = b;
}

void fade_tick()
{
	static uint8_t substep;
	uint8_t i;
	substep++;
	
	if (col_c[0] > substep)
		RED_ON();
	else
		RED_OFF();

	if (col_c[1] > substep)
		GREEN_ON();
	else
		GREEN_OFF();

	if (col_c[2] > substep)
		BLUE_ON();
	else
		BLUE_OFF();

	if (substep == 0xff)
	{
		for (i=0;i<3;i++)
		{
			if (col_d[i] < col_c[i])
				col_c[i]--;

			if (col_d[i] > col_c[i])
				col_c[i]++;
		}
	}
}

ISR(SIG_OUTPUT_COMPARE1A)
{
	fade_tick();
}
