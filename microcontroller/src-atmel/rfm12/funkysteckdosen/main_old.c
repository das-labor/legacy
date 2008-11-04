#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "serialio.h"           // From AvrX...

#include "config.h"
#include "rf.h"
#include "rfm12.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor



void my_init (void) __attribute__ ((naked)) \
    __attribute__ ((section (".init2")));

void
my_init (void)
{
	uint16_t x;
	for(x=0x60;x<0x460;x++)
		*(uint8_t*)x = 0x55;
}


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

TimerControlBlock tsttimer;
TimerControlBlock ticktimer;

AVRX_GCC_TASKDEF(testtransmit, 70, 7)
{
	//uint8_t txstr[4] = {0x00, 0x14, 0x05, 0x1d}; 
	uint8_t txstr[4] = {0x10, 0x00, 0x00, 0x40}; 
	while (42)
	{
		AvrXPutFifo(rftxfifo, txstr);
		AvrXDelay(&tsttimer, 200000);
	}
}

AVRX_GCC_TASKDEF(alttx, 100, 7)
{
	uint32_t i = 0;
	uint8_t mydata[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x92, 0x49, 0x24, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	while (23)
	{
		AvrXDelay(&tsttimer, 20000);
		rfm12_tx (sizeof(mydata), 0x00, &mydata);
		PORTD ^= _BV(PD6);
	}
}

AVRX_GCC_TASKDEF(ticktask, 300, 2)
{
	while (23)
	{
		rfm12_tick();
		AvrXDelay(&ticktimer, 2);
		PORTD ^= _BV(PD5);
	}

}
int main(void)
{
//    AvrXSetKernelStack(0);
/*
    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flago
*/
    DDRD |= _BV(PD6) | _BV(PD5);


/*	
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	//AvrXRunTask(TCB(rfrxtask));
//	AvrXRunTask(TCB(rftxtask));
//	AvrXRunTask(TCB(testtransmit));
//
	

	AvrXSetSemaphore(&EEPromMutex);
*/
    rfm12_init();
	

//    Epilog();                   // Switch from AvrX Stack to first task
	uint8_t mydata[] =
	{
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x92, 0x49, 0x24, 0x00,
		0x00, 0x00, 0x00, 0x00
	};
	uint32_t i = 0;

    while(1)
    {
		if (i & 0x3fff == 0)
		{
			rfm12_tx (sizeof(mydata), 0x00, &mydata);
			PORTD ^= _BV(PD6);
			i = 0;
		}
		rfm12_tick();
		i++;
    }
};
