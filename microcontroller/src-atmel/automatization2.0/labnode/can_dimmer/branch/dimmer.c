
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NUM_CHANNELS 4
#define MAX_VAL 550

volatile uint8_t update_in_progress;
uint8_t dim_max[NUM_CHANNELS];

uint16_t dim_vals_sorted[NUM_CHANNELS];
uint8_t channels_sorted[NUM_CHANNELS];

volatile uint8_t channels_active[NUM_CHANNELS];


//synchronize to zero cross
ISR(TIMER1_CAPT_vect) {
	if(channels_active[3])
		PORTC |= _BV(PC5);	//activate triac on zero-cross(EVG)
	
	TCNT1 = 620;
}

ISR(TIMER1_COMPB_vect) {
	static uint8_t state;
	static uint8_t next;

	static uint8_t channels[NUM_CHANNELS];
	static uint16_t dim_vals[NUM_CHANNELS];
	
	if (state == 0) {
		//load new soft-PWM values if no update is in progress
		if (!update_in_progress) {
			uint8_t x;
			for (x = 0; x < NUM_CHANNELS; x++) {
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

		//set next value to intterupt
		OCR1B = dim_vals[0];
		if (dim_vals[0] != MAX_VAL)  //exept case where all lamps are off
			state = 1;

	} else {
		handle_next:
		//check which channel needs to be activated

		switch (channels[next]) {
			case 0: if(channels_active[0])PORTA |= _BV(PA4); break;
			case 1: if(channels_active[1])PORTA |= _BV(PA5); break;
			case 2: if(channels_active[2])PORTC |= _BV(PC4); break;
			case 3: if(channels_active[3])PORTD |= _BV(PD5); break;
		}

		next++;
		if ((next != NUM_CHANNELS) && (dim_vals[next] != MAX_VAL)) {
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
	//init soft-PWM registers, set all channels to MAX_VAL == minPhasecut
	uint8_t x;
	for (x = 0; x < NUM_CHANNELS; x++) {
		channels_sorted[x] = x;
		dim_vals_sorted[x] = MAX_VAL;
		channels_active[x] = 0;
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
	OCR1A = 625;

	OCR1B = MAX_VAL;

	TIMSK |= _BV(TICIE1) | _BV(OCIE1B);//Input capture int on, output compare 1B on
}


void set_dimmer(uint8_t channel, uint8_t bright) {
	
	if(channel > (NUM_CHANNELS -1))
		return;
	
	uint16_t dimval = 512 - bright * 2;
	
	if (!bright)
		dimval = MAX_VAL;
	
	if (bright == 255) 
	{
		dim_max[channel] = 1;
		//enable port if max_brightness == always on
		
		switch (channel) {
			case 0: if(channels_active[0])PORTA |= _BV(PA4); break;
			case 1: if(channels_active[1])PORTA |= _BV(PA5); break;
			case 2: if(channels_active[2])PORTC |= _BV(PC4); break;
			case 3: if(channels_active[3])PORTD |= _BV(PD5); break;
		}
		dimval = MAX_VAL;	//no need for soft-PWM, ports are always on
	}
 	else dim_max[channel] = 0;

	
	update_in_progress = 1;

	uint8_t x;
	//remove channel from list
	for (x = 0; x < NUM_CHANNELS; x++) { 
		if (channels_sorted[x] == channel) {
			for (; x<NUM_CHANNELS - 1; x++) {
				channels_sorted[x] = channels_sorted[x+1];
				dim_vals_sorted[x] = dim_vals_sorted[x+1];				
			}
			break;
		}
	}

	dim_vals_sorted[NUM_CHANNELS-1] = MAX_VAL+1;

	//insert channel into table
	for (x = 0; x < NUM_CHANNELS; x++) { 
		if (dimval < dim_vals_sorted[x]) {
			uint8_t y;
			for (y = NUM_CHANNELS - 1; y > x; y--) {
				dim_vals_sorted[y] = dim_vals_sorted[y-1];
				channels_sorted[y] = channels_sorted[y-1];
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
	if(channel < NUM_CHANNELS)
	{
		channels_active[channel]=enable;
		if(enable){
			if(dim_max[channel])
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
			case 3: PORTC &= ~_BV(PC5);	break; //disable triac (EVG)
		}
			
	}
}	
