
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart/uart.h"
#include "../lib/com/com.h"
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

uint8_t command_auto;
uint8_t command_zuenden;
uint8_t command_pumpe;
uint8_t command_power_psu;
uint8_t command_bruecken;
uint8_t command_dump;
uint8_t command_fire;

uint8_t state_zuenden;

uint8_t state_500V_psu;
uint8_t command_500V_psu;

uint8_t command_simmer_psu;
uint8_t state_simmer_psu;

uint8_t simmer_control_error;


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


/*
//Command byte 1:
#define MSK_MAIN_CMD       0x01
#define MSK_AUTO_CMD       0x02
#define MSK_500V_PSU_CMD   0x08
#define MSK_ZUEND_CMD      0x10
#define MSK_SIMMER_CMD     0x20

//Command byte 2:
#define MSK_PUMPE_CMD      0x01
#define MSK_POWER_CMD      0x02
#define MSK_BRUECKEN_CMD   0x04
#define MSK_DUMP_CMD       0x08
#define MSK_FIRE_CMD       0x10

//Simmer Slave response byte
#define MSK_500V_PSU_STATE 0x08
#define MSK_ZUEND_STATE    0x10
#define MSK_SIMMER_STATE   0x20
*/


#define NUM_SIMMER_VALUES 2

uint16_t simmer_values[2];

uint16_t simmer_i_soll;

#define SIMMER_VALUE_U 0
#define SIMMER_VALUE_I 1



void master_com(){
  static uint8_t simmer_poll_num;  
  uint8_t cmd1, cmd2;
  cmd1 = (state_main_on      ? MSK_MAIN_CMD     : 0)
        |(command_auto       ? MSK_AUTO_CMD     : 0)
        |(command_500V_psu   ? MSK_500V_PSU_CMD : 0)
        |(command_zuenden    ? MSK_ZUEND_CMD    : 0)
        |(command_simmer_psu ? MSK_SIMMER_CMD   : 0) ;
  
  cmd2 = (command_pumpe      ? MSK_PUMPE_CMD    : 0)
        |(command_power_psu  ? MSK_POWER_CMD    : 0)
        |(command_bruecken   ? MSK_BRUECKEN_CMD : 0)
        |(command_dump       ? MSK_DUMP_CMD     : 0)
        |(command_fire       ? MSK_FIRE_CMD     : 0) ;
  
  
  uart_putc(0x42); // start mark
  uart_putc(cmd1);
  uart_putc(cmd2);
  
  uint8_t chk1 = ~cmd1;
  uint8_t chk2 = ~cmd2;
  
  uart_putc(chk1);
  uart_putc(chk2);
  
  //slave request 1
  uart_putc(0x10 | simmer_poll_num);
  
  uart_putc(simmer_i_soll & 0xff);
  uart_putc(simmer_i_soll >> 8);
  
  uint16_t timeout = 0;
  uint8_t c[3], num = 0, success;
  
  do{
    if(uart_getc_nb((char*)&c[num])){
      num ++;
      if(num == 3){
        success = 1;
        break;
      }
    }
    _delay_ms(1);
    timeout++;
  }while(timeout < 10);
  
  if(success){
    simmer_control_error = 0;
    uint8_t stat = c[0];
    
    state_500V_psu    = (stat & MSK_500V_PSU_STATE) ? 1:0;
    state_zuenden     = (stat & MSK_ZUEND_STATE )   ? 1:0;
    state_simmer_psu  = (stat & MSK_SIMMER_STATE)   ? 1:0;
     
    simmer_values[simmer_poll_num] = *(uint16_t *)&c[1];
    simmer_poll_num++;
    if(simmer_poll_num == NUM_SIMMER_VALUES){
      simmer_poll_num = 0;
    }
  }else{
    simmer_control_error = 1;
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
	uart_init();

	sei();
	
	//test_keys_and_display();

	while(1){
		uint8_t x;
		statemachine();
		master_com();
		display_states();

		_delay_ms(50);
				
	}
}
