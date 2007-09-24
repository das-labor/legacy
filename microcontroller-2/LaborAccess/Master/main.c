
#include <stdio.h>
#include <avr/io.h>

#include "avrx.h"               // AvrX System calls/data structures
#include "AvrXSerialIo.h"
#include "uart_sw.h"
#include "dummy_qport.h"
#include "i2csw.h"
#include "xlap.h"
#include "console.h"
#include "door.h"

#define TICKRATE 1000
#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define TMC8_CK256 (1<<CS02)


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);    // Reload the timer counter
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
	
    
	AvrXRunTask(TCB(server1));
	AvrXRunTask(TCB(qport));
	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(door));
//	AvrXRunTask(TCB(console_task));
	
	
	InitSerial0(BAUD(4800));
	InitSerial1();

	xlap_init();
	
    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	
	//put_c0('h');
	//put_c1('H');
	
	fdevopen(stream_putc,0);
	//fdevopen(put_c1,0);
	
	i2cInit();

	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
};
