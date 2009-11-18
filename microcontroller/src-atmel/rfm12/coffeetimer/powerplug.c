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
#define TNUM_MAX 12

/* 255-$time to send a logical 1 for ones and zeroes */
#define TS 130 /* short period */
#define TL 5   /* long period */

void pp_init()
{
	switchcode = 0;
	tnum = 0;
	
	TCCR2 |= ( _BV(CS22) ); /* clk/64 */
	TIMSK |= (_BV(TOIE2));
	TIFR |= (_BV(TOV2));
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
					rfm12_tx_off();
					TCNT2 = TS;
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
		TCNT2 = TL;
		mystate ^= 0x01; /* delay msk shifting a little longer */
	}
	
	if (!mystate)
		msk >>= 1;

	if (!msk) /* transmission of code block is over */
	{
		tnum++;
		mystate = 0;
	}


}

inline void pp_tick_d2 (void)
{
	static uint8_t sendmsk = 0x80, sendtriple = 0;
	static uint32_t msk = 0x80000000;

	if (tnum > 12)
		return;

	rfm12_ask_tx_mode(1);
	rfm12_tx_off();
	TCNT2 = 123; /* with timer set to clk/64, the next overflow will happen in approx. 0.000516s */

	if (sendmsk == 0x80 && msk > 0x80) /* handle first/next triple */
	{
		if (msk & switchcode)
			sendtriple = 0xC0; /* 110... */
		else
			sendtriple = 0x80; /* 100... */
	} else
	{
		sendtriple = 0x00;
	}
	
	if (sendmsk & sendtriple)
	{
		rfm12_tx_on();  /* send logical 1 */
	} else
	{
		rfm12_tx_off(); /* send logical 0 */
	}

	sendmsk >>= 1;
		
	if (sendmsk <= 0x10)
	{
		msk >>= 1;
		sendmsk = 0x80;
		
		/* last 1 */
		if (msk == 0x80)
		{
			rfm12_tx_on();
			sendmsk = 0x20;
		} else if (msk && msk <= 0x40)
		{
			rfm12_tx_off();
		} else if (msk == 0x00)
		{
			tnum++;
			msk = 0x80000000;
			if (tnum >= 12)
			{
				rfm12_tx_off();
				rfm12_ask_tx_mode(0);
				lcd_clrscr();
				lcd_puts ("all sent.");
				return;
			}
		}

	}
}


void pp_tick_d (void)
{
	static uint8_t pauseticks = 0, ontime = 0;
	static uint32_t msk = 0x80000000;


	if (tnum > 4)
	{
		rfm12_ask_tx_mode(0);
		return;
	}
//	rfm12_tx (6, 0, "foobar");
	rfm12_ask_tx_mode(1);
	lcd_clrscr();
	lcd_puts ("sending");
	lcd_gotoxy (0,1);
	lcd_puts ("t");
	print_uint8_lz (tnum);


	if (0) /* wtf? */
	{
		tnum++;
		pauseticks = 40;
		rfm12_tx_off();
		return;
	}

	TCNT2 = 155;
	if (pauseticks)
	{	
		/* short high */
		if ((pauseticks < ontime))
		{
			rfm12_ask_tx_mode(0);
			rfm12_tx_off();
		}
		pauseticks--;
		return;
	}
	
	/* long tx */
	if ((uint32_t) switchcode & (uint32_t) msk)
	{
		ontime = 4;
	} else  /* short period */
	{
		ontime = 9;
	}
	pauseticks = 10;
	rfm12_ask_tx_mode(1);
	rfm12_tx_on();

	msk >>= 1;

	if (msk == 0x20)
	{
		msk = 0x80000000;
		pauseticks = 40;
		rfm12_tx_off();
		rfm12_ask_tx_mode(0);
		tnum++;
	}
}

ISR (TIMER2_OVF_vect)
{
	pp_tick();
	return;
}
