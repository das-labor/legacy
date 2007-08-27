/* 

 Timer-Interrupt routines to reduce overhead in interrupt
 
*/

 #include <avr/io.h>
 #include "hardware.h"
 
 #define	temp 	R26
 #define 	temp1	R27
 #define 	temp2	R30

// globals...
//extern unsigned long    	timer_value;
 .comm	timer_value, 4

#ifdef TXEN_CHECK 
 .comm	txencheck, 1	; nicht global, initialisierung egal
 .comm 	txenrestore, 1	; Wird bei jedem TXEN-Restore inkrementiert
#endif

// Interrupt auf das Timer0-Überlauf Ereigniss
// Wird genau 450 mal pro Sekunde aufgerufen und
// erhöht einen Software 32-Bit Timer.
// Dieser läuft dann halt nach etwa 3,5 Monaten über...
.global		SIG_OVERFLOW0
SIG_OVERFLOW0:
	; Alles mögliche sichern
	push	R2
	in		R2, _SFR_IO_ADDR(SREG)
	push	temp
	
#ifdef TXEN_CHECK
	lds		temp, txencheck
	sbis 	_SFR_IO_ADDR(L0_DRIVER_EN_PORT), L0_DRIVER_EN
	rjmp	TO_Weiter1		; Wenn nicht freigegeben, dann Zähler rücksetzen
	dec		temp
	brne	TO_Weiter2

	; Hier ist der Zähler zu weit gelaufen, Port rücksetzen
	cbi		_SFR_IO_ADDR(L0_DRIVER_EN_PORT), L0_DRIVER_EN
	sbi		_SFR_IO_ADDR(L0_DRIVER_EN_DDR),  L0_DRIVER_EN
	push	temp2
	lds		temp2, txenrestore
	inc 	temp2
	sts		txenrestore, temp2
	pop		temp2
	
TO_Weiter1:	
	ldi		temp, 255
	
TO_Weiter2:	
	sts		txencheck, temp
#endif	
	
	; Timer incrementieren
	lds		temp, (timer_value + 0)	; 0. Byte
	inc		temp
	sts		(timer_value + 0), temp
	brne	SO_RAUS
	
	lds		temp, (timer_value + 1)	; 1. Byte
	inc		temp
	sts		(timer_value + 1), temp
	brne	SO_RAUS
	
	lds		temp, (timer_value + 2)	; 2. Byte
	inc		temp
	sts		(timer_value + 2), temp
	brne	SO_RAUS

	lds		temp, (timer_value + 3)	; 3. Byte
	inc		temp
	sts		(timer_value + 3), temp
	brne	SO_RAUS
	
SO_RAUS:	

	; Alles mögliche zurücksichern
	pop		temp 
	out		_SFR_IO_ADDR(SREG), R2
	pop		R2
	
	reti
	
	
// Gibt den aktuellen Timerwert zurück
//
// unsigned long timer_getvalue( void )
// Benutzt: temp (und indirekt R22-R25)
.global		timer_getvalue
timer_getvalue:
	in		temp, _SFR_IO_ADDR(SREG)
	cli
	
	lds		R22, (timer_value + 0)
	lds		R23, (timer_value + 1)
	lds		R24, (timer_value + 2)
	lds		R25, (timer_value + 3)
	
	out		_SFR_IO_ADDR(SREG), temp
	ret
	
// unsigned long timer_elapsed_time( unsigned long first_timepoint, unsigned long second_timepoint)	
.global		timer_elapsed_time	
	
// EOF

