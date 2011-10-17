

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
	can_switch_set(0x53, POWER_BASTELECKE, 1);
	rc_switch_set(POWER_FLIPPER, 1);

	rc_switch_set(LAMP_COUCHFLUTER, 1);
}

void labor_off(){
	
	can_switch_set(0x53, POWER_BASTELECKE, 0);
	rc_switch_set(POWER_COUCH, 0);
	rc_switch_set(POWER_BAR, 0);
	rc_switch_set(POWER_FLIPPER, 0);
	
	rc_switch_set(LAMP_LEUCHTSTOFF1, 0);
	rc_switch_set(LAMP_LEUCHTSTOFF2, 0);
	can_switch_set(0x53, LAMP_LEUCHTSTOFF3, 0);

	rc_switch_set(LAMP_COUCHFLUTER, 0);
	rc_switch_set(LAMP_HINTENFLUTER, 0);
	rc_switch_set(LAMP_MOODBAR, 0);
	rc_switch_set(LAMP_BASTELECKE, 0);
}

#define DDR_RELAIS DDRC
#define PORT_RELAIS PORTC

#define BIT_RELAIS_1_OFF 0
#define BIT_RELAIS_1_ON  1
#define BIT_RELAIS_0_OFF 2
#define BIT_RELAIS_0_ON  3

#define PORT_SWITCHES PORTD
#define PIN_SWITCHES PIND
#define BIT_SWITCH0 6
#define BIT_SWITCH1 7


AVRX_GCC_TASKDEF(switchtask, 60, 8){
	uint8_t state, oldstate = 0;
	static uint8_t old_relais[2];
	static uint8_t switches[2];

	PORT_SWITCHES |= (1<<BIT_SWITCH0) | (1<<BIT_SWITCH1);
	DDR_RELAIS |= (1<<BIT_RELAIS_0_ON) | (1<<BIT_RELAIS_0_OFF) | (1<<BIT_RELAIS_1_ON) | (1<<BIT_RELAIS_1_OFF);

	
	uint8_t sw;
	
	
	while(1){
		//Big Red Switch
		state = PIN_SWITCH & (1<<BIT_SWITCH);
		if (state != oldstate){
			oldstate = state;
			if(state){
				labor_off();	
			}else{
				labor_on();
			}
		}
		
		//Light switches
		sw = 1;
		if(PIN_SWITCHES & (1<<BIT_SWITCH0))
			sw = 0;
		
		if(sw != switches[0]){
			switches[0] = sw;
			rc_switch_set(20, sw);
		}
		
		sw = 1;
		if(PIN_SWITCHES & (1<<BIT_SWITCH1))
			sw = 0;
		
		if(sw != switches[1]){
			switches[1] = sw;
			rc_switch_set(21, sw);
		}
		
		if(relais[0] != old_relais[0]){
			uint8_t rstate;
			old_relais[0] = relais[0];
			rstate = relais[0];
			if(rstate==0)
				PORT_RELAIS |= (1<<BIT_RELAIS_0_ON);
			else
				PORT_RELAIS |= (1<<BIT_RELAIS_0_OFF);
		}
		
		if(relais[1] != old_relais[1]){
			uint8_t rstate;
			old_relais[1] = relais[1];
			rstate = relais[1];
			if(rstate==0)
				PORT_RELAIS |= (1<<BIT_RELAIS_1_ON);
			else
				PORT_RELAIS |= (1<<BIT_RELAIS_1_OFF);
		}

		AvrXDelay(&switchtimer, 500);
		PORT_RELAIS &= ~ ((1<<BIT_RELAIS_0_ON) | (1<<BIT_RELAIS_0_OFF) | (1<<BIT_RELAIS_1_ON) | (1<<BIT_RELAIS_1_OFF));
		
	}
}
