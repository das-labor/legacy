
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart/uart.h"
#include "uart1.h"
#include "../lib/com/com.h"
#include "borg_hw.h"
#include "pixel.h"
#include "7seg.h"

#define MAX_POWER_VOLTAGE 1400
#define MAX_FIRE_FREQUENCY 20

#define CTRL_MAIN_OFF        0
#define CTRL_MAIN_ON         1
#define CTRL_PUMPE           2
#define CTRL_500V_PSU_ON     3
#define CTRL_500V_PSU_OFF    4
#define CTRL_ZUENDEN         5
#define CTRL_SIMMER_PSU_ON  12
#define CTRL_SIMMER_PSU_OFF  7
#define CTRL_POWER_PSU_ON   11
#define CTRL_POWER_PSU_OFF   8
#define CTRL_CHARGE         15
#define CTRL_DUMP           13
#define CTRL_FIRE            9

#define CTRL_RESERVED_1     10
#define CTRL_RESERVED_2     14
#define CTRL_RESERVED_3      6

#define key_down(k) ((keys[((k)/4)] & (1<<((k)%4))) ? 0:1)


uint8_t state_main_on;

uint8_t command_auto;
uint8_t command_zuenden;

uint8_t command_pumpe;
uint8_t state_pumpe;

uint8_t command_power_psu;
uint8_t state_power_psu;

uint8_t command_bruecken;
uint8_t command_dump;
uint8_t command_fire;

uint8_t state_zuenden;

uint8_t state_500V_psu;
uint8_t command_500V_psu;

uint8_t command_simmer_psu;
uint8_t state_simmer_psu;

uint8_t command_charge;
uint8_t state_charge;

uint8_t command_fire;

uint16_t fire_frequency = 1;
uint16_t fire_period    = 1000;

uint8_t simmer_control_error;

uint8_t power_psu_error_bits;
uint8_t power_error;

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
		
		if(key_down(CTRL_SIMMER_PSU_ON)){
			command_simmer_psu = 1;
			command_pumpe = 1;
		}
		
		if(key_down(CTRL_SIMMER_PSU_OFF)){
			command_simmer_psu = 0;
		}
		
		if(key_down(CTRL_ZUENDEN)){
			command_zuenden = 1;
		}else{
			command_zuenden = 0;
		}
		
		if(key_down(CTRL_POWER_PSU_ON)){
			command_power_psu = 1;
		}
		
		if(key_down(CTRL_POWER_PSU_OFF)){
			command_power_psu = 0;
		}
		
		if(key_down(CTRL_CHARGE)){
			command_charge = 1;
		}else{
			command_charge = 0;
		}
		
		if(key_down(CTRL_FIRE)){
			command_fire = 1;
		}else{
			command_fire = 0;
		}
		
		
		
	}else{
		command_500V_psu = 0;
		command_simmer_psu = 0;
		command_zuenden = 0;
		command_power_psu = 0;
		command_charge = 0;
		command_fire = 0;
	}
	
	//Pumpe switch toggle
	static uint8_t old_pumpe_key_down;
	uint8_t pumpe_key_down = key_down(CTRL_PUMPE);
	if(pumpe_key_down & ! old_pumpe_key_down){
		command_pumpe ^= 1;
	}
	old_pumpe_key_down = pumpe_key_down;

}


void display_states(){
	set_led(CTRL_MAIN_ON,    state_main_on);
	set_led(CTRL_MAIN_OFF, ! state_main_on);
	
	set_led(CTRL_500V_PSU_ON,    state_500V_psu);
	set_led(CTRL_SIMMER_PSU_ON,  state_simmer_psu);
	set_led(CTRL_POWER_PSU_ON,   state_power_psu);

	set_led(CTRL_PUMPE,          state_pumpe);

	set_led(CTRL_ZUENDEN, state_zuenden);

	set_led(CTRL_CHARGE, state_charge);

	if(state_main_on){
		set_led(CTRL_500V_PSU_OFF,   ! state_500V_psu);
		set_led(CTRL_SIMMER_PSU_OFF, ! state_simmer_psu);
		set_led(CTRL_POWER_PSU_OFF,  ! state_power_psu);
		
	}else{
		//Mute off LEDs when main is off
		set_led(CTRL_500V_PSU_OFF, 0);
		set_led(CTRL_SIMMER_PSU_OFF, 0);
		set_led(CTRL_POWER_PSU_OFF, 0);
	
	}
	
}


