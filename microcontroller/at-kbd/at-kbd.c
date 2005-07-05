// at-kbd.c

#define __AT_KBD__C__


#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>

#include "at-kbd.h"

#define 	cli()   __asm__ __volatile__ ("cli" ::)
#define 	sei()   __asm__ __volatile__ ("sei" ::)

/* global Vars */

byte kbd_buffer[KBD_BUFFER_SIZE];
byte kbd_buffer_index=(KBD_BUFFER_SIZE-1); // zeigt auf das letzte element
t_kbd_event_handler	*kbd_event_handler=(void*)0;

//init KBD
/*
1) init kbd-buffer
2) set ports correct,
3) set counter correct

*/

void kbd_init(){
	int i;
	for (i=0; i<KBD_BUFFER_SIZE; ++i)
		kbd_buffer[i]=KBD_BUFFER_UNUSED;
	KBD_CLOCK_DDR &= ~(1<<KBD_CLOCK_BIT);
	KBD_DATA_DDR  &= ~(1<<KBD_DATA_BIT);
	cli();
	GICR |= (1<<INT2);		//enable INT2 in General Interupt Control Register (GICR)
	MCUCSR &= ~(1<<ISC2); 	//stitch to trigger on falling edge
	GIFR=(1<<5);		//clear INT2-Flag
	sei();
}

//KBD-Clock-Down Handler
/*

*/

INTERRUPT(SIG_INTERRUPT2){
	static byte c=0; //c counter
	static byte t=0, paraty=0;	//c temporary var. //paraty
	
	byte b=KBD_DATA_IN;
	
	++c;
	
	switch (c) {
		case 1:		//c start bit, start condition if data line low
			paraty = 0;
			c -= b;	//c if data line is high (no start bit) go here again
			break;
		case  2:	//c data bit 1
		case  3:	//c data bit 2
		case  4:	//c data bit 3
		case  5:	//c data bit 4
		case  6:	//c data bit 5
		case  7:	//c data bit 6
		case  8:	//c data bit 7
		case  9:	//c data bit 8
			t >>= 1;	
			t |= b?0x80:0;
		case 10:	//c paraty relevant bit
			paraty ^= b;
			break;
		case 11:	//c stop bit
			if (b==1){	//c stop condition is data line high
				c=0;	//c reset counter
				if (paraty == ODD_PARATY){
					//c insert byte to ringbuffer
					kbd_buffer[++kbd_buffer_index & KBD_BUFFER_MASK] = t;
					if (kbd_event_handler)	//c call user defined keyboard event handler
						kbd_event_handler(kbd_buffer_index);

				} else {
					//c lets simply ignore it
				}
			}
			break;
	}
}


