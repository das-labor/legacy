
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "mood.h"
#include "pwm.h"
#ifdef I2C
#include "i2cmaster.h"
#include "temp.h"
#endif

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

int main(void) {
	AvrXSetKernelStack(0);

	MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
	TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
	TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
	TIMSK = 1<<TOIE0;		// Enable interrupt flag

	DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5);
//	DDRD = 0xff;
	PORTD &= ~(1 << PD7) | ~(1 << PD6) | ~(1 << PD5);

	DDRC |= (1 << PC4) | (1 << PC5);
	PORTC |= (1 << PC4) | (1 << PC5);

//	PORTD = 0xff;
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

	init_pwm();
#ifdef I2C
	i2c_init();
#endif
	xlap_init();
	
    //AvrXRunTask(TCB(Monitor));
	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(mood));
	AvrXRunTask(TCB(pwmtask));
#ifdef I2C
	AvrXRunTask(TCB(temp));
#endif
    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);

	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
};