int16_t simmer_i_ist;
int16_t simmer_u;

int16_t simmer_i_soll;

int16_t power_u_ist;

int16_t power_u_soll;

extern volatile int8_t enc_delta;

void update_menu(){
	uint8_t mkeys[3];
	static uint8_t keys_old[3];
	static uint8_t selected = 0xff, old_selected = 0xff;
	
	static uint16_t disp_value[3];
	static uint16_t disp_value_old[3] = {0xffff, 0xffff, 0xffff};
	
	
	mkeys[0] = key_down(CTRL_RESERVED_1);
	mkeys[1] = key_down(CTRL_RESERVED_2);
	mkeys[2] = key_down(CTRL_RESERVED_3);
	
	uint8_t x;
	uint8_t edge = 0xff;
	
	for(x=0; x<3; x++){
		if(mkeys[x] && ! keys_old[x]){
			edge = x;
		}
		keys_old[x] = mkeys[x];
	}
	
	if(edge != 0xff){
		if(selected == edge){
			selected = 0xff;
		}else{
			selected = edge;
		}
	}
	
	if(selected != old_selected){
		
		switch(selected){
			case 0:
				set_led(CTRL_RESERVED_1, 1);
				set_led(CTRL_RESERVED_2, 0);
				set_led(CTRL_RESERVED_3, 0);
				break;
			case 1:
				set_led(CTRL_RESERVED_1, 0);
				set_led(CTRL_RESERVED_2, 1);
				set_led(CTRL_RESERVED_3, 0);
				break;
			case 2:
				set_led(CTRL_RESERVED_1, 0);
				set_led(CTRL_RESERVED_2, 0);
				set_led(CTRL_RESERVED_3, 1);
				break;
			default:
				set_led(CTRL_RESERVED_1, 0);
				set_led(CTRL_RESERVED_2, 0);
				set_led(CTRL_RESERVED_3, 0);
				break;
		}
		
		old_selected = selected;
	}
	
	int8_t enc = encoder_read();
	
	if(selected == 0){
		simmer_i_soll += enc;
		if(simmer_i_soll > 250){
			simmer_i_soll = 250;
		}else if(simmer_i_soll < 0){
			simmer_i_soll = 0;
		}
		disp_value[0] = simmer_i_soll;
	}else{
		disp_value[0] = simmer_i_ist;
	}
	
	if(selected == 1){
		power_u_soll += enc * 5;
		if(power_u_soll > MAX_POWER_VOLTAGE){
			power_u_soll = MAX_POWER_VOLTAGE;
		}else if(power_u_soll < 0){
			power_u_soll = 0;
		}
		disp_value[1] = power_u_soll;
	}else{
		disp_value[1] = power_u_ist;
	}
	
	if(selected == 2){
		fire_frequency += enc;
		if(fire_frequency > MAX_FIRE_FREQUENCY){
			fire_frequency = MAX_FIRE_FREQUENCY;
		}else if(fire_frequency < 1){
			fire_frequency = 1;
		}
	}else{
	}
	
	disp_value[2] = fire_frequency;
	
	static uint16_t old_fire_frequency;
	if(fire_frequency != old_fire_frequency){
		fire_period = 1000 / fire_frequency;
	}
	
	for(x=0;x<3;x++){
		if(disp_value[x] != disp_value_old[x]){
			disp_value_old[x] = disp_value[x];
			char strbuf[5];
			sprintf(strbuf, "%4d", disp_value[x]);
			seg_print(x, strbuf);
		}
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


#define SIMMER_VALUE_U 0
#define SIMMER_VALUE_I 1


void master_com(){
    
  uint8_t cmd1, cmd2;
  cmd1 = (state_main_on      ? MSK_MAIN_CMD     : 0)
        |(command_auto       ? MSK_AUTO_CMD     : 0)
        |(command_500V_psu   ? MSK_500V_PSU_CMD : 0)
        |(command_zuenden    ? MSK_ZUEND_CMD    : 0)
        |(command_simmer_psu ? MSK_SIMMER_CMD   : 0) ;
  
  cmd2 = (command_pumpe      ? MSK_PUMPE_CMD    : 0)
        |(command_power_psu  ? MSK_POWER_PSU_CMD: 0)
        |(command_charge     ? MSK_CHARGE_CMD   : 0)
        |(command_dump       ? MSK_DUMP_CMD     : 0)
        |(command_fire       ? MSK_FIRE_CMD     : 0) ;
  
  
  uart_putc(0x42); // start mark
  uart_putc(cmd1);
  uart_putc(cmd2);
  
  uint8_t chk1 = ~cmd1;
  uint8_t chk2 = ~cmd2;
  
  uart_putc(chk1);
  uart_putc(chk2);

  uart_putc(simmer_i_soll & 0xff);
  uart_putc(simmer_i_soll >> 8);

  uart_putc(power_u_soll & 0xff);
  uart_putc(power_u_soll >> 8);
  
  uart_putc(fire_period & 0xff);
  uart_putc(fire_period >> 8);
  
  {
	  //slave request 1
	  static uint8_t simmer_poll_num;
	  #define NUM_SIMMER_VALUES 2

	  uart_putc(0x10 | simmer_poll_num);
	  
	  uint16_t timeout = 0;
	  uint8_t c[3], num = 0, success = 0;
	  
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
	    
		if(simmer_poll_num == 0){
			simmer_i_ist = *(uint16_t *)&c[1];
		}else if(simmer_poll_num == 1){
			simmer_u = *(uint16_t *)&c[1];
		}
		
	    simmer_poll_num++;
	    if(simmer_poll_num == NUM_SIMMER_VALUES){
	      simmer_poll_num = 0;
	    }
	  }else{
	    simmer_control_error = 1;
	  }
  }
  
  {
	  //slave request 2
	  static uint8_t power_poll_num;
	  #define NUM_POWER_VALUES 1

	  uart_putc(0x20 | power_poll_num);
	  
	  uint16_t timeout = 0;
	  uint8_t c[4], num = 0, success = 0;
	  
	  do{
	    if(uart_getc_nb((char*)&c[num])){
	      num ++;
	      if(num == 4){
	        success = 1;
	        break;
	      }
	    }
	    _delay_ms(1);
	    timeout++;
	  }while(timeout < 10);
	  
	  if(success){
	    power_error = 0;
	    uint8_t stat = c[0];
	    
	    state_power_psu   = (stat & MSK_POWER_PSU_STATE)  ? 1:0;
		state_pumpe       = (stat & MSK_PUMPE_STATE)  ? 1:0;
		state_charge      = (stat & MSK_CHARGE_STATE) ? 1:0;

		power_psu_error_bits = c[1];
	    
		if(power_poll_num == 0){
			power_u_ist = *(uint16_t *)&c[2];
		}else if(power_poll_num == 1){
			//
		}
		
	    power_poll_num++;
	    if(power_poll_num == NUM_POWER_VALUES){
	      power_poll_num = 0;
	    }
	  }else{
	    power_error = 1;
	  }
  }
}


int my_putc(char c, FILE * f){
	uart1_putc(c);
	return 0;
}

int main(){
	borg_hw_init();
	uart_init();
	uart1_init();

	fdevopen(my_putc, 0);
	
	printf("Hello World");

	sei();
	
	while(1){
		statemachine();
		master_com();
		display_states();
		update_menu();
				
	}
}
