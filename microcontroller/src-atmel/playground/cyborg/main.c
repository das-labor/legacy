
#include <avr/io.h>
#include <stdio.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "AvrXSerialIo.h"

#include "adc.h"
#include "servo.h"
#include "task1.h"


#define TICKRATE 1000
#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)
#define TMC8_CK256 (1<<CS02)


AVRX_SIGINT(SIG_OVERFLOW0)
{	
	IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
}

AVRX_EXTERNTASK(server1);

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flag
	
	AvrXRunTask(TCB(task1));
	AvrXRunTask(TCB(task2));
	
	InitSerial0(BAUD(9600));
	
	adc_init();

	servo_init();

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	
	fdevopen(put_char0, get_char0);
	
    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
