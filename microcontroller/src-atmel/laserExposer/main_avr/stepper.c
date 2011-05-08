

//Pinout
#define STEPPER_PORT PORTA
#define STEPPER_DDR  DDRA


uint8_t stepper_command;
#define CMD_STEP    1
#define CMD_RUN     2
#define CMD_HOME    3


#define STEPPER_OFF_DELAY 100

uint16_t stepper_delay;
uint16_t stepper_d;



uint8 stepper_sequence[4] = {1,4,2,8};
uint8 stepper_state;

void stepper_tick(){
	switch(stepper_command){
		case CMD_STEP:
			stepper_off_delay = STEPPER_OFF_DELAY;
			if(stepper_dir){
				stepper_state++;
				stepper_state %= 4;
			}else{
				stepper_state--;
				stepper_state %= 4;
			}
			stepper_command = 0;
			break;
		case CMD_HOME:
			stepper_dir = 0;
			stepper_run = 1;
			if(
			
			
	
	
	}
	
	if(stepper_run){
		if(stepper_d == 0){
			stepper_off_delay = STEPPER_OFF_DELAY;
			stepper_d = stepper_delay;
			if(stepper_dir){
				stepper_state++;
				stepper_state %= 4;
			}else{
				stepper_state--;
				stepper_state %= 4;
			}
		}
	}
	
	if(stepper_off_delay){
		stepper_off_delay --;
		STEPPER_PORT = stepper_sequence[stepper_state];
	}else{
		STEPPER_PORT = 0;
	}
}

