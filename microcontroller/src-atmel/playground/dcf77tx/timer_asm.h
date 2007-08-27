#ifndef TIMER_ASM_H
#define TIMER_ASM_H

#include "timer.h"
#include "hardware.h"

// Diesen Timer-Wert möglichst nicht anrührern, sondern nur mit
// timer_getvalue abfragen.
extern unsigned long    	timer_value;

// Gibt den aktuellen Timerwert zurück
unsigned long timer_getvalue( void );

// Gibt an, wie oft ein TXEN-Restore durchgeführt wurde
// Für TXEN-Restorefunkionalität muss in hardware.h "#define TXEN_CHECK 1" stehen
#ifdef TXEN_CHECK
extern unsigned char 		txenrestore;
#endif

#endif

// EOF

