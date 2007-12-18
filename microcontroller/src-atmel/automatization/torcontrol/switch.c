

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


#define DDR_RELAIS DDRC
#define PORT_RELAIS PORTC

#define BIT_RELAIS_0_OFF 0
#define BIT_RELAIS_0_ON  1
#define BIT_RELAIS_1_OFF 2
#define BIT_RELAIS_1_ON  3

#define PORT_SWITCHES PORTB
#define PIN_SWITCHES PINB
#define BIT_SWITCH0 0


AVRX_GCC_TASKDEF(switchtask, 60, 8){
	static uint8_t old_relais[2];
	static uint8_t switches[2];

	PORT_SWITCHES |= (1<<BIT_SWITCH0);
	DDR_RELAIS |= (1<<BIT_RELAIS_0_ON) | (1<<BIT_RELAIS_0_OFF) | (1<<BIT_RELAIS_1_ON) | (1<<BIT_RELAIS_1_OFF);

	
	uint8_t sw;
	
	
	while(1){
		//Light switches
		sw = 1;
		if(PIN_SWITCHES & (1<<BIT_SWITCH0))
			sw = 0;
		
		if(sw != switches[0]){
			switches[0] = sw;
			rc_switch_set(20, sw);
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
