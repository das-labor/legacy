// Timerroutinen// generiert einen Software 32-Bit Timer, der // mit 450 Hz (2,222..ms) @ 7.372800 Mhz "tickt".// Dazu wird der 8-Bit Hardwaretimer Timer0 verwendet.#include <avr/io.h>#include <avr/interrupt.h>#include "hardware.h"#include "timer.h"// Initialisiert das Timersystemvoid timer_init( void ){	// Timer0- und Output-Compare0 Interrupt abschalten	TIMSK &= ~(_BV(TOIE0)); // | _BV(OCIE0));	timer_value = (unsigned long)0;	// Softwaretimer auf 0 setzen	#if defined(__AVR_ATmega128__) 		TCCR0 = _BV(CS02);  	#else		TCCR0 = _BV(CS01) | _BV(CS00);	// clkIO/64 (From prescaler) => 450 Hz @ 7.372800 Mhz  	#endif	TCNT0 = 0;				// Hardwaretimer auf 0 setzen	TIFR |= _BV(TOV0);		// Timer0-Interruptflag l�schen	TIMSK |= _BV(TOIE0);	// Timer0-Interrupt aktivieren}/* Gibt die Zeitdifferenz zwischen zwei Timestamps zur�ck * Der zweite Wert sollte immer der vom sp�teren Zeitpunkt sein. * Ansonsten erh�lt man einen sehr gro�en falschen Wert. * Wenn aber der Timer zwischen den zwei Zeitpunkten * �bergelaufen ist, kommt trotzdem die richtige Zeitdifferenz herraus!	 */ inline unsigned long timer_diff_time( unsigned long first_timepoint, unsigned long second_timepoint){  return( second_timepoint - first_timepoint );}/*  Gibt true zur�ck, wenn die �bergebene Zeit abgelaufen ist.  *  Es muss ein auf 0 initialisiertes Handle �bergeben werden. *  Wenn true zur�ckgegeben wird, wird auch das Handle wieder auf 0 (=ung�ltig) gesetzt. *  Wenn die Funktion auf ein ung�ltiges Handle trifft, wird in dieses die aktuelle Zeit gespeichert *  und false zur�ck gegeben. So kann die Funktion immer wieder aufgerufen werden, ohne das das *  Handle neu initialisiert werden muss.	 */unsigned char timer_autodelay(unsigned long *handle, unsigned long time){	unsigned char retval=0;	if(!handle) return(0);
	if(*handle) {	    // Zeit abgelaufen?		if(timer_elapsed_time(*handle)>time) {			retval=!0;			*handle = 0;		}	} else {		// Handle ung�ltig, Zeitstempel ins Handle schreiben		*handle = timer_getvalue();		// F�r eine sehr seltene, aber m�gliche Fehlersituation...		if(!(*handle)) *handle++;	}	return(retval);}// EOF
