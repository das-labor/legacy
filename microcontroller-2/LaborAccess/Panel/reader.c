
#include "reader.h"
#include "i2csw.h"
#include "client.h"

//Switch 1 goes low when a card is inserted.
//Goes high again when the card is at the end.
//
//Switch 2 goes low after the card passes the first roll.
//it stays low while the card is in.
//
//Switch 3 goes low when the card is fully drawn in.

#define PIN_SWITCH PINA
#define BIT_SWITCH1 PA5
#define BIT_SWITCH2 PA4
#define BIT_SWITCH3 PA3

#define SWITCH1 ((~PINA) & (1<<BIT_SWITCH1))
#define SWITCH2 ((~PINA) & (1<<BIT_SWITCH2))
#define SWITCH3 ((~PINA) & (1<<BIT_SWITCH3))


#define PORT_MOTOR PORTB
#define DDR_MOTOR DDRB
//dircetion the motor runs, if these Pins are pulled low
#define BIT_MOTOR_IN PB0
#define BIT_MOTOR_OUT PB1

//stop means electric braking, off means complete off
#define MOTOR_IN() PORT_MOTOR &= ~(1<<BIT_MOTOR_IN); PORT_MOTOR |= (1<<BIT_MOTOR_OUT)
#define MOTOR_OUT() PORT_MOTOR &= ~(1<<BIT_MOTOR_OUT); PORT_MOTOR |= (1<<BIT_MOTOR_IN)
#define MOTOR_STOP() PORT_MOTOR |= (1<<BIT_MOTOR_OUT); PORT_MOTOR |= (1<<BIT_MOTOR_IN)
#define MOTOR_OFF() PORT_MOTOR &= ~(1<<BIT_MOTOR_OUT); PORT_MOTOR &= ~(1<<BIT_MOTOR_IN)

#define LOAD_OK 0
#define LOAD_ERROR 1
#define LOAD_LONG 2
#define LOAD_SHORT 3

TimerControlBlock   pollTimer, timeoutTimer;             // Declare the control blocks needed for timers

MessageQueue ReaderMsgInQueue;

ReaderMsg_t ReaderMsgOut;


void chipdrive_init(){
	PORT_POWER |= (1<<BIT_POWER);//power to smartcard off
	DDR_POWER |= (1<<BIT_POWER);
	DDR_MOTOR |= (1<<BIT_MOTOR_IN);
	DDR_MOTOR |= (1<<BIT_MOTOR_OUT);
	MOTOR_OFF();
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
	MOTOR_IN();
	
	//is the second switch activated fast enough?
	c=0;
	while(!SWITCH2){
		AvrXDelay(&pollTimer, 10);
		if((c++)>50){
			goto load_error;
		}
	}
	
	//is the card drawn in fast enough?
	c=0;
	while(!SWITCH3){
		AvrXDelay(&pollTimer, 10);
		if((c++)>150){
			goto load_error;
		}
	}
	
	//o.k. - the card is in, so we can stop the motor
	MOTOR_STOP();
	
	//switch 1 is still pressed, so the card is to long
	if(SWITCH1) return LOAD_LONG;
	
	//switch 2 is not pressed anymore, so the card is to short
	if(!SWITCH2) return LOAD_SHORT;
	
	AvrXDelay(&pollTimer, 100);
	return LOAD_OK;

load_error:
	//turn motor off
	MOTOR_OFF();	
	return LOAD_ERROR;
}

u08 chipdrive_eject(){
	AvrXDelay(&pollTimer, 100);
	u08 c = 0;
	MOTOR_OUT();
	while( SWITCH2 ){
		AvrXDelay(&pollTimer, 50);
		if((c++)>40){
			MOTOR_OFF();		
			return LOAD_ERROR;
		}
	}
	//just let motor roll out, so card is pushed a little further.
	MOTOR_OFF();
	return LOAD_OK;
}

u08 chipdrive_capture(){
	AvrXDelay(&pollTimer, 100);
	u08 c = 0;
	MOTOR_IN();
	while( SWITCH3 ){
		AvrXDelay(&pollTimer, 50);
		if((c++)>40){
			MOTOR_OFF();		
			return LOAD_ERROR;
		}
	}
	//just let motor roll out, so card is pushed a little further.
	MOTOR_OFF();
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
			ReaderMsg_t *p;
			AvrXSendMessage(&ClientQueue, (MessageControlBlock*)&ReaderMsgOut);
			
			p = (ReaderMsg_t*)AvrXWaitMessage(&ReaderMsgInQueue);
			if(p->state == COMMAND_CAPTURE){
				chipdrive_capture();
			}
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
