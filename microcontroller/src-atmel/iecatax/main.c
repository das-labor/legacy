#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "avrx.h"


#include "iec_hw.h"
#include "iec.h"
#include "config.h"
#include "ata.h"
#include "dos.h"

void init_xram (void) __attribute__ ((naked)) \
    __attribute__ ((section (".init1")));

void init_xram (void)
{
	/* enable external SRAM */
	MCUCR |= (1<<SRE);
}


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



int main(void)
{
    AvrXSetKernelStack(0);

		
	DDRA = 0;
	PORTA = 0;  
	DDRB = 0x57;
	PORTB = 0x7f;
	DDRC = 0;
	PORTC = 0;
#ifdef ATA_ADDRESSLATCH	
	DDRD = 0xF8;
	PORTD = 0xF8;
#else
	DDRD = 0xFB;
	PORTD = 0xFB;
#endif
	DDRE = 0x06;
	PORTE = 0x04;
	
	/* setup interrupt */
  	sbi (MCUCR, ISC01);
  	cbi (MCUCR, ISC00);
  	sbi (GICR, INT0);
	
	MCUCR |= _BV(SE);
	TCNT0 = TCNT0_INIT;
	// Most other chips...  Note: some are TCCR0 and some are TCCR0B...
	TCCR0 =  (1<<CS02);

	//TIMSK |= _BV(TOIE0);

	TCCR2 = (1<<CS21);//clk/8

	iec_hw_init();
	
	LED_ON;
	error = INIT_ERROR;
	/* init submodules */
#ifdef ATA	
	if (ataInit()) {
		if (dosInit()) {
			error = VERSION_ERROR;
		}
	}
#endif
	
	LED_OFF;

	AvrXRunTask(TCB(iecAtnTask));
	AvrXRunTask(TCB(iec_task));
	//AvrXRunTask(TCB(led_task));

	Epilog();
	while(1);
}
