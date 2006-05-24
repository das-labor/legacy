
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
void chipdrive_load(){
	u08 state, oldstate = 0, c1;
	u16 c2;	
	//wait until a card is inserted
	while(1){
		state = PIN_SWITCH & ((1<<BIT_SWITCH1)|(1<<BIT_SWITCH2));
		//switch 1 or 2 was released, so a card was drawn out
		if(state > oldstate) oldstate = state;
		//switch 1 or 2 was pressed, so a card was pushed in
		if(state < oldstate) break;
		AvrXDelay(&pollTimer, 100);
	}
	//begin drawing in card
	PORT_MOTOR &= ~(1<<BIT_MOTOR_IN);
	
	//is the second switch activated fast enough?
	c1=0;
	while(PIN_SWITCH & (1<<BIT_SWITCH2)){
		AvrXDelay(&pollTimer, 100);
		if((c1++)>10) return LOAD_ERROR;
	}
	
	//is the card drawn in fast enough?
	c2=0;
	while(PIN_SWITCH & (1<<BIT_SWITCH3)){
		AvrXDelay(&pollTimer, 10);
		if((c2++)>200) return LOAD_ERROR;
	}
	
	//o.k. - the card is in, so we can stop the motor
	PORT_MOTOR |= (1<<BIT_MOTOR_IN);
	
	//switch 1 is still pressed, so the card is to long
	if(!(PIN_SWITCH & (1<<BIT_SWITCH1))) return LOAD_LONG;
	
	//switch 2 is not pressed anymore, so the card is to short
	if(PIN_SWITCH & (1<<BIT_SWITCH2)) return LOAD_SHORT;
	
	return LOAD_OK;
}

void chipdrive_eject(){
	PORT_MOTOR &= ~(1<<BIT_MOTOR_OUT);
	while( !(PIN_SWITCH & (1<<BIT_SWITCH2)) );
	PORT_MOTOR |= (1<<BIT_MOTOR_OUT);
}

#define CHIPDRIVE_SENSE_CARD !(PIN_SWITCH & (1<<BIT_SWITCH1))

int main(){
unsigned char card=0;

PORTB = 0x08;
chipdrive_init();
    console_init();

while(1){
if(CHIPDRIVE_SENSE_CARD){
chipdrive_load();
card = 1;
}
if( !(PINB & (1<<3)) ){
if(card){
card = 0;
chipdrive_eject();
while(CHIPDRIVE_SENSE_CARD);
}
}
console();
}
}



AVRX_GCC_TASKDEF(reader, 50, 3)
{
	chipdrive_init();
	
	while(1){
				
		
		
	}
}
