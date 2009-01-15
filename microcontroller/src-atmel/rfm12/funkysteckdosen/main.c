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
	0x14115100,
	0x00510550, /*switch a on*/
	0x0015050d, /*A 1 on*/
	0x00510550, /*switch a on*/
	0xAAAAAAAA  /* testcode */
};

// 141151+0
void timer_init()
{
	TCCR0 |= (  _BV(CS01) ); /* clk/64 */
	TIMSK |= (_BV(TOIE0));
	TIFR |= (1<<TOV0);
}

#if 1
ISR (TIMER0_OVF_vect)
{
	static uint8_t pauseticks = 0, ontime = 0;
	static uint32_t msk = 0x80000000;
	const uint8_t tmp[12] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

	if (!transmissions)
	{
		rfm12_tx_off();
		return;
	}

	if (pauseticks)
	{	
		/* short high */
//		if ((sendtype == 0 && pauseticks < 9)  /* short period */
//			|| (sendtype == 1 && pauseticks < 8)) /* long period */
		if ((pauseticks < ontime))
		{
			rfm12_tx_off();
			PORTD &= ~(_BV(PD5));
		
		} else
		{
		//	rfm12_tx_on();
		}
		pauseticks--;
		return;
	}
	
	PORTD |= (_BV(PD5));
	
	/* long tx */
	if ((uint32_t) switchcodes[1] & (uint32_t) msk)
	{
//		rfm12_tx (10, 0xaa, tmp);
//		TCNT0 = 3;
		ontime = 4;
	} else
	{
	/* short period */
//		rfm12_tx (1, 0xaa, tmp);
//		TCNT0 = 9;
		ontime = 10;
	}
	pauseticks = 10;
	rfm12_tx_on();
	PORTD |= (_BV(PD5));
	TCNT0 = 5;

	msk >>= 1;

	if (msk == 0x20)
	{
		msk = 0x80000000;
		transmissions--;
		pauseticks = 20;
		rfm12_tx_off();
	}
//	rfm12_tick();

}
#endif


int main(void)
{
	uint16_t i = 0;
	uint8_t tmp[5] = {0xaa, 0x55, 0xff, 0xf9, 0x9f};
	rfm12_init();
	timer_init();

	DDRD |= (_BV(PD5) | _BV(PD6)) | _BV(PD7);
	PORTD |= _BV(PD7);
	
	DDRB |= (_BV(PB3)) | _BV(PB2);
	PORTB |= (_BV(PB3)) | _BV(PB2);

	//PORTD ^= _BV(PD5);
	
	rfm12_rawmode (1);
	transmissions = 40;
	txcode = 0;
	PORTD ^= _BV(PD6);
	_delay_ms(250);
	_delay_ms(250);
	sei();
	_delay_ms(250);
	_delay_ms(250);
	PORTD ^= _BV(PD6);


	while(1)
	{
//		if (i & 0x200) rfm12_tick();
		asm volatile ("nop");
		i++;
	}
};
