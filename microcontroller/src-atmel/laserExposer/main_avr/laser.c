
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

#define LS_SYNC 0
#define LS_LINE 1

//the laser opto triggers this interrupt
ISR(TIMER1_CAPT_vect){
	TCCR1A = (1<<COM1A1) ;
	TCCR1A = (1<<COM1A1) | (1<<FOC1A); //Turn laser is off
	uint16_t start_time = ICR1 + begin_delay; 
	OCR1A = start_time;
	OCR1B = start_time - 500;
	laser_state = LS_LINE;
}



void laser_write_line(){
	uint8_t * data = data_table;
	
	//wait for OCR0A compare match (begin of line, laser off)
	while((TIFR & (1<<OCF1A)) == 0);
	TIFR = (1<<OCF1A);

	TCCR1A = (1<<COM1A0); //Toggle  laser on compare match from now

	while(1){
		while(1){
			uint8_t tmp = *data++; 
			if(tmp == 0) break;
			OCR1A += tmp;
			
			while((TIFR & (1<<OCF1A)) == 0);
			TIFR = (1<<OCF1A);
		}
		
		uint16_t tmp16;
		tmp16 = *data++;
		tmp16 |= (*data++)<<8;
		
		if(tmp16 == 0){
			break;
		};
		OCR1A += tmp16;
		while((TIFR & (1<<OCF1A)) == 0);
		TIFR = (1<<OCF1A);
	}
}

ISR(TIMER1_COMPB_vect){
	if(laser_state == LS_LINE){
		if(write_line_enable){
			laser_write_line();
		}
		TCCR1A = (1<<COM1A1) ;
		TCCR1A = (1<<COM1A1) | (1<<FOC1A); //Assure that laser is off		

		TCCR1A = (1<<COM1A1) | (1<<COM1A0); //turn laser on on next compare (sync)
		
		OCR1A = ICR1 + end_delay;
		laser_state = LS_SYNC;
		OCR1B = ICR1 + sync_timeout_delay;
	}else{
		//LS_SYNC (we have missed the sync pulse)
		
		TCCR1A = (1<<COM1A1) ;
		TCCR1A = (1<<COM1A1) | (1<<FOC1A); //Assure that laser is off		

		TCCR1A = (1<<COM1A1) | (1<<COM1A0); //turn laser on on next compare (sync)
		
		OCR1A = OCR1B + end_delay;
		OCR1B = OCR1B + sync_timeout_delay;
	}
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

