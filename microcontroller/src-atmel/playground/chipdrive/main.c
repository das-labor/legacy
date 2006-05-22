#include <avr/io.h>

#include "console.h"

#define PORT_MOTOR PORTC
#define DDR_MOTOR DDRC
#define BIT_MOTOR_IN PC2
#define BIT_MOTOR_OUT PC3

#define PIN_SWITCH PINB
#define BIT_SWITCH1 PB0
#define BIT_SWITCH2 PB1
#define BIT_SWITCH3 PB2


void chipdrive_init(){
	PORT_MOTOR |= (1<<BIT_MOTOR_IN)|(1<<BIT_MOTOR_OUT);
	DDR_MOTOR |= (1<<BIT_MOTOR_IN)|(1<<BIT_MOTOR_OUT);
}

void chipdrive_load(){
	PORT_MOTOR &= ~(1<<BIT_MOTOR_IN);
	while(PIN_SWITCH & (1<<BIT_SWITCH3));
	PORT_MOTOR |= (1<<BIT_MOTOR_IN);
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
