/*#######################################################################################
Derkodieren eines RC Summen Signals 
#######################################################################################*/

#ifndef _RC_H
#define _RC_H

#if defined (__AVR_ATmega32__)
#define TIMER_TEILER        CK64
#define TIMER_RELOAD_VALUE  250
#endif

#if defined (__AVR_ATmega644__)
//#define TIMER_TEILER        CK64
#define TIMER_RELOAD_VALUE  250
//#define TIMER_TEILER        CK256                 // bei 20MHz
//#define TIMER_RELOAD_VALUE  -78 					// bei 20MHz
#endif

#define GAS   PPM_in[2]


extern void rc_sum_init (void);

extern volatile int PPM_in[11];
extern volatile int PPM_diff[11];  // das diffenzierte Stick-Signal
extern volatile unsigned char NewPpmData;

#endif //_RC_H
