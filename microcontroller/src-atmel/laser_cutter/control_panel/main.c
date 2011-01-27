
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "borg_hw.h"
#include "pixel.h"
#include "7seg.h"


#define CTRL_MAIN_OFF        0
#define CTRL_MAIN_ON         1
#define CTRL_PUMPE           2
#define CTRL_500V_PSU_ON     3
#define CTRL_500V_PSU_OFF    4
#define CTRL_ZUENDEN          5
#define CTRL_SIMMER_PSU_ON  12
#define CTRL_SIMMER_PSU_OFF  7
#define CTRL_POWER_PSU_ON   11
#define CTRL_POWER_PSU_OFF   8
#define CTRL_BRUECKEN        15
#define CTRL_DUMP           13
#define CTRL_FIRE            9

#define CTRL_RESERVED_1     10
#define CTRL_RESERVED_2     14
#define CTRL_RESERVED_3      6


void test1(){
	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,1);
		}
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,0);
		}
	}

}


#define key_down(k) ((keys[((k)/4)] & (1<<((k)%4))) ? 0:1)


uint8_t state_main_on;

uint8_t state_500V_psu;
uint8_t command_500V_psu;

void statemachine_500v(){
	static uint8_t old_command;
	if(command
}


uint8_t command_simmer_psu;
uint8_t state_simmer_psu;



void statemachine(){
	if(key_down(CTRL_MAIN_ON)){
		state_main_on = 1;
	}
	if(key_down(CTRL_MAIN_OFF)){
		state_main_on = 0;
	}
	
	
	if(state_main_on){
	
		if(key_down(CTRL_500V_PSU_ON)){
			command_500V_psu = 1;
		}
		if(key_down(CTRL_500V_PSU_OFF)){
			command_500V_psu = 0;
		}
	
	}else{
		command_500V_psu = 0;
		command_simmer_psu = 0;
	}
}


void display_states(){
	set_led(CTRL_MAIN_ON,    state_main_on);
	set_led(CTRL_MAIN_OFF, ! state_main_on);
	

	set_led(CTRL_500V_PSU_ON,    state_500V_psu);


	if(state_main_on){
	
		set_led(CTRL_500V_PSU_OFF, ! state_500V_psu);
	}else{
		//Mute off LEDs when main is off
		set_led(CTRL_500V_PSU_OFF, 0);
	
	}
	
}


void test_keys_and_display(){
	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			//_delay_ms(100);
			uint8_t value = (keys[(x/4)] & (1<<(x%4))) ? 0:1;
			
			setpixel(x, value);
			
			if(value){
				int8_t sbuf[10];
				sprintf(sbuf, "%04d",x);
				seg_print(0, sbuf);
			}
			
		}
	}
}

int main(){
	
	borg_hw_init();

	sei();
	
	//test_keys_and_display();

	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			//_delay_ms(100);
		}
		statemachine();
		display_states();
	}

}