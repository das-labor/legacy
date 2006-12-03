
#include "reader.h"

#define PORT_MOTOR PORTC
#define DDR_MOTOR DDRC
#define BIT_MOTOR_IN PC2
#define BIT_MOTOR_OUT PC3


//Switch 1 goes low when a card is inserted.
//Goes high again when the card is at the end.
//
//Switch 2 goes low after the card passes the first roll.
//it stays low while the card is in.
//
//Switch 3 goes low when the card is fully drawn in.

#define PIN_SWITCH PINB
#define BIT_SWITCH1 PB0
#define BIT_SWITCH2 PB1
#define BIT_SWITCH3 PB2

#define LOAD_OK 0
#define LOAD_ERROR 1
#define LOAD_LONG 2
#define LOAD_SHORT 3

TimerControlBlock   pollTimer, timeoutTimer;             // Declare the control blocks needed for timers



void chipdrive_init(){
	PORT_MOTOR |= (1<<BIT_MOTOR_IN)|(1<<BIT_MOTOR_OUT);
	DDR_MOTOR |= (1<<BIT_MOTOR_IN)|(1<<BIT_MOTOR_OUT);
}


//wait for card to be inserted and load it
u08 chipdrive_load(){
	u08 state, oldstate = 0, c;
	//wait until a card is inserted
	while(1){
		state = PIN_SWITCH & ((1<<BIT_SWITCH1)|(1<<BIT_SWITCH2));
		//switch 1 or 2 was released, so a card was drawn out
		if(state > oldstate) oldstate = state;
		//switch 1 or 2 was pressed, so a card was pushed in
		if((state < oldstate) || (state == 0)) break;
		AvrXDelay(&pollTimer, 50);
	}
	//wait a little, so user has pushed card to first roll
	AvrXDelay(&pollTimer, 50);
	
	//begin drawing in card
	PORT_MOTOR &= ~(1<<BIT_MOTOR_IN);
	
	//is the second switch activated fast enough?
	c=0;
	while(PIN_SWITCH & (1<<BIT_SWITCH2)){
		AvrXDelay(&pollTimer, 10);
		if((c++)>50){
			//stop motor
			PORT_MOTOR |= (1<<BIT_MOTOR_IN);
			return LOAD_ERROR;
		}
	}
	
	//is the card drawn in fast enough?
	c=0;
	while(PIN_SWITCH & (1<<BIT_SWITCH3)){
		AvrXDelay(&pollTimer, 10);
		if((c++)>150){
			//stop motor
			PORT_MOTOR |= (1<<BIT_MOTOR_IN);
			return LOAD_ERROR;
		}
	}
	
	//o.k. - the card is in, so we can stop the motor
	PORT_MOTOR |= (1<<BIT_MOTOR_IN);
	
	//switch 1 is still pressed, so the card is to long
	if(!(PIN_SWITCH & (1<<BIT_SWITCH1))) return LOAD_LONG;
	
	//switch 2 is not pressed anymore, so the card is to short
	if(PIN_SWITCH & (1<<BIT_SWITCH2)) return LOAD_SHORT;
	
	return LOAD_OK;
}

u08 chipdrive_eject(){
	AvrXDelay(&pollTimer, 100);
	u08 c = 0;
	PORT_MOTOR &= ~(1<<BIT_MOTOR_OUT);
	while( !(PIN_SWITCH & (1<<BIT_SWITCH2)) ){
		AvrXDelay(&pollTimer, 50);
		if((c++)>40){
			//motor off
			PORT_MOTOR |= (1<<BIT_MOTOR_OUT);
			return LOAD_ERROR;
		}
	}
	//wait a little until the card is really out
	AvrXDelay(&pollTimer, 50);
	PORT_MOTOR |= (1<<BIT_MOTOR_OUT);
	return LOAD_OK;
}


AVRX_GCC_TASKDEF(reader, 50, 3)
{
	u08 error_count = 0;
	chipdrive_init();
	if ( (PIN_SWITCH & (1<<BIT_SWITCH2)) == 0) {
		//switch 2 pressed, so card is in drive
		chipdrive_eject();
	};
	
	while(1){
		if (chipdrive_load() != LOAD_OK){
			error_count ++;
		}else{
			// do stuff here
			PORTB |=8;
			while (PINB & 8);
		}

		if (chipdrive_eject() != LOAD_OK){
			error_count++;
			if (error_count > 2){
				AvrXDelay(&pollTimer, 30000);
			}
		}else{
			//only reset errors on correct eject, because
			//on correct load the card could still be always
			//jammed when ejecting.
			error_count = 0;
		}
		
	}
}
