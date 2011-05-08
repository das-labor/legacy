
#include <avr/io.h>
#include "ioport.h"
#include "stepper.h"
#include "pinout.h"

uint8_t stepper_command;
uint8_t stepper_run;
//1= forward, 0= reverse
uint8_t stepper_dir;


#define STEPPER_OFF_DELAY 100

uint8_t stepper_delay = 1;
uint8_t stepper_d;

uint16_t stepper_off_delay;

uint8_t stepper_sequence[4] = {1,4,2,8};
uint8_t stepper_state;

uint16_t step_counter;
uint16_t position_request;

void update_stepper(){
	switch(stepper_command){
		case CMD_STEP:
			stepper_off_delay = STEPPER_OFF_DELAY;
			if(stepper_dir){
				stepper_state++;
				step_counter ++;
				stepper_state %= 4;
			}else{
				stepper_state--;
				step_counter --;
				stepper_state %= 4;
			}
			stepper_command = 0;
			break;
		case CMD_HOME:
			stepper_dir = 0;
			stepper_run = 1;
			if(INPUT(LS)){
				stepper_run = 0;
				stepper_command = 0;
				step_counter = 0;
			}else{
				
			}
			break;
		case CMD_GOTO:
			if(step_counter == position_request){
				stepper_run = 0;
				stepper_command = 0;
			}else if(step_counter < position_request){
				stepper_dir = 1;
				stepper_run = 1;
			}else{
				stepper_dir = 0;
				stepper_run = 1;
			
			}
		
	}
	
	if(stepper_run){
		if(stepper_d == 0){
			stepper_d = stepper_delay;
			stepper_off_delay = STEPPER_OFF_DELAY;
			if(stepper_dir){
				stepper_state++;
				step_counter ++;
				stepper_state %= 4;
			}else{
				stepper_state--;
				step_counter --;
				stepper_state %= 4;
			}
		}else{
			stepper_d --;
		}
	}
	
	if(stepper_off_delay){
		stepper_off_delay --;
		STEPPER_PORT = stepper_sequence[stepper_state];
	}else{
		STEPPER_PORT = 0;
	}
}


void stepper_init(){
	SET_DDR(LS_LED);
	OUTPUT_ON(LS_LED);
	
	STEPPER_DDR = 0x0f;
}
