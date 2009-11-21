#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "rfm12_config.h"
#include "rfm12.h"
#include "powerplug.h"
#include "lcd.h"
#include "numprint.h"

static uint8_t tnum;
static uint32_t switchcode;

/* max. amount of transmissions */
#define TNUM_MAX 120

/* 255-$time to send a logical 1 for ones and zeroes */
#define TS 205 /* short period */
#define TL 155 /* long period */
#define TP 130 /* pause period */

void pp_init()
{
	switchcode = 0;
	tnum = 0;
	
	TCCR2 |= ( _BV(CS22) | _BV(CS20) ); /* clk/128 */
	TIMSK |= (_BV(TOIE2));
}

void pp_send (uint8_t in_onoff, uint32_t in_code)
{
	uint32_t out_code;
	out_code = in_code;

	if (in_onoff)
	{
		out_code |= 0x00000100;
	} else
	{
		out_code |= 0x00000400;
	}
	switchcode = out_code;
	tnum = 0;
}

inline void pp_tick ()
{
	static uint32_t msk = 0x80000000;
	static uint8_t  mystate = 0, init_done = 0;

	if (tnum > TNUM_MAX) return;
	
	if (!init_done)
	{
		rfm12_init();
		rfm12_ask_tx_mode(1);
		rfm12_tx_off();
		mystate = 0;
		msk = 0x80000000;
		init_done = 1;
	}

	if (tnum == TNUM_MAX)
	{
		rfm12_tx_off();
		rfm12_ask_tx_mode(0);
		tnum++; /* disable further transmissions */
		init_done = 0; /* prepare for next code */
		return;
	}

	if (msk > 0xff)
	{
		switch (mystate)
		{
			case 0:
				if (msk & switchcode) /* send 11 */
				{
					TCNT2 = TL;
					rfm12_tx_on();
				} else                /* send 1 */
				{
					TCNT2 = TS;
					rfm12_tx_on();
				}
				mystate = 1;
			break;
			case 1:
				if (msk & switchcode) /* send 0 */
				{
					TCNT2 = TS;
					rfm12_tx_off();
				} else                /* send 00 */
				{
					TCNT2 = TL;
					rfm12_tx_off();
				}
				mystate = 0;
			break;
		}
	} else if (msk == 0x80) /* send trailing 1 */
	{
		rfm12_tx_on();
		TCNT2 = TS;
		mystate = 0;
	} else /* pause between two code blocks (for 14 long cycles) */
	{	rfm12_tx_off();
		TCNT2 = TP;
		mystate ^= 0x01; /* delay msk shifting a little longer */
	}
	
	if (!mystate)
		msk >>= 1;

	if (!msk) /* transmission of code block is over */
	{
		tnum++;
		mystate = 0;
		msk = 0x80000000;
	}


}

ISR (TIMER2_OVF_vect)
{
	cli();
	pp_tick();
	sei();
	return;
}
