/*
 * {{{
 *    uses parts of fnordlicht firmware next generation
 *
 *    for additional information please
 *    see http://koeln.ccc.de/prozesse/running/fnordlicht
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 *     Lars Noschinski <lars@public.noschinski.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 }}} */

#include <avr/io.h>

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "pwm.h"
#include "static_scripts.h"

//defines for the Portpins the LEDs are connected to.
#define PORT_LED PORTC
#define DDR_LED DDRC

//the offset is the bitnumber of the Red Channel on the Port.
//the other color shall follow as RGB
#define LED_OFFSET 0
#define MASK_LEDS (0x07<<LED_OFFSET);


/* structs */
volatile struct global_t global = {{0, 0}};

/* prototypes */


/* TYPES AND PROTOTYPES */

/* encapsulates all pwm data including timeslot and output mask array */
struct timeslots_t {
	struct {
		uint8_t mask;
		uint16_t top;
	} slots[PWM_MAX_TIMESLOTS];

	uint8_t index;  /* current timeslot intex in the 'slots' array */
	uint8_t count;  /* number of entries in slots */
	uint8_t next_bitmask; /* next output bitmask, or signal for start or middle of pwm cycle */
	uint8_t initial_bitmask; /* output mask set at beginning */
};

static inline void prepare_next_timeslot(void);

/* GLOBAL VARIABLES */

/* timer top values for 256 brightness levels (stored in flash) */
static const uint16_t timeslot_table[] PROGMEM = {
      2,     8,    18,    31,    49,    71,    96,   126,
    159,   197,   238,   283,   333,   386,   443,   504,
    569,   638,   711,   787,   868,   953,  1041,  1134,
   1230,  1331,  1435,  1543,  1655,  1772,  1892,  2016,
   2144,  2276,  2411,  2551,  2695,  2842,  2994,  3150,
   3309,  3472,  3640,  3811,  3986,  4165,  4348,  4535,
   4726,  4921,  5120,  5323,  5529,  5740,  5955,  6173,
   6396,  6622,  6852,  7087,  7325,  7567,  7813,  8063,
   8317,  8575,  8836,  9102,  9372,  9646,  9923, 10205,
  10490, 10779, 11073, 11370, 11671, 11976, 12285, 12598,
  12915, 13236, 13561, 13890, 14222, 14559, 14899, 15244,
  15592, 15945, 16301, 16661, 17025, 17393, 17765, 18141,
  18521, 18905, 19293, 19685, 20080, 20480, 20884, 21291,
  21702, 22118, 22537, 22960, 23387, 23819, 24254, 24693,
  25135, 25582, 26033, 26488, 26946, 27409, 27876, 28346,
  28820, 29299, 29781, 30267, 30757, 31251, 31750, 32251,
  32757, 33267, 33781, 34299, 34820, 35346, 35875, 36409,
  36946, 37488, 38033, 38582, 39135, 39692, 40253, 40818,
  41387, 41960, 42537, 43117, 43702, 44291, 44883, 45480,
  46080, 46684, 47293, 47905, 48521, 49141, 49765, 50393,
  51025, 51661, 52300, 52944, 53592, 54243, 54899, 55558,
  56222, 56889, 57560, 58235, 58914, 59598, 60285, 60975,
  61670, 62369, 63072, 63779,   489,  1204,  1922,  2645,
   3371,  4101,  4836,  5574,  6316,  7062,  7812,  8566,
   9324, 10085, 10851, 11621, 12394, 13172, 13954, 14739,
  15528, 16322, 17119, 17920, 18725, 19534, 20347, 21164,
  21985, 22810, 23638, 24471, 25308, 26148, 26993, 27841,
  28693, 29550, 30410, 31274, 32142, 33014, 33890, 34770,
  35654, 36542, 37433, 38329, 39229, 40132, 41040, 41951,
  42866, 43786, 44709, 45636, 46567, 47502, 48441, 49384,
  50331, 51282, 52236, 53195, 54158, 55124, 56095, 57069,
  58047, 59030, 60016, 61006, 62000, 62998 };

/* pwm timeslots (the top values and masks for the timer1 interrupt) */
static struct timeslots_t pwm;
volatile struct global_pwm_t global_pwm;

/* FUNCTIONS AND INTERRUPTS */

