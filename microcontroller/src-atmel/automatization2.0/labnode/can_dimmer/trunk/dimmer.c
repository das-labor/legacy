
#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_CHANNELS 4
#define MAX_VAL 550

//synchronize to zero cross
ISR(TIMER1_CAPT_vect){
	TCNT1 = 620;	
}

volatile uint8_t update_in_progress;
uint8_t dim_max[NUM_CHANNELS];

uint16_t dim_vals_sorted[NUM_CHANNELS];
uint8_t channels_sorted[NUM_CHANNELS];


ISR(TIMER1_COMPB_vect){
	static uint8_t state;
	static uint8_t next;
	
	static uint8_t channels[NUM_CHANNELS];
	static uint16_t dim_vals[NUM_CHANNELS];
	
	if(state == 0){
		if(!update_in_progress){
			uint8_t x;
			for(x=0;x<NUM_CHANNELS;x++){
				channels[x] = channels_sorted[x];
				dim_vals[x] = dim_vals_sorted[x];
			}
		}
		next = 0;
		if(dim_max[0]) PORTA |= (1<<PA4); else PORTA &= ~(1<<PA4);
		if(dim_max[1]) PORTA |= (1<<PA5); else PORTA &= ~(1<<PA5);
		if(dim_max[2]) PORTC |= (1<<PC4); else PORTC &= ~(1<<PC4);
		if(dim_max[3]) PORTC |= (1<<PC5); else PORTC &= ~(1<<PC5);
		
		OCR1B = dim_vals[0];
		if(dim_vals[0] != MAX_VAL){ //exept case where all lamps are off
			state = 1;
		}
	}else{
		handle_next:
		switch(channels[next]){
			case 0: PORTA |= (1<<PA4); break;
			case 1: PORTA |= (1<<PA5); break;
			case 2: PORTC |= (1<<PC4); break;
			case 3: PORTC |= (1<<PC5); break;	
		}
		next++;
		if((next != NUM_CHANNELS) && (dim_vals[next] != MAX_VAL)){
			OCR1B = dim_vals[next];
			if(TCNT1 >= OCR1B){   //if allready time for next
				TIFR = (1<<OCF1B); //reset flag in case it was set allready
				goto handle_next;   //and handle now
			}
		}else{
			//all channels handled
			OCR1B = MAX_VAL;
			state = 0;		
		}
	}
}



void dimmer_init(){
	uint8_t x;
	for(x=0;x<NUM_CHANNELS;x++){
		channels_sorted[x] = x;
		dim_vals_sorted[x] = MAX_VAL;
	}

	DDRA |= (1<<PA4) | (1<<PA5);
	DDRC |= (1<<PC4) | (1<<PC5);

	TCCR1B |= (1<<ICNC1) | (1<<WGM12) | (1<<CS12); //CTC (TOP = OCR1A), clk/256
	OCR1A = 625;

	OCR1B = MAX_VAL;

	TIMSK |= (1<<TICIE1) | (1<<OCIE1B);//Input capture int on, output compare 1B on

	
}


void set_dimmer(uint8_t channel, uint8_t bright){

	if(channel == 3) bright = (bright>128) ? 255:0;//only allow full or no brighnes for evg on channel 3

	uint16_t dimval = 512 - bright * 2;
	if(bright == 0) dimval = MAX_VAL;
	if(bright == 255) dim_max[channel] = 1; else dim_max[channel] = 0;



	update_in_progress = 1;

	uint8_t x;
	//remove channel from list
	for(x=0;x<NUM_CHANNELS;x++){ 
		if(channels_sorted[x] == channel){
			for(;x<NUM_CHANNELS-1;x++){
				channels_sorted[x] = channels_sorted[x+1];
				dim_vals_sorted[x] = dim_vals_sorted[x+1];				
			}
			break;
		}
	}
	
	dim_vals_sorted[NUM_CHANNELS-1] = MAX_VAL+1;
	
	//insert channel into table
	for(x=0;x<NUM_CHANNELS;x++){ 
		if(dimval < dim_vals_sorted[x]){
			uint8_t y;
			for(y=NUM_CHANNELS-1;y>x;y--){
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
