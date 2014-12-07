#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "dimmer.h"

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

volatile uint8_t update_in_progress;
uint8_t dim_max[NUM_DIMMER_CHANNELS];

uint16_t dim_vals_sorted[NUM_DIMMER_CHANNELS];
uint8_t channels_sorted[NUM_DIMMER_CHANNELS];
uint8_t dim_vals_8bit[NUM_DIMMER_CHANNELS];

volatile uint8_t channels_active[NUM_DIMMER_CHANNELS];

//lookuptable for gamma corretion
static const uint8_t exptab[256] PROGMEM =
{
    255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 
    253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 252, 252, 252, 
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 251, 251, 251,
    251, 251, 251, 251, 250, 250, 250, 250, 250, 250, 250, 250, 250, 249, 249, 249, 249, 249, 249, 249, 249, 248, 
    248, 248, 248, 248, 248, 247, 247, 247, 247, 247, 247, 246, 246, 246, 246, 246, 245, 245, 245, 245, 245, 244, 
    244, 244, 244, 243, 243, 243, 243, 242, 242, 242, 241, 241, 241, 241, 240, 240, 240, 239, 239, 239, 238, 238, 
    237, 237, 237, 236, 236, 235, 235, 234, 234, 234, 233, 233, 232, 232, 231, 231, 230, 230, 229, 228, 228, 227, 
    227, 226, 225, 225, 224, 223, 223, 222, 221, 220, 220, 219, 218, 217, 216, 216, 215, 214, 213, 212, 211, 210, 
    209, 208, 207, 206, 205, 204, 203, 202, 200, 199, 198, 197, 196, 194, 193, 192, 190, 189, 187, 186, 184, 183, 
    181, 179, 178, 176, 174, 173, 171, 169, 167, 165, 163, 161, 159, 157, 155, 153, 150, 148, 146, 143, 141, 138, 
    136, 133, 131, 128, 125, 122, 119, 116, 113, 110, 107, 104, 100, 97, 93, 90, 86, 83, 79, 75, 71, 67, 63, 59, 54, 
    50, 45, 41, 36, 31, 26, 21, 16, 11, 5, 0
};

//synchronize to zero cross
ISR(TIMER1_CAPT_vect) {
	if (channels_active[3])
		PORTC |= _BV(PC5);	//activate triac on zero-cross(EVG)

	TCNT1 = 620;
}