/** init timer 1 */
inline void init_timer1(void) {
	/* no prescaler, CTC mode */
	TCCR1B = _BV(CS10) | _BV(WGM12);
	//TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);
	TCCR1A = 0;
#ifdef _ATMEGA88
	TCCR1C = 0;
#endif

	/* enable timer1 overflow (=output compare 1a)
	* and output compare 1b interrupt */
	_TIMSK_TIMER1 |= _BV(OCIE1A) | _BV(OCIE1B);

	/* set TOP for CTC mode */
	OCR1A = 64000;

	/* load initial delay, trigger an overflow */
	OCR1B = 65000;
}



/** init pwm */
void init_pwm(void) {
	uint8_t i;

	PORT_LED &= ~MASK_LEDS;
	DDR_LED |= MASK_LEDS;

	init_timer1();

	for (i=0; i<3; i++) {
		global_pwm.channels[i].brightness = 0;
		global_pwm.channels[i].target_brightness = 0;
		global_pwm.channels[i].speed = 0x00200;
		global_pwm.channels[i].flags.target_reached = 0;
		global_pwm.channels[i].remainder = 0;
		global_pwm.channels[i].mask = _BV(i+LED_OFFSET);
	}

	update_pwm_timeslots();
}

/** update pwm timeslot table */
void update_pwm_timeslots(void) {
	uint8_t sorted[PWM_CHANNELS] = { 0, 1, 2 };
	uint8_t i, j;
	uint8_t mask = 0;
	uint8_t last_brightness = 0;

	/* sort channels according to the current brightness */
	for (i=0; i<PWM_CHANNELS; i++) {
		for (j=i+1; j<PWM_CHANNELS; j++) {
			if (global_pwm.channels[sorted[j]].brightness < global_pwm.channels[sorted[i]].brightness) {
				uint8_t temp;

 				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
		}
	}

	/* timeslot index */
	j = 0;

	/* calculate timeslots and masks */
	for (i=0; i < PWM_CHANNELS; i++) {

		/* check if a timeslot is needed */
		if (global_pwm.channels[sorted[i]].brightness > 0 && global_pwm.channels[sorted[i]].brightness < 255) {
			/* if the next timeslot will be after the middle of the pwm cycle, insert the middle interrupt */
			if (last_brightness < 181 && global_pwm.channels[sorted[i]].brightness >= 181) {
				/* middle interrupt: top 65k and mask 0xff */
				pwm.slots[j].top = 65000;
				pwm.slots[j].mask = 0xff;
				j++;
			}

			/* insert new timeslot if brightness is new */
			if (global_pwm.channels[sorted[i]].brightness > last_brightness) {

				/* remember mask and brightness for next timeslot */
				mask |= global_pwm.channels[sorted[i]].mask;
				last_brightness = global_pwm.channels[sorted[i]].brightness;

				/* allocate new timeslot */
				pwm.slots[j].top = pgm_read_word(&timeslot_table[global_pwm.channels[sorted[i]].brightness - 1 ]);
				pwm.slots[j].mask = mask;
				j++;
			}
			else {
				/* change mask of last-inserted timeslot */
				mask |= global_pwm.channels[sorted[i]].mask;
				pwm.slots[j-1].mask = mask;
			}
		}
	}

	/* if all interrupts happen before the middle interrupt, insert it here */
	if (last_brightness < 181) {
		/* middle interrupt: top 65k and mask 0xff */
		pwm.slots[j].top = 65000;
		pwm.slots[j].mask = 0xff;
 		j++;
	}

	/* reset pwm structure */
	pwm.index = 0;
	pwm.count = j;

	/* next interrupt is the first in a cycle, so set the bitmask to 0 */
	pwm.next_bitmask = 0;

	/* calculate initial bitmask */
	pwm.initial_bitmask = 0xff;
	for (i=0; i < PWM_CHANNELS; i++) {
		if (global_pwm.channels[i].brightness > 0) {
			pwm.initial_bitmask &= ~global_pwm.channels[i].mask;
		}
	}
}

/** fade any channels not already at their target brightness */
void update_brightness(void) {
	uint8_t i;

	/* iterate over the channels */
	for (i=0; i<PWM_CHANNELS; i++) {
		uint8_t old_brightness;

		/* fade channel if not already at target brightness, set flag if target reached */
		if (global_pwm.channels[i].brightness != global_pwm.channels[i].target_brightness) {
			/* safe brightness, for later compare with calculated value */
			old_brightness = global_pwm.channels[i].brightness;

			/* increase brightness */
			if (global_pwm.channels[i].brightness < global_pwm.channels[i].target_brightness) {
				/* calculate new brightness value, high byte is brightness, low byte is remainder */
				global_pwm.channels[i].brightness_and_remainder += global_pwm.channels[i].speed;

				/* if new brightness is lower than before or brightness is higher than the target,
				* just set the target brightness and reset the remainder, since we addedd too much */
				if (global_pwm.channels[i].brightness < old_brightness || global_pwm.channels[i].brightness > global_pwm.channels[i].target_brightness) {
					global_pwm.channels[i].brightness = global_pwm.channels[i].target_brightness;
					global_pwm.channels[i].remainder = 0;
				}

				/* or decrease brightness */
			}
			else if (global_pwm.channels[i].brightness > global_pwm.channels[i].target_brightness) {
				/* calculate new brightness value, high byte is brightness, low byte is remainder */
				global_pwm.channels[i].brightness_and_remainder -= global_pwm.channels[i].speed;

				/* if new brightness is higher than before or brightness is lower than the target, just set the target brightness */
				if (global_pwm.channels[i].brightness > old_brightness || global_pwm.channels[i].brightness < global_pwm.channels[i].target_brightness) {
					global_pwm.channels[i].brightness = global_pwm.channels[i].target_brightness;
					global_pwm.channels[i].remainder = 0;
				}
			}

			/* if target brightness has been reached, set flag */
			if (global_pwm.channels[i].brightness == global_pwm.channels[i].target_brightness) {
				global_pwm.channels[i].flags.target_reached = 1;
			}
		}
	}
}

/** prepare next timeslot */
static inline void prepare_next_timeslot(void) {
	/* check if this is the last interrupt */
	if (pwm.index >= pwm.count) {
		/* select first timeslot and trigger timeslot rebuild */
		pwm.index = 0;
		global.flags.last_pulse = 1;
		OCR1B = 65000;
	}
	else {
		/* load new top and bitmask */
		OCR1B = pwm.slots[pwm.index].top;
		pwm.next_bitmask = pwm.slots[pwm.index].mask;

		/* select next timeslot */
		pwm.index++;
	}

	/* clear compare interrupts which might have in the meantime happened */
	//TIFR |= _BV(OCF1B);
}

/** interrupts*/

/** timer1 overflow (=output compare a) interrupt */
ISR(SIG_OUTPUT_COMPARE1A) {
	/* decide if this interrupt is the beginning of a pwm cycle */
	if (pwm.next_bitmask == 0) {
		/* output initial values */
		PORT_LED = ~pwm.initial_bitmask;
        
		/* if next timeslot would happen too fast or has already happened, just spinlock */
		while (TCNT1 + 500 > pwm.slots[pwm.index].top) {
			/* spin until timer interrupt is near enough */
			while (pwm.slots[pwm.index].top > TCNT1) {
				asm volatile ("nop");
			}

			/* output value */
			PORT_LED &= ~pwm.slots[pwm.index].mask;

			/* we can safely increment index here, since we are in the first timeslot and there
			* will always be at least one timeslot after this (middle) */
			pwm.index++;
		}

		/* signal new cycle to main procedure */
		global.flags.new_cycle = 1;
	}

	/* prepare the next timeslot */
	prepare_next_timeslot();
}

/** timer1 output compare b interrupt */
ISR(SIG_OUTPUT_COMPARE1B) {
	/* normal interrupt, output pre-calculated bitmask */
	PORT_LED &= ~pwm.next_bitmask;

	/* and calculate the next timeslot */
	prepare_next_timeslot();
}



void pwm_handler() {
	if (global.flags.last_pulse) {
		global.flags.last_pulse = 0;
		update_pwm_timeslots();
	}
	/* at the beginning of each pwm cycle, call the fading engine and
	* execute all script threads */
	if (global.flags.new_cycle) {
		global.flags.new_cycle = 0;
		update_brightness();
		execute_script_threads();
	}
}
