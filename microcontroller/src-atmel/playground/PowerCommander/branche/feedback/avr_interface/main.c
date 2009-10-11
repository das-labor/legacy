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
#include "switch.h"
#include "led.h"

#include "twi_master/twi_master.h"


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

	DDRA |= _BV(PA2) | _BV(PA3) | _BV(PA4); // Status LED R G B
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge HS, rcd
	//  PD7 PD6
	// PC2
	DDRB &= ~_BV(PB2); // Eingang 
	DDRD &= ~_BV(PD3); // Eingang 
	PORTB |= _BV(PB2); // Pullup Taster vortrag
	PORTD |= _BV(PD3); // Pullup Taster lounge
//	_delay_ms(1000);

/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}
	//InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	xlap_init();

	AvrXRunTask(TCB(cancom_in));
	AvrXRunTask(TCB(cancom_out));
	AvrXRunTask(TCB(cancom_out_info));
	AvrXRunTask(TCB(i2ccom_in));
	AvrXRunTask(TCB(i2ccom_out));

	//	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(switchtask));
	AvrXRunTask(TCB(switch_lounge));
	AvrXRunTask(TCB(switch_vortrag));
	AvrXRunTask(TCB(led));
	
	/* Needed for EEPROM access in monitor */
	//AvrXSetSemaphore(&EEPromMutex);

	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
};