ISR(TIMER1_COMPB_vect) {
	static uint8_t state;
	static uint8_t next;

	static uint8_t channels[NUM_DIMMER_CHANNELS];
	static uint16_t dim_vals[NUM_DIMMER_CHANNELS];

	if (state == 0) { //state = 0 means the timer is at MAX_VAL
		//load new soft-PWM values if no update is in progress
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
		//disable all ports that are not always on
		if (!dim_max[0]) PORTA &= ~_BV(PA4);
		if (!dim_max[1]) PORTA &= ~_BV(PA5);
		if (!dim_max[2]) PORTC &= ~_BV(PC4);
		if (!dim_max[3]) PORTD &= ~_BV(PD5);

		//set next value to interrupt
		OCR1B = dim_vals[0]; //set timer to the first time an output needs changing
		if (dim_vals[0] != MAX_VAL) //exept case where all lamps are off
			state = 1;
	} else {
		handle_next:
		//since the dim_vals are sorted, we need to find out the channel,
		//for which this one is, and set its output.
		switch (channels[next]) {
			case 0: if (channels_active[0]) PORTA |= _BV(PA4); break;
			case 1: if (channels_active[1]) PORTA |= _BV(PA5); break;
			case 2: if (channels_active[2]) PORTC |= _BV(PC4); break;
			case 3: if (channels_active[3]) PORTD |= _BV(PD5); break;
		}

		next++;
		if ((next != NUM_DIMMER_CHANNELS) && (dim_vals[next] != MAX_VAL)) {
			OCR1B = dim_vals[next];
			if (TCNT1 >= OCR1B) {   //if allready time for next
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
	//init soft-PWM registers, set all channels to MAX_VAL == minPhasecut
	uint8_t x;
	for (x = 0; x < NUM_DIMMER_CHANNELS; x++) {
		channels_sorted[x] = x;
		dim_vals_sorted[x] = MAX_VAL;
		channels_active[x] = 0;
		dim_vals_8bit[x] = 255;
	}

	//set soft-PWM ports to output
	DDRA |= _BV(PA4) | _BV(PA5);	//2x triac
	DDRC |= _BV(PC4) | _BV(PC5);	//2x triac
	DDRD |= _BV(PD5); // EVG: 0-10V

	//disable all triacs
	PORTA &= ~_BV(PA4);
	PORTA &= ~_BV(PA5);
	PORTC &= ~_BV(PC4);
	PORTD &= ~_BV(PD5);
	PORTC &= ~_BV(PC5);

	PORTD |= _BV(PD6);	//pull up an Zero-Cross-Detection-Input-Pin

	TCCR1B |= _BV(ICNC1) | _BV(WGM12) | _BV(CS12); //CTC (TOP = OCR1A), clk/256

	//The output comapre A is the period of the timer.
	//thereby it runs at exactly 100Hz and stays synchronus with the mains frequency
	//even if an ICP pulse should be missing
	OCR1A = 625;

	OCR1B = MAX_VAL;

	TIMSK |= _BV(TICIE1) | _BV(OCIE1B); //Input capture int on, output compare 1B on
}


void set_dimmer(uint8_t channel, uint8_t bright) {
	//this is the API to the dimmer module.
	//it calculates the dim_vals for the respective brightnes
	//and applys the dim_max flag for channels at maximum brightnes (255)
	// bright   dim_val   dim_max
	//   0        512        0
	//   .         .         0
	//   .         .         0
	//  254        4         0
	//  255        2         1
	if (channel > (NUM_DIMMER_CHANNELS - 1))
		return;

	dim_vals_8bit[channel] = bright;

	if (channel == 3)
	{
		// do gamma correction on neon tube
		bright = pgm_read_byte(exptab + bright);
	}

	uint16_t dimval = 512 - bright * 2;

	if (!bright)
		dimval = MAX_VAL;

	if (bright == 255)
	{
		dim_max[channel] = 1;
		//enable port if max_brightness == always on

		switch (channel) {
			case 0: if (channels_active[0]) PORTA |= _BV(PA4); break;
			case 1: if (channels_active[1]) PORTA |= _BV(PA5); break;
			case 2: if (channels_active[2]) PORTC |= _BV(PC4); break;
			case 3: if (channels_active[3]) PORTD |= _BV(PD5); break;
		}
		dimval = MAX_VAL;	//no need for soft-PWM, ports are always on
	}
	else
		dim_max[channel] = 0;

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

void enable_channel(uint8_t channel, uint8_t enable)
{
	if (channel < NUM_DIMMER_CHANNELS)
	{
		channels_active[channel] = enable;
		if (enable) {
			if (dim_max[channel])
			{
				switch (channel) {
					case 0: PORTA |= _BV(PA4); break;
					case 1: PORTA |= _BV(PA5); break;
					case 2: PORTC |= _BV(PC4); break;
					case 3: PORTD |= _BV(PD5); break;
				}
			}
		//	if(channel == 3)
		//		PORTC |= _BV(PC5);	//activate triac (EVG)
		}
		else
			switch (channel) {
				case 0: PORTA &= ~_BV(PA4); break;
				case 1: PORTA &= ~_BV(PA5); break;
				case 2: PORTC &= ~_BV(PC4); break;
				case 3: PORTC &= ~_BV(PC5); break; //disable triac (EVG)
			}
	}
}

uint8_t get_channel_status() {
	return channels_active[0] + (channels_active[1] << 1) + (channels_active[2] << 2) + (channels_active[3] << 3);
}

