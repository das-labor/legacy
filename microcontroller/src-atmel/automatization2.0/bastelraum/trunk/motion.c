#include "motion.h"

static volatile uint8_t motion_tresh = 128;

/* motion condition */
volatile static uint8_t motion_cond = 0;

void motion_init ()
{
	/* int. 2.56V reference, muxer to channel 0, prescaler /64, free running mode */
	ADMUX = _BV(REFS0) | _BV(REFS1);
	/* adc init, int enable, start int, prescaler to 64 */
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADATE);
	ADCSRB = 0x00;

	motion_tresh = eeprom_read_byte ((uint8_t *) EEP_MOTION_TRESH);
}

/* motion cond. polling, condition is cleared once it is read */
uint8_t motion_get ()
{
	if (!motion_cond)
		return 0;
	
	motion_cond = 0;
	return 1;
}

void motion_tick ()
{
	static can_message msg = {0, 0, 0x13, 0x37, 1, {0x9F}};

	if (!motion_get())
		return;
	
	can_transmit (&msg);
}

ISR (ADC_vect)
{
	static uint8_t mcount = 0;
	static uint8_t chan = 1;
	static uint32_t meanv[2] = {0x00, 0x00};
	int32_t delta;
	uint16_t aval;

	aval = ADC;
	mcount++;
	if (mcount <= 1)
	{
		/* discard result after channel switch / result from last active chan. */
		return;
	}
	
	delta = ((uint32_t) aval * 65536) - meanv[chan];
	meanv[chan] += (delta / 1024);
	
	
	if (mcount == 4)
	{
		if (chan)
		{
			ADMUX |= _BV(MUX0);
		} else
		{
			ADMUX &= ~(_BV(MUX0));
		}
	}

	if (mcount == 5)
	{
		/* one conversion after the channel switch: current result is still
		 * the one for the previous channel
		 */
		chan ^= 0x01;
		mcount = 0;
	}

	motion_tresh = 0xf0; // XXX

	if (delta > - motion_tresh && delta < motion_tresh)
		return;
	
	motion_cond = 1;
}
