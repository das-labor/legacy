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

struct t_counter_status timing_counter = { 0,0,0,0,0 };

ISR(INT1_vect)
{
	cli();
	timing_counter.tastercounter_vortrag++;
	sei();
}

ISR(INT2_vect)
{
	cli();
	timing_counter.tastercounter_lounge++;
	sei();
}

//ISR(TIMER1_OVF_vect)
void taster()
{
	
	//	andere interrupts aus!
	
//	cli();
	 
	//	 alle 9.8 ticks sollte sollte was vom schalter kommen
	//	 also sagen wir, dass wenn sich nach 32ticks nichts am schalter
	//	 getan hat, dann wurde er los gelassen. Wir koennen den 
	//	 Counter fuer die eingaben auf null setzen
	
	if ((timing_counter.tickscounter & 0x001F) == 0)
	{ // alle 32 ticks ... 0.032 sekunden
		if (timing_counter.tastercounter_vortrag != 0)
		{
			if (timing_counter.tastercounter_vortrag == timing_counter.tastercounter_vortrag_last)
			{
				 
				//	was soll passieren wenn der schlater losgelassen wurde
				//	in erster linie sicher ein Rest
				
				itr_schalter_vortrag_statisch();
				timing_counter.tastercounter_vortrag = 0;
				timing_counter.tastercounter_vortrag_last = 0;
			} else
			{

				itr_schalter_vortrag_dynamisch();
				
				//	und wir zaehlen natuerlich weiter
				
				timing_counter.tastercounter_vortrag_last = timing_counter.tastercounter_vortrag;
				
				//	der schalter wird noch gedrueckt. wir haben also einen 
				//	dynamischen Bereich
				
				//				itr_schalter_vortrag_dynamisch();
			}
		}
		if ( timing_counter.tastercounter_lounge != 0)
		{
			if ( timing_counter.tastercounter_lounge == timing_counter.tastercounter_lounge_last)
			{
				 
				//	 keine aenderung festgestellt folglich call to set fur lounge 
				
				itr_schalter_lounge_statisch();
				timing_counter.tastercounter_lounge = 0;
				timing_counter.tastercounter_lounge_last = 0;
			} else
			{
				itr_schalter_lounge_dynamisch();
				timing_counter.tastercounter_lounge_last = timing_counter.tastercounter_lounge;
			}
		}
	}

	if ( (timing_counter.tickscounter & 0x03FF) == 0) // alle 1024 ticks ... ca 1sec
		{}

	if ( (timing_counter.tickscounter & 0x14FF) == 0) // alle 5120 ticks ... ca 5sec
		{}

	 
	//	 ueberlaeufe sind ok!
	
	timing_counter.tickscounter++;
	
		und alle interrupts wieder auf go!
	
//	sei();
}

AVRX_SIGINT(SIG_OVERFLOW0)
{
	IntProlog();                // Save interrupted context, switch stacks
	TCNT0 = TCNT0_INIT;			// Reload the timer counter
	AvrXTimerHandler();         // Process Timer queue
	taster();
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

	DDRA |= _BV(PA2) | _BV(PA3) | _BV(PA4); // Status LED G R B
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge HS, rcd

	DDRB &= ~_BV(PB2); // Eingang 
	DDRD &= ~_BV(PD3); // Eingang 
	PORTB |= _BV(PB2); // Pullup Taster vortrag
	PORTD |= _BV(PD3); // Pullup Taster lounge
/*
	MCUCR |= _BV(ISC11);							// Trigger Interrupt on any logical change on pin pd2
	GICR |= _BV(INT1) | _BV(INT2);		// Enable External Interrupt Request 1 / 2
	MCUCSR &= ~_BV(ISC2);
	
	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11);	// FastPWM bit 2, clk/64
	
//	TIMSK |= _BV(TOIE0);							// Enable Timer0 Overflow Interrupt
*/

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
	AvrXRunTask(TCB(i2ccom_in));
	AvrXRunTask(TCB(i2ccom_out));

	//	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(switchtask));
	AvrXRunTask(TCB(led));
	
	/* Needed for EEPROM access in monitor */
	//AvrXSetSemaphore(&EEPromMutex);

	Epilog();                   // Switch from AvrX Stack to first task
	while (1);
};
