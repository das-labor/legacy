#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

// #include "config.h"
// #include "rf.h"
#include "rfm12_config.h"
#include "rfm12.h"

volatile static uint8_t transmissions;
volatile static uint8_t txcode; /* array index for transmission code */
volatile static uint8_t tmp[2] = {0xff, 0xff};

#undef PREAMBLE
#define PREAMBLE 0xff

const uint32_t switchcodes[] = {
	0x00141151, /* B An */
	0x00510550, /*switch a on*/
	0x0015050d, /*A 1 on*/
	0x00510550 /*switch a on*/
};

void timer_init()
{
	TCCR0 |= (  _BV(CS01) ); /* clk/64 */
	TIMSK |= (_BV(TOIE0));
	TIFR |= (1<<TOV0);
}

#if 1
ISR (TIMER0_OVF_vect)
{
	static uint8_t pauseticks = 0;
	static uint32_t msk = 0x80000000;
	const uint8_t tmp[12] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
	
	if (!transmissions) return;
	if (pauseticks)
	{
		pauseticks--;
		return;
	}
	
	PORTD |= (_BV(PD5));

	if ((uint32_t) switchcodes[0] & (uint32_t) msk)
	{
		rfm12_tx (9, 0xaa, tmp);
		TCNT0 = 10;
	} else
	{
		rfm12_tx (1, 0xaa, tmp);
		TCNT0 = 40;
	}

	msk >>= 1;

	if (!msk)
	{
		msk = 0x80000000;
		transmissions--;
		pauseticks = 0;
	}
	rfm12_tick();

	PORTD &= ~(_BV(PD5));
}
#endif


int main(void)
{
	uint16_t i = 0;
	uint8_t tmp[2] = {0xaa, 0x55};
	rfm12_init();
	timer_init();

	DDRD |= (_BV(PD5) | _BV(PD6));
	DDRB |= (_BV(PB3));
	PORTB |= (_BV(PB3));
	//PORTD ^= _BV(PD5);
	
	transmissions = 10;
	txcode = 0;
	PORTD ^= _BV(PD6);
	_delay_ms(250);
	_delay_ms(250);
	sei();
	_delay_ms(250);
	_delay_ms(250);
	PORTD ^= _BV(PD6);

	rfm12_tick();

	while(1)
	{
	//	rfm12_tick();
		asm volatile ("nop");
		i++;
	}
};
