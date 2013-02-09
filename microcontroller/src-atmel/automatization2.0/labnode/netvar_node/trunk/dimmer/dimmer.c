#include <avr/io.h>
#include <avr/interrupt.h>

#include "../config.h"

/*

Here are some wave forms:
Keep in mind that the value MAX_VAL is the timer value at Triac
turn off, thereby dim_val = MAX_VAL-1 is the lowest possible brightnes. This is
Somewhat different to totally off (which would theoretically be at 625),
but the Lightbulbs only glow very dimmly with the value 550 as MAX_VAL.
The highest brightnes is at dim_val = 0 (turning the triac on at about zero cross),
But the triac is actually turned total on/off for the maximum/minimum input value.

                     * * 
                   *     *
                  *       *             *
                 *         *           *
Mains ---       *           *         *
                             *       *
                              *     *
                                * *      

ICP
(Zero cross)  __|___________|___________|__  <- ICP Pulses set timer to 620
                                                (short before overflow) timer = 0 at zero cross
Output0       ______________________________
                      ____        ____
Output1       _______|    |______|    |_____
                    ______      ______
Output2       _____|      |____|      |_____
              ______________________________
Output3       
				          ^           ^
                          |           |
                  Triac is turned of at timer == MAX_VAL (550)
                  for the channels not at max or min
                  This point is somewhat before the zero cross(which is at 625)
                  Triac stays on on its own until zero cross. Turning the gate
                  off before is required, because else it might stay on. 

*/

#define MAX_VAL 550


//synchronize to zero cross
ISR(TIMER1_CAPT_vect) {
	TCNT1 = 620;
}

volatile uint8_t update_in_progress;
uint8_t dim_max[NUM_DIMMER_CHANNELS];

uint16_t dim_vals_sorted[NUM_DIMMER_CHANNELS];
uint8_t channels_sorted[NUM_DIMMER_CHANNELS];


ISR(TIMER1_COMPB_vect) {
	static uint8_t state;
	static uint8_t next;

	static uint8_t channels[NUM_DIMMER_CHANNELS];
	static uint16_t dim_vals[NUM_DIMMER_CHANNELS];

	if (state == 0) { //state = 0 means the timer is at MAX_VAL
		if (!update_in_progress) {
			//transfer the control information from main programm to interrupt
			//if it is not being updated from the main programm at the moment.
			uint8_t x;
			for (x = 0; x < NUM_DIMMER_CHANNELS; x++) {
				channels[x] = channels_sorted[x];
				dim_vals[x] = dim_vals_sorted[x];
			}
		}
		next = 0;
		
		//turn the outputs full on, if dim_max is set. Otherwise
		//this is the time for turn off of the output (timer at MAX_VAL)
		if (dim_max[0]) PORTA |= _BV(PA4); else PORTA &= ~_BV(PA4);
		if (dim_max[1]) PORTA |= _BV(PA5); else PORTA &= ~_BV(PA5);
		if (dim_max[2]) PORTC |= _BV(PC4); else PORTC &= ~_BV(PC4);
		if (dim_max[3]) PORTC |= _BV(PC5); else PORTC &= ~_BV(PC5);

		OCR1B = dim_vals[0]; //set timer to the first time an output needs changing
		if (dim_vals[0] != MAX_VAL) { //exept case where all lamps are off
			state = 1;
		}
	} else {
		handle_next:
		//since the dim_vals are sorted, we need to find out the channel,
		//for which this one is, and set its output.
		switch (channels[next]) {
			case 0: PORTA |= _BV(PA4); break;
			case 1: PORTA |= _BV(PA5); break;
			case 2: PORTC |= _BV(PC4); break;
			case 3: PORTC |= _BV(PC5); break;
		}
		next++;
		if ((next != NUM_DIMMER_CHANNELS) && (dim_vals[next] != MAX_VAL)) {
			OCR1B = dim_vals[next];
			if (TCNT1 >= OCR1B){   //if allready time for next
				TIFR = _BV(OCF1B); //reset flag in case it was set allready
				goto handle_next;   //and handle now
			}
		} else {
			//all channels handled
			OCR1B = MAX_VAL;
			state = 0;
		}
	}
}


void dimmer_init() {
	uint8_t x;
	for (x = 0; x < NUM_DIMMER_CHANNELS; x++) {
		channels_sorted[x] = x;
		dim_vals_sorted[x] = MAX_VAL;
	}

	DDRA |= _BV(PA4) | _BV(PA5);
	DDRC |= _BV(PC4) | _BV(PC5);

	TCCR1B |= _BV(ICNC1) | _BV(WGM12) | _BV(CS12); //CTC (TOP = OCR1A), clk/256

	//The output comapre A is the period of the timer.
	//thereby it runs at exactly 100Hz and stays synchronus with the mains frequency
	//even if an ICP pulse should be missing
	OCR1A = 625;

	OCR1B = MAX_VAL;

	TIMSK |= _BV(TICIE1) | _BV(OCIE1B);//Input capture int on, output compare 1B on
}


void set_dimmer(void* ch, uint8_t bright) {
	//this is the API to the dimmer module.
	//it calculates the dim_vals for the respective brightnes
	//and applys the dim_max flag for channels at maximum brightnes (255)
	// bright   dim_val   dim_max
	//   0        512        0
	//   .         .         0
	//   .         .         0
	//  254        4         0
	//  255        2         1

	uint8_t channel = (uint8_t) ch;

	uint16_t dimval = 512 - bright * 2;
	if (bright == 0) dimval = MAX_VAL;
	if (bright == 255) dim_max[channel] = 1; else dim_max[channel] = 0;

	update_in_progress = 1;


	//now we enter the new dim_val into the sorted list dim_vals_sorted.
	//the respective channel is stored in channels_sorted.
	//example:
	// dim_vals_sorted    channels_sorted
	//        150                3
	//        400                1
	//        500                2
	//        625                0

	uint8_t x;
	//remove channel from list
	for (x = 0; x < NUM_DIMMER_CHANNELS; x++) {
		if (channels_sorted[x] == channel) {
			for (; x < NUM_DIMMER_CHANNELS - 1; x++) {
				channels_sorted[x] = channels_sorted[x+1];
				dim_vals_sorted[x] = dim_vals_sorted[x+1];
			}
			break;
		}
	}

	dim_vals_sorted[NUM_DIMMER_CHANNELS - 1] = MAX_VAL+1;

	//insert channel into table
	for (x = 0; x < NUM_DIMMER_CHANNELS; x++) {
		if (dimval < dim_vals_sorted[x]) {
			uint8_t y;
			for (y = NUM_DIMMER_CHANNELS - 1; y > x; y--) {
				dim_vals_sorted[y] = dim_vals_sorted[y - 1];
				channels_sorted[y] = channels_sorted[y - 1];
			}
			dim_vals_sorted[x] = dimval;
			channels_sorted[x] = channel;
			break;
		}
	}
	update_in_progress = 0;
}

