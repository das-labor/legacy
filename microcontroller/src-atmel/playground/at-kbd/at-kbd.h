// at-kbd.h

#ifndef __AT_KBD__H__
#define __AT_KBD__H__

#include "config.h"

#ifndef F_CPU
 #define F_CPU 16000000UL // 16 MHz  
#endif
 #include <avr/delay.h>


/*
About the keyboard:
we use int2 for the clock so we wait for a falling edge and call.
*/


/* types */

typedef unsigned char byte;
typedef void(t_kbd_event_handler)(byte);

/* macros */

#define LOW 0
#define HIGH 1

#define EVEN_PARATY	0
#define ODD_PARATY	1
#define KBD_DATA_IN (((KBD_DATA_IN_PORT) & (1<<(KBD_DATA_BIT)))>>(KBD_DATA_BIT))

#define KBD_CLOCK_IN (((KBD_CLOCK_IN_PORT) & (1<<(KBD_CLOCK_BIT)))>>(KBD_CLOCK_BIT))


#define KBD_SET_DATA_HIGH()	{/*KBD_DATA_OUT_PORT |=  (1<<KBD_DATA_BIT);*/ \
							 KBD_DATA_DDR 	   &= ~(1<<KBD_DATA_BIT);} 

#define KBD_SET_DATA_LOW()	{/*KBD_DATA_OUT_PORT &= ~(1<<KBD_DATA_BIT);*/ \
							 KBD_DATA_DDR 	   |=  (1<<KBD_DATA_BIT);} 

#define KBD_SET_CLOCK_HIGH()	{/*KBD_CLOCK_OUT_PORT |=  (1<<KBD_CLOCK_BIT);*/ \
							  	 KBD_CLOCK_DDR 	    &= ~(1<<KBD_CLOCK_BIT);}
#define KBD_SET_CLOCK_LOW()		{/*KBD_CLOCK_OUT_PORT &= ~(1<<KBD_CLOCK_BIT);*/ \
								 KBD_CLOCK_DDR	    |=  (1<<KBD_CLOCK_BIT);} 


#define KBD_SET_DATA(x)  if((x)) KBD_SET_DATA_HIGH()  else KBD_SET_DATA_LOW()
#define KBD_SET_CLOCK(x) if((x)) KBD_SET_CLOCK_HIGH() else KBD_SET_CLOCK_LOW()

#define KBD_SET_CLOCK_INPUT()	KBD_SET_CLOCK_HIGH()
#define KBD_SET_DATA_INPUT()	KBD_SET_DATA_HIGH()




//c wait for falling edge on clock line
// #define KBD_WAIT_CLOCK_SIG() while(!KBD_CLOCK_IN); while(KBD_CLOCK_IN);
#define KBD_WAIT_SCAN_SIG() while(!KBD_CLOCK_IN); while(KBD_CLOCK_IN);
//c wait for riseing edge on clock line
#define KBD_WAIT_SET_SIG() while(KBD_CLOCK_IN); while(!KBD_CLOCK_IN);

#define KBD_BUFFER_MASK 0xff
#define KBD_BUFFER_SIZE (KBD_BUFFER_MASK+1) //c size of the ringbuffer in bytes must
#define KBD_BUFFER_UNUSED 0xff	//c the value used for initialisation of kbd_buffer



/* global Vars */

extern byte kbd_buffer[KBD_BUFFER_SIZE];
extern byte kbd_buffer_index; // zeigt auf das letzte element
extern t_kbd_event_handler	*kbd_event_handler;

/* functions */
void kbd_init();
void send_kbd_byte(byte data);

#endif

