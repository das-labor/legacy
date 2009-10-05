
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "switch.h"
//#include "licht_schalter.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor





AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

ISR(TIMER2_OVF_vect)
{
	/*
		andere interrupts aus!
	*/
	cli();
	/* 
		 alle 9.8 ticks sollte sollte was vom schalter kommen
		 also sagen wir, dass wenn sich nach 32ticks nichts am schalter
		 getan hat, dann wurde er los gelassen. Wir koennen den 
		 Counter fuer die eingaben auf null setzen
	*/
	if (!(PINB & _BV(PB1)))
			counter++;
	if ((timing_counter.tickscounter & 0x001F) == 0) { // alle 32 ticks ... 0.032 sekunden
		if (timing_counter.tastercounter_vortrag != 0) {
			if (timing_counter.tastercounter_vortrag == timing_counter.tastercounter_vortrag_last) {
				/* 
					was soll passieren wenn der schlater losgelassen wurde
					in erster linie sicher ein Rest
				*/
				itr_schalter_vortrag_statisch();
				timing_counter.tastercounter_vortrag = 0;
				timing_counter.tastercounter_vortrag_last = 0;
			} else {

				itr_schalter_vortrag_dynamisch();
				/*
					und wir zaehlen natuerlich weiter
				*/
				timing_counter.tastercounter_vortrag_last = timing_counter.tastercounter_vortrag;
				/*
					der schalter wird noch gedrueckt. wir haben also einen 
					dynamischen Bereich
				*/
				//				itr_schalter_vortrag_dynamisch();
			}
		}
	}

	if ( (timing_counter.tickscounter & 0x03FF) == 0) // alle 1024 ticks ... ca 1sec
		{}

	if ( (timing_counter.tickscounter & 0x14FF) == 0) // alle 5120 ticks ... ca 5sec
		{}

	/* 
		 ueberlaeufe sind ok!
	*/
	timing_counter.tickscounter++;
	/*
		und alle interrupts wieder auf go!
	*/
	sei();
}


int main(void)
{
    AvrXSetKernelStack(0);

    TCCR2 |= _BV(CS22) | _BV(WGM21) | _BV(WGM20) | _BV(COM21);	// clk/64
    TIMSK |= _BV(TOIE2);		// enable timer overflow int

    MCUCR = _BV(SE);      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = _BV(TOIE0);		// Enable interrupt flag

    // LEDS Küchenbutton blau grün rot
    DDRC |=  _BV(PC5) | _BV(PC4) | _BV(PC3);
    PORTC &= ~(_BV(PC4) | _BV(PC3));
    PORTC |= _BV(PC5);

    //Button Küchenlicht
    DDRB &= ~_BV(PB1);
    PORTB |= _BV(PB1);


    //PD 5,6,7 LEDs
    DDRD |= _BV(PD5) | _BV(PD6) | _BV(PD7);
    PORTD &= ~(_BV(PD5) | _BV(PD6) | _BV(PD7));

    //PC0 Button
    DDRC &= ~_BV(PC0);
    PORTC |= _BV(PC0);

    PORTD |= _BV(PD7);

	
	
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	xlap_init();
    //AvrXRunTask(TCB(Monitor));
	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(switchtask));
//	AvrXRunTask(TCB(licht_task));
	

    /* Needed for EEPROM access in monitor */
//	AvrXSetSemaphore(&EEPromMutex);
	

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
