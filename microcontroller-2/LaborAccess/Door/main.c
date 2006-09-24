
#include <avr/io.h>
#include <stdlib.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures


#include "config.h"
#include "serial.h"

#include "MotorCtrl.h"


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
    AvrXRunTask(TCB(MotorCtrl));

    /* Needed for EEPROM access in monitor */
    AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
