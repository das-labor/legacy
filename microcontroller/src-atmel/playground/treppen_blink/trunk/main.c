/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
//#include <util/delay.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "bewegung.h"
#include "led.h"


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

	MCUCR |= _BV(SE);      	    // Enable "sleep" mode (low power when idle)
	TCNT0 |= TCNT0_INIT;		    // Load overflow counter of timer0
	TCCR0 |= TMC8_CK256;		// Set Timer0 to CPUCLK/256
//	TCCR0B |= TMC8_CK256;		  // set timer0 atmega644p
	TIMSK |= _BV(TOIE0);		  // Enable interrupt flag
//	TIMSK0 |= _BV(TOIE0);		  // Enable interrupt flag atmega644p

	DDRD |= _BV(DATA) | _BV(CLK);
	PORTD |= _BV(DATA) | _BV(CLK);



	xlap_init();

	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(bewegung));
	//AvrXRunTask(TCB(led));


	Epilog();                   // Switch from AvrX Stack to first task
	while (1);
};
