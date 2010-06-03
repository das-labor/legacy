// Keyboard communication routines

#ifndef __KB_INCLUDED
#define __KB_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

#define ISC00 0
#define ISC01 1

// Keyboard konnections
#define PORT_KB PIND
#define CLOCK   PIND2 //Connected to int0, don't change
#define DATAPIN PIND4

void init_kb(void);

// getchar with blocking
int   getcharkb(void);

// getchar without blocking, returns 0, if no char is availlable
int getcharx(void);
#endif

