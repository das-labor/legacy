
#include <avr/io.h>
#include "ioport.h"
#include "pinout.h"
#include "uart/uart.h"
#include "laser.h"
#include "stepper.h"

typedef struct{
	char name;
	uint16_t size;
	void * data;
	void (* before)();
	void (* updated)();
}setting_t;


uint8_t motor_command;
uint8_t laser_command;

int16_t stepper_cmd;
uint8_t stepper_command_flag; //this is set when giving a stepper command,
                              //and reset when the status is polled as soon
                              //as the stepper is ready. It is then also passed
                              //to the host, so this can recognize the difference
                              //between the stepper being ready before the command
                              //or after.

void motor_updated(){
	if(motor_command == '1'){
 		OUTPUT_OFF(MOTOR);
	} else {
		OUTPUT_ON(MOTOR); 
	}
}

void stepper_updated(){
	// 0  = go home
	// -1 = step forward
	// -2 = step backward
	// -3 = stop
	//positive values: go to that position
	
	if(stepper_cmd == 0){
		stepper_command = CMD_HOME;
	}else if(stepper_cmd == -1){
		stepper_dir = 1;
		stepper_command = CMD_STEP;
	}else if(stepper_cmd == -2){
		stepper_dir = 0;
		stepper_command = CMD_STEP;
	}else if(stepper_cmd == -3){
		stepper_command = 0;
	}else if(stepper_cmd > 0){
		position_request = stepper_cmd;
		stepper_command = CMD_GOTO;
	}
	
	stepper_command_flag = 1;
}

void get_status(){
	uint8_t s;
		
	s  = stepper_command    ? 0    : 0x01;
	s |= INPUT(MOTOR_READY) ? 0x02 : 0;
	s |= INPUT(DOOR_SWITCH) ? 0    : 0x04;
	
	if((stepper_command == 0) && (stepper_command_flag)){
		s |= 0x08;
		stepper_command_flag = 0;
	}

	
	uart_putc(s);
}

void before_data(){
	write_line_enable = 0;
	get_status();
}

void data_updated(){
	write_line_enable = 1;
}



setting_t settings[] = {
	{'m',   1, &motor_command,   0,           motor_updated   },
	{'l',   1, &laser_command,   0,           0               },
	{'d', 0xffff, data_table,       before_data, data_updated    },
	{'s',   2, &stepper_cmd,     0,           stepper_updated },
	{'S',   0, 0,                get_status,  0               },
};


static uint8_t com_state;
#define COM_IDLE   0
#define COM_DATA   1
#define COM_SIZE_L 2
#define COM_SIZE_H 3

void update_com(){
	uint8_t c;
	static void (*updated)();
	static uint8_t * data_p;
	static uint16_t data_size;
	static uint8_t my_laser_state;
	
	while(uart_getc_nb((char*)&c)){
		if(com_state == COM_IDLE){
			uint8_t x;
			uint8_t found = 0;
			for(x=0; x < (sizeof(settings) / sizeof(setting_t)); x++){
				if(c == settings[x].name){
					updated = settings[x].updated;
					data_size = settings[x].size;
					if(data_size != 0){
						com_state = COM_DATA;
					}
					data_p = settings[x].data;
					found = 1;
					if(settings[x].before){
						settings[x].before();
					}
					if(data_size == 0xffff){
						com_state = COM_SIZE_L;
					}
					break;
				}
			}
			if(!found){
				uart_putstr("Exposer\r\n");
			}
		}else if(com_state == COM_DATA){
			*data_p ++ = c;
			data_size --;
			if(data_size == 0){
				com_state = COM_IDLE;
				if(updated){
					updated();
				}
			}
		}else if(com_state == COM_SIZE_L){
			com_state = COM_SIZE_H;
			data_size = c;
		}else if(com_state == COM_SIZE_H){
			com_state = COM_DATA;
			data_size |= c<<8;
		}
	}
	
	if((laser_command == '1') && INPUT(MOTOR_READY)){
		if(my_laser_state == 0){
			my_laser_state = 1;
			laser_on();
		}
	}else{
		my_laser_state = 0;
		laser_off();
	}
}
