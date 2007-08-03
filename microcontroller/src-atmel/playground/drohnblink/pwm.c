

#include <avr/io.h>

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "pwm.h"


//defines for the Portpins the LEDs are connected to.
#define PORT_LED0 PORTD
#define DDR_LED0 DDRD

#define PORT_LED1 PORTB
#define DDR_LED1 DDRB

#define PORT_RGB PORTC
#define DDR_RGB DDRC


/* structs */
volatile struct global_t global = {{0, 0}};

/* prototypes */


/* TYPES AND PROTOTYPES */

/* encapsulates all pwm data including timeslot and output mask array */
typedef struct{
	struct {
		uint16_t mask;
		uint16_t top;
	} slots[PWM_MAX_TIMESLOTS];

	uint8_t index;  /* current timeslot index in the 'slots' array */
	uint16_t next_bitmask; /* next output bitmask, or signal for start or middle of pwm cycle */
}timeslots_t;

static inline void prepare_next_timeslot(void);

/* GLOBAL VARIABLES */

//PD2, PD3, PD4, PD5, PD6, PD7, PB0, PB5, PB4, PB3, PB2, PB1
static const uint16_t masks[] = {
	0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xff7f, 0xfeff, 0xdfff, 0xefff, 0xf7ff, 0xfbff, 0xfdff
};

static const uint8_t masks_rgb[] = {
	0x20, 0x10, 0x08
};


/* pwm timeslots (the top values and masks for the timer1 interrupt) */
//this one stores the structs for the 3 colors
static timeslots_t tss[3];
//backup of the above, used while update of the other
static timeslots_t backup_tss[3];

//working copy - the right color is copied here at the biginning of each cycle
static timeslots_t pwm;

//flags when tss is being updated
uint8_t tss_write_in_progress;

uint8_t bright[3][PWM_CHANNELS];


/* timer top values for 62 brightness levels (stored in flash) */
static const uint16_t timeslot_table[] PROGMEM = {
       	7,      26,     59,     104,    163,    234,    319,    417,
        527,    651,    788,    938,    1100,   1276,   1465,   1667,
        1882,   2109,   2350,   2604,   2871,   3151,   3444,   3750,
        4069,   4401,   4746,   5104,   5475,   5859,   6257,   6667,
        7090,   7526,   7975,   8438,   8913,   9401,   9902,   10417,
        10944,  11484,  12038,  12604,  13184,  13776,  14382,  15000,
        15632,  16276,  16934,  17604,  18288,  18984,  19694,  20417,
        21152,  21901,  22663,  23438,  24225,  25026,  // 25840,
};
  
/* FUNCTIONS AND INTERRUPTS */

/** init timer 1 */
inline void init_timer1(void) {
	/* no prescaler */
	TCCR1B = _BV(CS10);
	//TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);
	TCCR1A = 0;
#ifdef _ATMEGA88
	TCCR1C = 0;
#endif

	/* enable timer1 overflow (=output compare 1a)
	* and output compare 1b interrupt */
	_TIMSK_TIMER1 |= _BV(OCIE1A) | _BV(OCIE1B);

	/* set value for start of new cycle*/
	//a little larger, so this happens just after the last possible regular int
	OCR1A = 25100;

	/* load initial delay, trigger an overflow */
	OCR1B = 65000;
}



/** init pwm */
void init_pwm(void) {
	PORT_LED0 = 0;
	DDR_LED0 = 0xfc;
	
	PORT_LED1 = 0;
	DDR_LED1 = 0x3f;
	
	DDR_RGB |= 0x38;

	update_timeslots();
	
	init_timer1();
}

