/*
 * PC0 = schalter
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

// #include "config.h"
// #include "rf.h"
#include "rfm12_config.h"
#include "rfm12.h"

#define DEFAULT_TRANS 35

volatile static uint8_t transmissions;
volatile static uint8_t txcode; /* array index for transmission code */
volatile static uint8_t tmp[2] = {0xff, 0xff};

#undef PREAMBLE
#define PREAMBLE 0xff

const uint32_t switchcodes[6][2] = {
	{ 0x41115400, 0x41115100 }, /* stehlampe */
	{ 0x41154400, 0x41154100 }, /* schreibtisch */
	{ 0x41151400, 0x41151100 }, /* drucker */
	{ 0x41145400, 0x41145100 }, /* musi */
	{ 0x45055400, 0x45055100 }, /* gateway */ 
	{ 0x00, 0x00 }
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
	static uint8_t pauseticks = 0, ontime = 0, tnum = 0, oldstat, swstate;
	static uint32_t msk = 0x80000000;
	const uint8_t tmp[12] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

	if (oldstat != (PINC & _BV(PC0)))
	{
		swstate = (PINC & _BV(PC0)) ? 0 : 1;
		transmissions = DEFAULT_TRANS;
		tnum = 0;
	}
	
	oldstat = (PINC & _BV(PC0));

	if (tnum > 4) return;
	if (!transmissions)
	{
		tnum++;
		transmissions = DEFAULT_TRANS;
		pauseticks = 40;
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
	if ((uint32_t) switchcodes[tnum][1] & (uint32_t) msk)
	{
//		rfm12_tx (10, 0xaa, tmp);
//		TCNT0 = 3;
		ontime = 4;
	} else
	{
	/* short period */
//		rfm12_tx (1, 0xaa, tmp);
//		TCNT0 = 9;
		ontime = 9;
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

	DDRD |= (_BV(PD5) | _BV(PD6)) | _BV(PD7);
	PORTD |= _BV(PD7);
	
	DDRB |= (_BV(PB3)) | _BV(PB2);
	PORTB |= (_BV(PB3)) | _BV(PB2);

	PORTC |= _BV(PC0); /* pullup for switch */

	//PORTD ^= _BV(PD5);
	
	rfm12_rawmode (1);
	transmissions = DEFAULT_TRANS;
	txcode = 0;
	PORTD ^= _BV(PD6);
	_delay_ms(250);
	_delay_ms(250);
	sei();
	_delay_ms(250);
	_delay_ms(250);
	timer_init();
	PORTD ^= _BV(PD6);


	while(1)
	{
//		if (i & 0x200) rfm12_tick();
		asm volatile ("nop");
		i++;
	}
};
