// Keyboard communication routines

#ifndef __KB_INCLUDED
#define __KB_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

//select 0 or 1
#define KB_USE_INT 1

#if KB_USE_INT == 1
#define CLOCK   PD3 //Connected to int1, don't change
#define DATAPIN PD2
#else
#define CLOCK   PD2 //Connected to int0, don't change
#define DATAPIN PD3
#endif
// Keyboard konnections
#define PORT_KB PIND
#define KB_EN_PULLUPS PORTD|=_BV(PD3)|_BV(PD2);
#define KB_SEL_DDR DDRD&=~(_BV(PD3)|_BV(PD2));
void init_kb(void);

// getchar with blocking
int   getcharkb(void);

// getchar without blocking, returns 0, if no char is availlable
unsigned char getcharx(void);
#endif