/** update pwm timeslot table */
void calculate_timeslots(timeslots_t * ts, uint8_t bright[]) {
	uint8_t sorted[PWM_CHANNELS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	uint8_t i, j;
	uint16_t mask = 0x3ffc;
	uint8_t last_brightness = 0;

	/* sort channels according to the current brightness */
	for (i=0; i<PWM_CHANNELS; i++) {
		for (j=i+1; j<PWM_CHANNELS; j++) {
			if (bright[sorted[j]] < bright[sorted[i]]) {
				uint8_t temp;

 				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
		}
	}

	/* timeslot index */
	j = 0;

	//store as initial bitmask
	ts->next_bitmask = mask;
	
	/* calculate timeslots and masks */
	for (i=0; i < PWM_CHANNELS; i++) {

		/* check if a timeslot is needed */
		if (bright[sorted[i]] == 0){
			mask &= masks[sorted[i]];
			//store as initial bitmask
			ts->next_bitmask = mask;
		}else if (bright[sorted[i]] < 63){

			/* insert new timeslot if brightness is new */
			if (bright[sorted[i]] > last_brightness) {

				/* remember mask and brightness for next timeslot */
				mask &= masks[sorted[i]];
				last_brightness = bright[sorted[i]];

				/* allocate new timeslot */
				ts->slots[j].top = pgm_read_word(&timeslot_table[bright[sorted[i]] - 1 ]);
				ts->slots[j].mask = mask;
				j++;
			}else {
				/* change mask of last-inserted timeslot */
				mask &= masks[sorted[i]];
				ts->slots[j-1].mask = mask;
			}
		}
	}

	/* reset pwm structure */
	ts->index = 0;
	
	//insert dummy slot at end - its top is never reached, so the other interrupt
	//is executed instead.
	ts->slots[j].top = 65000;
}


void update_timeslots(){
	tss_write_in_progress = 1;
	calculate_timeslots(&tss[0], bright[0]);
	calculate_timeslots(&tss[1], bright[1]);
	calculate_timeslots(&tss[2], bright[2]);
	tss_write_in_progress = 0;
	
	memcpy(&backup_tss, &tss, sizeof(tss));
}


/** prepare next timeslot */
static inline void prepare_next_timeslot(void) {
	/* load new top and bitmask */
	OCR1B = pwm.slots[pwm.index].top;
	pwm.next_bitmask = pwm.slots[pwm.index].mask;

	/* select next timeslot */
	pwm.index++;

	/* clear compare interrupts which might have in the meantime happened */
	//TIFR |= _BV(OCF1B);
}

/** interrupts*/

/** timer1 overflow (=output compare a) interrupt */
ISR(SIG_OUTPUT_COMPARE1A) {
	static uint8_t rgb = 2;
	uint8_t x, rgb_mask;
	uint8_t * ts_source;

	//select next component
	if(++rgb > 2) rgb = 0;
	rgb_mask = masks_rgb[rgb];
		
	//use backup timeslot table, if first is being written to.
	if(tss_write_in_progress){
		ts_source = (uint8_t *) &backup_tss[rgb];
	}else{
		ts_source = (uint8_t *) &tss[rgb];
	}
		
	//copy next timeslot structure
	for(x=0;x<sizeof(timeslots_t);x++){
		((uint8_t*)&pwm)[x] = ts_source[x];	
	}

	// spin until it is time for the next cycle
	while(TCNT1 < 25840);
	//switch to next color
	PORT_LED0 = 0;
	PORT_LED1 = 0;
	PORT_RGB = rgb_mask;
	for(x=0;x<10;x++)
		asm volatile("nop");
	//reset timer
	TCNT1 = 0;
	/* output initial values */
	PORT_LED0 = LOW(pwm.next_bitmask);
	PORT_LED1 = HIGH(pwm.next_bitmask);
	
	/* if next timeslot would happen too fast or has already happened, just spinlock */
	while (TCNT1 + 500 > pwm.slots[pwm.index].top) {
		/* spin until timer interrupt is near enough */
		while (pwm.slots[pwm.index].top > TCNT1);

		/* output value */
		PORT_LED0 = LOW(pwm.slots[pwm.index].mask);
		PORT_LED1 = HIGH(pwm.slots[pwm.index].mask);
		
		/* we can safely increment index here, since we are in the first timeslot and there
		* will always be at least one (dummy) timeslot after this */
		pwm.index++;
	}

	/* signal new cycle to main procedure */
	global.flags.new_cycle = 1;

	/* prepare the next timeslot */
	prepare_next_timeslot();
}

/** timer1 output compare b interrupt */
ISR(SIG_OUTPUT_COMPARE1B) {
	/* normal interrupt, output pre-calculated bitmask */
	PORT_LED0 = LOW(pwm.next_bitmask);
	PORT_LED1 = HIGH(pwm.next_bitmask);
	
	/* and calculate the next timeslot */
	prepare_next_timeslot();
}
