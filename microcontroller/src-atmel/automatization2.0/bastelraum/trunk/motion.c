#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "io.h"
#include "motion.h"
#include "Bastelcmd.h"

static const uint8_t motion_admux[NUM_DETECTORS] =
{
	(0x00 | ADMUX_PREDEF),
	(0x01 | ADMUX_PREDEF)
};

/* motion tick counter for the current 60s
 * note: ticks from *all* motion detectors are commulated in this value.
 */
static volatile uint8_t motion_ticks = 0;

/* counter */
static volatile uint8_t hscount = 0;

/* current motion detector channel */
static volatile uint8_t mux_chan = 0;

/* number of idle periods counted */
static volatile uint8_t motion_idlecount = 0;

static void timer0_init(void)
{
	TCCR0B = (_BV(CS00) | _BV(CS02)); /* clk/1024 */
	TIMSK0 = _BV(TOIE0); /* overflow int. */
}

void motion_init()
{
	timer0_init();
}

static void adc_disable(void)
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
static void adc_init(void)
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
void motion_tick()
{
	static uint8_t warnperiod = 0;
	static uint8_t old_pwm[3];
	/* keine bewegung erkannt - warnung zuende */
	if (warnperiod && hscount > M_WARN_LENGTH)
	{
		set_pwm(F_PWM_FENSTER, old_pwm[0]);
		set_pwm(F_PWM_MITTE, old_pwm[1]);
		set_pwm(F_PWM_NISCHE, old_pwm[2]);
		warnperiod = 0;
	}
	/* warten bis 2 min um sind */
	if (hscount < 240) /* 240 * 0.5s -> 2 minutes */
		return;

	motion_idlecount++;
	/* idle zähler zurücksetzen falls es bewegung gab */
	if (motion_ticks >= MOTION_NUM_IGNORE)
	{
		motion_idlecount = 0;
	}
	/* abschalten falls idlecount gleich IDLE_TRESHOLD und OFF_TRESHOLD ist */
	if (motion_idlecount == M_IDLE_TRESHOLD + M_OFF_TRESHOLD)
	{
		set_output_all(0);
		motion_idlecount = M_IDLE_TRESHOLD + M_OFF_TRESHOLD + 1; /* anti-overflow... */
	}
	/* keine bewegung erkannt licht zur warnung abdimmen */
	else if (motion_idlecount == M_IDLE_TRESHOLD)
	{
		if (get_outputs())
		{
			old_pwm[0] = get_pwm(F_PWM_FENSTER);
			old_pwm[1] = get_pwm(F_PWM_MITTE);
			old_pwm[2] = get_pwm(F_PWM_NISCHE);
			set_pwm(F_PWM_FENSTER, 10);
			set_pwm(F_PWM_MITTE, 10);
			set_pwm(F_PWM_NISCHE, 10);
			warnperiod = 1;
		}
	}

	motion_ticks = 0;
	hscount = 0;
}

