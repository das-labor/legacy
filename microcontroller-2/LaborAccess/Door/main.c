
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "serial.h"

#define LINESIZE 16

#define IDLE    0
#define OPENING 1
#define CLOSING 2

#define RAISEING 0
#define FALLING  1

int8_t schranke_state;
int8_t motor_state;





AVRX_GCC_TASKDEF(MotorCtl, 20, 0)       
{
	uint8_t step = 0;
	uint8_t steps[] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
	uint8_t val;
	TimerControlBlock timer;

	motor_state = IDLE;
	AvrXDelay(&timer, 500);

	for(;;) {
		val = 0;
		val |= (steps[step] & 0x01) ? 0x00:0x80;
		val |= (steps[step] & 0x02) ? 0x00:0x40;
		val |= (steps[step] & 0x04) ? 0x00:0x08;
		val |= (steps[step] & 0x08) ? 0x00:0x04;
		AvrXDelay(&timer, 500);
	}
}

//AVRX_GCC_TASKDEF(SchrankeCtl, 20, 0)       
//{
//	TimerControlBlock timer;
//	uint16_t value;
//
//	schranke_state = 0;
//
//	for(;;) {
//		AvrXDelay(&timer, 500);
//		
//	};
//}



AVRX_GCC_TASKDEF(SerialComm, 20, 0)
{
};



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
	
    SerialInit(UBRR_INIT);  // Initialize USART baud rate generator
    AvrXRunTask(TCB(MotorCtl));

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};

