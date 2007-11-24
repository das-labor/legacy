
#include <avr/io.h>

#include "avrx.h"               // AvrX System calls/data structures



#include "xcan.h"
#include "xlap.h"


TimerControlBlock timer;


AVRX_GCC_TASKDEF(feuchtetask, 100, 8)
{
	DDRC |= (1<<PC5);
	ADMUX = (1<<REFS0) | (1<<ADLAR) | 4;//AVCC ref, left adjust result, channel 4
		
	static can_message_t msg = {0, 0xff, PORT_CHUCK, PORT_CHUCK, 1};
	msg.addr_src = myaddr;
	
	while(1){
		uint16_t s;
		uint8_t r;
		
		PORTC |= (1<<PC5);
		AvrXDelay(&timer, 5);
		
		ADCSRA = (1<<ADEN) | (1<<ADSC) | 7;//start conversion, clk/128
		AvrXDelay(&timer, 5);
		PORTC &= ~(1<<PC5);

		r = ADCH;
		
		msg.data[0] = r;
		
		can_put(&msg);
		
		for(s=0; s < 300; s++)
			AvrXDelay(&timer, 1000);
	}
}
