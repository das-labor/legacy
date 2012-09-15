#include "motion.h"
const uint8_t motion_admux[NUM_DETECTORS] =
{
	(0x00 | ADMUX_PREDEF),
	(0x01 | ADMUX_PREDEF)
};

/* motion tick counter for the current 60s
 * note: ticks from *all* motion detectors are commulated in this value.
 */
volatile static uint8_t motion_ticks = 0;

/* counter */
volatile static uint8_t hscount = 0;

/* current motion detector channel */
volatile static uint8_t mux_chan = 0;

/* number of idle periods counted */
volatile static uint8_t motion_idlecount = 0;

void timer0_init ()
{
	TCCR0B = (_BV(CS00) | _BV(CS02)); /* clk/1024 */
	TIMSK0 = _BV(TOIE0); /* overflow int. */
}

void motion_init ()
{
	timer0_init();
}

void adc_disable()
{
	ADCSRA &= ~(_BV(ADEN));
}


ISR(ADC_vect)
{
	static uint16_t l8[NUM_DETECTORS]; /* sum of last 8 values */
	static uint16_t c8[NUM_DETECTORS]; /* sum of current 8 values */
	static uint8_t s = 0;              /* state counter */
	
	if (s == 0)
	{
		/* discard first measurement */
		s++;
		return;
	}

	if (s < 8)
	{
		c8[mux_chan] += ADC;
		s++;
		return;
	}

	/* ... after 8 samples ... */
	adc_disable();

	if (c8[mux_chan] + MOTION_TRESHOLD < l8[mux_chan] || c8[mux_chan] - MOTION_TRESHOLD > l8[mux_chan])
	{
	PORTB ^= _BV(PB0); /* DEBUG FOO XXX */
		if (motion_ticks < 0xff)
			motion_ticks++;
	}

	l8[mux_chan] = c8[mux_chan];
	c8[mux_chan] = 0;
	s = 0;
}

/* initialize the ADC, enable the interrupt, select mux channel
 */
void adc_init ()
{
	mux_chan++;
	if (mux_chan >= NUM_DETECTORS)
		mux_chan = 0;
	ADMUX = motion_admux[mux_chan];
	
	ADCSRB = 0x00; /* free running mode */

	/* enable, auto trigger (free running), int enable, start conv, pscaler 128 */
	ADCSRA = (_BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADSC) | 
		_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t c = 0;
	
	c++;
	if (c < 38) /* ~ 0.5s (76.3 overflows per second) */
		return;
	
	c = 0;

	/* enable the adc, start 8 sample runs */
	adc_init();
	hscount++;
}


/* handler to be executed from main() */
void motion_tick ()
{
//	static uint8_t sreg_old;
	static uint8_t warnperiod = 0;

	if (warnperiod && hscount > M_WARN_LENGTH)
	{
		/* restore old sreg state */
		sreg ^= 0xff;
		change_shift_reg (sreg);
		warnperiod = 0;
	}

	if (hscount < 240) /* 240 * 0.5s -> 2 minutes */
		return;
	
	motion_idlecount++;
	if (motion_ticks >= MOTION_NUM_IGNORE)
	{
		motion_idlecount = 0;
	}

	if (motion_idlecount == M_IDLE_TRESHOLD + M_OFF_TRESHOLD)
	{
		sreg = 0;
		change_shift_reg (sreg);
		send_status();
		motion_idlecount = M_IDLE_TRESHOLD + M_OFF_TRESHOLD + 1; /* anti-overflow... */
	} else if (motion_idlecount == M_IDLE_TRESHOLD)
	{
		sreg ^= 0xff;
		change_shift_reg (sreg);
		warnperiod = 1;
	}


	motion_ticks = 0;
	hscount = 0;
}


#if 0
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
	//static can_message msg = {0, 0, 0x13, 0x37, 1, {0x9F}};

	if (!motion_get())
		return;
	
	//can_transmit (&msg);
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
#endif
