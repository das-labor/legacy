
#include <avr/io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "server1.h"

#include "MotorCtrl.h"

#define LINESIZE 16

#define STATE_CLOSED   0
#define STATE_OPENING1 1
#define STATE_OPENING2 2
#define STATE_CLOSING1 3
#define STATE_CLOSING2 4
#define STATE_OPEN     5
#define STATE_ERROR    6
#define STATE_MESSED   7
#define STATE_UNKNOWN  8

#define STEPS_PER_CYCLE 800;

#define LIGHT_SENSOR_THRESHOLD 511

uint16_t steps_remaining = STEPS_PER_CYCLE;
uint8_t btn_open    = 0;
uint8_t btn_close   = 0;
uint8_t btn_recover = 0;
int8_t  edge        = 0;


#ifdef MOTORCTRL_DEBUG
void print_state(uint8_t state)
{
	static uint8_t old_state=0xff;
	char buf[50];

	if (old_state == state) return;
	old_state = state;

	// itoa(steps_remaining, buf, 10);
	// SerialPutStr( buf );
	// SerialPutStr(" : ");

	switch(state) {
	case STATE_CLOSED:
		SerialPutStr("STATE_CLOSED\n");
		break;
	case STATE_OPENING1:
		SerialPutStr("STATE_OPENING1\n");
		break;
	case STATE_OPENING2:
		SerialPutStr("STATE_OPENING2\n");
		break;
	case STATE_CLOSING1:
		SerialPutStr("STATE_CLOSING1\n");
		break;
	case STATE_CLOSING2:
		SerialPutStr("STATE_CLOSING2\n");
		break;
	case STATE_OPEN:
		SerialPutStr("STATE_OPEN\n");
		break;
	case STATE_ERROR:
		SerialPutStr("STATE_ERROR\n");
		break;
	case STATE_MESSED:
		SerialPutStr("STATE_MESSED\n");
		break;
	case STATE_UNKNOWN:
		SerialPutStr("STATE_UNKNOWN\n");
		break;
	default:
		SerialPutStr("Huh, dont know state\n" );
	}
}
#endif //MOTORCTRL_DEBUG


#define STEPPER_MASK 0x3C
#define BIT_LED PA7	

void MotorCtrl_init(){
	//set pins for stepper to output
    DDRC |= STEPPER_MASK;
	
	PORTB |= 0x03;//pullups on buttons on
	
	PORTA |= (1<<BIT_LED);
	DDRA |= (1<<BIT_LED);

    ADMUX = 0xc1;
    ADCSRA = (1<<ADEN) | 7;//ADC aktivieren, Takt/128		
}

AVRX_GCC_TASKDEF(MotorCtrl, 20, 0)       
{
	TimerControlBlock timer;
//	uint8_t  step_codes[] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
//	uint8_t  step_codes[] = { 0x03, 0x06, 0x0c, 0x09 };
	uint8_t  step_codes[] = { 0x24, 0x30, 0x18, 0x0C };
	
	uint8_t  step     = 0;
	int8_t   step_dir = 0;
	uint8_t  state = STATE_UNKNOWN;
	//int8_t   val; //komisches zeug?? : Pinbelgung umwürfeln?!? wird ja immer komischer.....
	uint8_t  ls_state, i;
	uint16_t bright;
	
	MotorCtrl_init();
	
	// Initialize ls_state convert from AD-Converter
	for(i=0; i<10; i++) {
		ADCSRA |= (1<<ADIF);//Interrupt Flag zurücksetzen
		ADCSRA |= (1<<ADSC);//Einen Konvertierungsvorgang starten
		while( (ADCSRA & (1<<ADIF)) == 0  );
		bright = ADC;
	};
	
	if (bright>LIGHT_SENSOR_THRESHOLD) 
		ls_state=0;
	else 
		ls_state=1;

	// Run state machine
	for(;;) {
#		ifdef MOTORCTRL_DEBUG
		print_state(state);
#		endif // MOTORCTRL_DEBUG

		// State machine
		switch(state) {
		case STATE_OPEN:
			if (btn_close) {
				state = STATE_CLOSING1;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir=-1;
			} else if (edge) {
				state = STATE_MESSED;
			}
			break;
		case STATE_CLOSING1:
			if (edge==-1) {
				state = STATE_CLOSING2;
				step_dir = -1;
				steps_remaining = STEPS_PER_CYCLE;
			} else if (!steps_remaining) {
				state = STATE_OPENING2;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = 1;
			}
			break;
		case STATE_CLOSING2:
			if (edge==-1) {
				state = STATE_CLOSED;
				step_dir = 0;
			} else if (!steps_remaining) {
				state = STATE_OPENING1;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = 1;
			}
			break;
		case STATE_OPENING1:
			if (edge==-1) {
				state = STATE_OPENING2;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = 1;
			} else if (!steps_remaining) {
				state = STATE_ERROR;
				step_dir = 0;
			}
			break;
		case STATE_OPENING2:
			if (edge==-1) {
				step_dir = 0;
				state = STATE_OPEN;
			} else if (!steps_remaining) {
				state = STATE_ERROR;
				step_dir = 0;
			}
			break;
		case STATE_CLOSED:
			if (btn_open) {
				state = STATE_OPENING1;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = 1;
			} else if (!steps_remaining) {
				state = STATE_MESSED;
			}
			break;
		case STATE_MESSED:
			step_dir = 0;
			break;
		case STATE_UNKNOWN:
			step_dir = 0;
			if (btn_open) {
				state = STATE_OPENING1;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = 1;
			} else if (btn_close) {
				state = STATE_CLOSING1;
				steps_remaining = STEPS_PER_CYCLE;
				step_dir = -1;
			} else if (edge) {
				state = STATE_MESSED;
			}
			break;
		case STATE_ERROR:
			step_dir = 0; 
			break;
		};

		PORTC &= ~STEPPER_MASK;
			
		if (step_dir) {
			step = step + step_dir; step &= 0x03;
			steps_remaining--;

			PORTC |= step_codes[step];
			
		} else {
			
		}
		edge = 0;

		
		// check cmds
/*
		if (doorstate & (STATE_DOOR_DOWNSTAIRS)){
			btn_open = 1;
			btn_close = 0;
		}else{
			btn_open = 0;
			btn_close = 1;
		}
*/
		
		if(PINB & 0x01)
			btn_open = 0;
		else
			btn_open = 1;
		
		if(PINB & 0x02)
			btn_close = 0;
		else
			btn_close = 1;
		
		
		
		
		// Check LS
		ADCSRA |= (1<<ADIF);
		ADCSRA |= (1<<ADSC);
		AvrXDelay(&timer, 7); 
		while( (ADCSRA & (1<<ADIF)) == 0  );
		bright = ADC;

		if (ls_state==0 && bright<200) {
			ls_state = 1;
			edge = 1;
		} else if (ls_state==1 && bright>800) {
			ls_state = 0;
			edge = -1;
		} else
			edge = 0;

	}
}
