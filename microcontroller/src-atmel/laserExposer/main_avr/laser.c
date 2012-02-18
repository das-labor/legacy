
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ioport.h"
#include "pinout.h"

#include <util/delay.h>

uint8_t data_table[512];

uint16_t begin_delay = 9750;

uint16_t end_delay = 47500ul;
uint16_t sync_timeout_delay = 50000ul;

static uint8_t laser_state;

volatile uint8_t write_line_enable = 1;

static uint16_t last_line_start;

#define LS_SYNC 0
#define LS_LINE 1

//the laser opto triggers this interrupt
ISR(TIMER1_CAPT_vect){
	DDRC |= _BV(PC6);
	PORTC |= _BV(PC6);

	last_line_start = ICR1;

	TCCR1A = (1<<COM1A1) ;                //turn off output on compare match
	TCCR1A = (1<<COM1A1) | (1<<FOC1A);    //Force compare match (Laser off)
	TIMSK &= ~(1<<TICIE1); //turn ourselves off
	
	uint16_t start_time = ICR1 + begin_delay; //calculate at which timer value to begin lasering 
	OCR1A = start_time;                  //set OCR1A to that value (this is used from laser write line)
	OCR1B = start_time - 500;            //set up OCR1B to occur long enough before that to have the interrupt
                                         //routine ready and waiting for the OCR1A match
	laser_state = LS_LINE;

	PORTC &= ~_BV(PC6);
}



// A data value is always a delay or an end mark.
// A delay means: toggle the laser after this amount of time
// (initial state is off)
// an end mark means "drawing is finished for this line"
// 
// The data is encoded in the following way:
// a byte from 1 to 255 is that amount of delay.
// a byte = 0 means that a 16bit value follows in the next two bytes,
// which shall be interpreted as the delay.
// a zero in the 16bit value means "end of line"

static void laser_write_line(){
	uint8_t * data = data_table;
	
	//wait for OCR0A compare match (begin of line, laser off)
	while((TIFR & (1<<OCF1A)) == 0);
	TIFR = (1<<OCF1A);

	TCCR1A = (1<<COM1A0); //Toggle  laser on compare match from now

	while(1){
		while(1){
			uint8_t tmp = *data++; 
			if(tmp == 0) //16 bit value?
				break;   //yes: break inner while
			OCR1A += tmp; //no: use as delay
			
			while((TIFR & (1<<OCF1A)) == 0); //and wait for it
			TIFR = (1<<OCF1A);
			                              //interpret next byte|^
		}
		
		uint16_t tmp16;
		tmp16 = *data++;               //read 16bit value
		tmp16 |= (*data++)<<8;
		
		if(tmp16 == 0){               //end mark?
			break;                    //yes: end line
		};
		OCR1A += tmp16;                   //no: delay that amount
		while((TIFR & (1<<OCF1A)) == 0);  //and wait for it...
		TIFR = (1<<OCF1A);
		                               //interpret next byte|^
	}
}

ISR(TIMER1_COMPB_vect){
	DDRC |= _BV(PC7);
	PORTC |= _BV(PC7);

	if(laser_state == LS_LINE){
		if(write_line_enable){
			laser_write_line();
		}
		TCCR1A = (1<<COM1A1) ;
		TCCR1A = (1<<COM1A1) | (1<<FOC1A); //Assure that laser is off		

		TIFR  =  (1<<ICF1); //clear flag
		TIMSK |= (1<<TICIE1);  //turn on opto capture int

		TCCR1A = (1<<COM1A1) | (1<<COM1A0); //turn laser on on next compare
                                            //for the sync pulse that hits the opto
		
		OCR1A = last_line_start + end_delay;           //set up the time for the sync pulse
		laser_state = LS_SYNC;              //tell ourselves that we are in "searching for sync" state
		OCR1B = last_line_start + sync_timeout_delay;  //set up a timeout for the sync pulse to occur
		                                    //(which brings us back in to the interrupt handler to the
		                                    //case below, because the ICR interrupt has not occured
		                                    //and set the laser_state to LS_LINE, and reprogrammed the comnpares)
	}else{
		//LS_SYNC (we have missed the sync pulse)
		
		TCCR1A = (1<<COM1A1) ;
		TCCR1A = (1<<COM1A1) | (1<<FOC1A); //Assure that laser is off

		TIFR  =  (1<<ICF1); //clear flag
		TIMSK |= (1<<TICIE1);  //turn on opto capture int

		TCCR1A = (1<<COM1A1) | (1<<COM1A0); //turn laser on on next compare (sync)
		
		OCR1A = OCR1B + end_delay;
		OCR1B = OCR1B + sync_timeout_delay;
	}
	
	PORTC &= ~_BV(PC7);

}


//test pattern
void fill_data_table(){
	uint16_t x;
	for(x=0;x < 101; x++){
		data_table[x] = 122 - x;
	}
	
	data_table[102] = 0;
	data_table[103] = 0;
	data_table[104] = 0;	
}


void laser_init(){

	fill_data_table();

	SET_DDR(LASER);
	SET_DDR(LASER_PWM);
	OUTPUT_ON(LASER_PWM);

	TCCR1B = 1;//clk/1
}

void laser_on(){
	TCNT1 = 1;
	
	OCR1B = 0;
	OCR1A = 0;
	
	TIFR = (1<<OCF1B) | (1<<ICF1);
	TIMSK |= (1<<OCIE1B) | (1<<TICIE1); //capture b and icp int on (they do all the work)	
}

void laser_off(){
	TIMSK &= ~ ((1<<OCIE1B) | (1<<TICIE1)); //ints off
	TCCR1A = 0; //disconnect laser from timer
}

