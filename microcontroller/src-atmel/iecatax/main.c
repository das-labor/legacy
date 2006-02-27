#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "avrx.h"



#include "iec_hw.h"
#include "iec.h"



enum
{
	FALSE,
	TRUE
};

typedef unsigned char BOOL;

// Peripheral initialization

#define TICKRATE 1000
#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

/*
 Timer 0 Overflow Interrupt Handler

 Prototypical Interrupt handler:
 . Switch to kernel context
 . handle interrupt
 . switch back to interrupted context.
 */
AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Switch to kernel stack/context
    TCNT0 += TCNT0_INIT;		// Add to pre-load to account for any missed clocks
    AvrXTimerHandler();         // Call Time queue manager
    Epilog();                   // Return to tasks
}

// Super simple string printers...

// PutString from RAM
void myputs(int (*putch)(char), const uint8_t * psz)
{
	while (*psz != 0)
		(*putch)(*psz++);
}

// PutString from FLASH
void myputs_P(int (*putch)(char), const uint8_t * psz)
{
	while (__LPM(psz) != 0)
		(*putch)(__LPM(psz++));
}


// This task uses GCC Libc stdio facility and needs an additional 60-80 bytes of stack
// for processing the strings.  Longer strings probably need more stack.




int main(void)
{
    AvrXSetKernelStack(0);

	DDRA = 0;
	PORTA = 0;  
	DDRB = 0x57;
	PORTB = 0x57;
	DDRC = 0;
	PORTC = 0;
	DDRD = 0xFB;
	PORTD = 0xFB;
	DDRE = 0x04;
	PORTE = 0x04;
	
	/* setup interrupt */
  	sbi (MCUCR, ISC01);
  	cbi (MCUCR, ISC00);
  	sbi (GICR, INT0);
	
	MCUCR |= _BV(SE);
	TCNT0 = TCNT0_INIT;
	// Most other chips...  Note: some are TCCR0 and some are TCCR0B...
	TCCR0 =  (1<<CS02);

	TIMSK |= _BV(TOIE0);

	TCCR2 = (1<<CS21);//clk/8

	iec_hw_init();

	AvrXRunTask(TCB(iecAtnTask));
	AvrXRunTask(TCB(iec_task));

	Epilog();
	while(1);
}
