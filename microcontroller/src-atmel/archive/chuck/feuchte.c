
#include <avr/io.h>

#include "avrx.h"               // AvrX System calls/data structures



#include "xcan.h"
#include "xlap.h"


TimerControlBlock timer;

uint8_t feuchte = 100;

AVRX_GCC_TASKDEF(feuchtetask, 100, 8)
{
	uint16_t tmp;
	DDRC |= (1<<PC5);
	//ADMUX = (1<<REFS0) | (1<<ADLAR) | 4;//AVCC ref, left adjust result, channel 4
	ADMUX = (1<<REFS0) | 4;//AVCC ref, left adjust result, channel 4
			
	static can_message_t msg = {0, 0xff, PORT_CHUCK, PORT_CHUCK, 1};
	msg.addr_src = myaddr;
	
	while(1){
		uint16_t s;
		
		PORTC |= (1<<PC5);
		AvrXDelay(&timer, 5);
		
		ADCSRA = (1<<ADEN) | (1<<ADSC) | 7;//start conversion, clk/128
		AvrXDelay(&timer, 5);
		PORTC &= ~(1<<PC5);

		tmp = ADC;
		
		if(tmp < 100){
			feuchte = 100-tmp;
		}else{
			feuchte = 0;
		}
		
		msg.data[0] = feuchte;
		
		can_put(&msg);
		
		for(s=0; s < 300; s++)
			AvrXDelay(&timer, 1000);
	}
}
