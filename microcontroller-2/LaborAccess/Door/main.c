
#include <avr/io.h>
#include <stdlib.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures



#include "config.h"
#include "serial.h"

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

AVRX_GCC_TASKDEF(MotorCtl, 20, 0)       
{
	TimerControlBlock timer;
//	uint8_t  step_codes[] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
	uint8_t  step_codes[] = { 0x03, 0x06, 0x0c, 0x09 };
	uint8_t  step     = 0;
	int8_t   step_dir = 0;
	uint8_t  state = STATE_UNKNOWN;
	int8_t   val; //komisches zeug??
	uint8_t  ls_state, i;
	uint16_t bright;
	
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
		print_state(state);

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
		default:
			SerialPutStr("DEFAULT");
			step_dir = 0;
		};

		if (step_dir) {
			step = step + step_dir; step &= 0x03;
			steps_remaining--;

			val = 0;
			val |= (step_codes[step] & 0x01) ? 0x80:0x00;
			val |= (step_codes[step] & 0x02) ? 0x40:0x00;
			val |= (step_codes[step] & 0x04) ? 0x08:0x00;
			val |= (step_codes[step] & 0x08) ? 0x02:0x00;
			PORTC = val;
		} else {
			PORTC = 0;
		}
		edge = 0;

		// check cmds
		if (PINB & 0x08) 
			btn_open = 0;
		else
			btn_open = 1;

		if (PINB & 0x04) 
			btn_close = 0;
		else
			btn_close = 1;

		// Check LS
		ADCSRA |= (1<<ADIF);
		ADCSRA |= (1<<ADSC);
		AvrXDelay(&timer, 15); 
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


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flag

    DDRC = 0xFF;
    DDRB = 0x00;
    PORTB= 0x0F;

    ADMUX = 0xc0;
    ADCSRA = (1<<ADEN) | 7;//ADC aktivieren, Takt/128
	
    SerialInit(UBRR_INIT);  // Initialize USART baud rate generator
    AvrXRunTask(TCB(MotorCtl));

    /* Needed for EEPROM access in monitor */
    AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};

