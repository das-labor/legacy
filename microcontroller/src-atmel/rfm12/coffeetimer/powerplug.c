#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "rfm12_config.h"
#include "rfm12.h"
#include "powerplug.h"

static uint8_t tnum;
static uint32_t switchcode;

void pp_init()
{
	switchcode = 0;
	tnum = 0;
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
}

void pp_tick (void)
{
	static uint8_t pauseticks = 0, ontime = 0, tnum = 0, swstate;
	static uint32_t msk = 0x80000000;


	if (tnum > 4) return;
	rfm12_ask_tx_mode(1);

	if (0) /* wtf? */
	{
		tnum++;
		pauseticks = 40;
		rfm12_tx_off();
		return;
	}

	if (pauseticks)
	{	
		/* short high */
		if ((pauseticks < ontime))
		{
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
	rfm12_tx_on();
	TCNT2 = 5;

	msk >>= 1;

	if (msk == 0x20)
	{
		msk = 0x80000000;
		pauseticks = 20;
		rfm12_tx_off();
	}
	rfm12_ask_tx_mode(0);
}
