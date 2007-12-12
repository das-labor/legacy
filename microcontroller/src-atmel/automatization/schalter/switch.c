

#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "fifo.h"
#include "xcan.h"
#include "rf.h"
#include "lamp_and_switch.h"


#define PIN_SWITCH PIND
#define BIT_SWITCH PD0

TimerControlBlock switchtimer;

void labor_on(){
	rc_switch_set(POWER_BAR, 1);
	rc_switch_set(POWER_COUCH, 1);
	rc_switch_set(POWER_BASTELECKE, 1);
	rc_switch_set(POWER_FLIPPER, 1);

	rc_switch_set(LAMP_COUCHFLUTER, 1);
}

void labor_off(){
	
	rc_switch_set(POWER_BASTELECKE, 0);
	rc_switch_set(POWER_COUCH, 0);
	rc_switch_set(POWER_BAR, 0);
	rc_switch_set(POWER_FLIPPER, 0);

	rc_switch_set(LAMP_COUCHFLUTER, 0);
}

AVRX_GCC_TASKDEF(switchtask, 20, 8){
	uint8_t state, oldstate = 0;
	
	while(1){
		state = PIN_SWITCH & (1<<BIT_SWITCH);
		if (state != oldstate){
			oldstate = state;
			if(state){
				labor_off();	
			}else{
				labor_on();
			}
		}
		AvrXDelay(&switchtimer, 200);		
	}
}
