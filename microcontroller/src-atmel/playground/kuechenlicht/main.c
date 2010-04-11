#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "config.h"
#include "pwm.h"

enum lightstates {off,on,fade_rgb,fade_off,fade_on};
volatile static enum lightstates lstate;
static volatile uint8_t debounce = 0;
uint16_t filter (uint16_t in_val);

void timer0_init();
void init(void)
{
	COLORS_INIT();
	ACSR = _BV(ACD); /* adc off */
	pwm_init();
	timer0_init();
	lstate = off;
	sei();
}

uint16_t sensor_read ()
{
	/* PC2 = Sensor plate, PC3 = Cap */
	uint16_t i;
	register uint16_t count = 0;

	/* GND both */
	DDRC |= (_BV(PC2) | _BV(PC3));
	PORTC &= ~(_BV(PC2) | _BV(PC3));

	/* wait f. discharge */
	for (i=0;i<0xffff;i++)
		__asm volatile ("nop");

	DDRC &= ~(_BV(PC2)); /* set plate to input */
	PORTC &= ~(_BV(PC2));

	do
	{
		count++;

		DDRC |= _BV(PC3);    /* cap to output */
		PORTC |= _BV(PC3);   /* charge cap */

		for (i=0;i<10;i++)
			__asm volatile ("nop");

		DDRC &= ~(_BV(PC3)); /* cap to input */
		DDRC |= _BV(PC2);    /* plate to output */
		PORTC &= ~(_BV(PC2)); /* plate low */
		
//		for (i=0;i<1;i++)
//			__asm volatile ("nop");
		DDRC &= ~(_BV(PC2) | _BV(PC3));
		PORTC &= ~(_BV(PC3));

	} while (!(PINC & _BV(PC3)));

	return count;

}

uint16_t sensor_read_onecap()
{
	register uint16_t counter = 0;
	uint16_t i;
	

	/* set to input */
	DDRC |= (_BV(PC2));
	PORTC &= ~(_BV(PC2)); /* output on */
	for (i=0;i<0x7ff;i++)
		__asm volatile ("nop");

	/* charge (pullup on) */
//	PORTC |= _BV(PC2);
#if 0	
	for (i=0;i<0x4;i++)
	{
		__asm volatile ("nop");
		__asm volatile ("nop");
	}
	
	
#endif
	DDRC &= ~(_BV(PC2));
	PORTC &= ~(_BV(PC2)); /* output on */
	
	while (!(PINC & _BV(PC2)))
	{
		counter++;
	}
	/* discharge */
	DDRC |= _BV(PC2);
	PORTC &= ~(_BV(PC2));
	return counter;
}

void timer0_init()
{
	TCCR0 = (_BV(CS02) | _BV(CS00));
	TIMSK |= _BV(TOIE0);
}

ISR(TIMER0_OVF_vect) /* fade to a different color approx. every 4 seconds */
{
	static uint8_t a=0, fidx=0;
	sei();

	const uint8_t f[8][3] =
	{
		{255,  0,  0},
		{255,255,  0},
		{0  ,255,  0},
		{0,  255,255},
		{0  ,  0,255},
		{255,  0,255},
		{255,255,255},
		{255,120,  0}
	};
	
	if (debounce)
		debounce--;
	
	if (lstate != fade_rgb)
		return;

	a++;
	if (a) return;

	fidx++;
	fidx %= 8;
	fadeto(f[fidx][0],f[fidx][1],f[fidx][2],255);
}

uint16_t filter (uint16_t in_val)
{
	static int32_t fval;
	static uint8_t init_done = 0;
	int32_t delta;

	if (!init_done)
	{
		fval = in_val * 65536;
		init_done = 1;
		return 0;
	}
	delta = ((in_val * 65536) - fval);
	fval += delta / 1024;
	return abs(delta / 65536);
}

int main(void)
{
	uint8_t onoff = 0;
	uint16_t delta, touchcount = 0;

	DDRC |= _BV(PC2);
	PORTC &= ~(_BV(PC2));
	_delay_ms(250);

	init();
	while (23)
	{
		delta = filter(sensor_read_onecap());

		if (delta > 15)
		{
			touchcount++;
		} else
		{
			touchcount = 0;
		}

		if (touchcount > 10)
		{
			touchcount = 0;
			
			if (debounce)
				continue;

			debounce = 60; /* approx. 1s */

			switch (lstate)
			{
				case on:
					lstate = fade_rgb;
					WHITE_OFF();
				break;
				case off:
					fadeto(120,120,0,50);
					WHITE_ON();
					lstate = on;
				break;
				case fade_rgb:
					lstate = off;
					fadeto(0,0,0, 100);
				break;
			}
			
		}
	}
}

