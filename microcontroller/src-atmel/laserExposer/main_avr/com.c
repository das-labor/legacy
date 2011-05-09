
#include <avr/io.h>
#include "ioport.h"
#include "pinout.h"
#include "uart/uart.h"
#include "laser.h"

void init_com(){
	uint16_t y;
	for(y=0;y<512;y++){
		delay_table[y] = 50;
	
	}
}


typedef struct{
	char name;
	uint8_t size;
	void * data;
	void (* updated)();
}setting_t;


uint8_t motor_command;
uint8_t laser_command;

void motor_updated(){
	if(motor_command){
 		OUTPUT_ON(MOTOR);
	} else {
		OUTPUT_OFF(MOTOR); 
	}
}

void stepper_updated(){
	stepper_forward();
}

setting_t settings[] = {
	{'m', 1, &motor_command, motor_updated },
	{'l', 1, &laser_command, 0 },
	{'d', 512, data_table, 0 },
	{'t', 512, delay_table, 0 },
	{'s', 1, stepper_command, stepper_updated },
};

/*
	uint8 c, v;
	uint16 x;
		switch(c){
			case 's':{
				v = uart_getc();
				if(v == 'f'){
					stepper_forward();
				}else{
					stepper_off();
				}
				
			}break;
			default:{
				UART_1_PutString("Exposer\r\n");
			}break;
			
		}
	}
*/	

static uint8_t com_state;
#define COM_IDLE 0
#define COM_DATA 1

void update_com(){
	uint8 c;
	static void (*updated)();
	static uint8_t * data_p;
	static uint16_t data_size;
	
	while(uart_getc_nb(&c)){
		if(com_state == COM_IDLE){
			uint8_t x;
			uint8_t found = 0;
			for(x=0; x < (sizeof(settings) / sizeof(setting_t)); x++){
				if(c == settings[x].name){
					updated = settings[x].updated;
					com_state = COM_DATA;
					data_size = settings[x].size;
					data_p = settings[x].data;
					found = 1;
					break;
				}
			}
			if(!found){
				uart_putstr("Exposer\r\n");
			}
		}else{
			*data_p ++ = c;
			data_size --;
			if(data_size == 0){
				com_state = COM_IDLE;
				if(updated){
					updated();
				}
			}
		}
	}
}
