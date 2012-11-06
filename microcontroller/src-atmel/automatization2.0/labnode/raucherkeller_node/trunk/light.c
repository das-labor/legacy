#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

uint8_t channels_active[NUM_CHANNELS];

void light_init() {
	//disable all channels
	for(uint8_t i; i < NUM_CHANNELS; i++)
		enable_channel(i, 0);
	
	//set switches to output
	DDRA |= _BV(PA0) | _BV(PA1) | _BV(PA2) | _BV(PA3);
	DDRC |= _BV(PC6) | _BV(PC7);
}

void enable_channel(uint8_t channel, uint8_t enable)
{
	if (channel < NUM_CHANNELS)
	{
		channels_active[channel] = enable;
		uint8_t *port = 0, pinmask;
		
		//decode port/pin
		switch (channel) {
			//neon light
			case 0:
				port = &PORTA;
				pinmask = _BV(PA1);
				break;
				
			//ddr light
			case 1:
				port = &PORTA;
				pinmask = _BV(PA0);
				break;
				
			//not connected
			case 2:
				port = &PORTA;
				pinmask = _BV(PA2);
				break;
				
			//not connected
			case 3:
				port = &PORTA;
				pinmask = _BV(PA3);
				break;
				
			//not connected
			case 4:
				port = &PORTC;
				pinmask = _BV(PC6);
				break;
				
			//not connected
			case 5:
				port = &PORTC;
				pinmask = _BV(PC7);
				break;
			
			default:
				break;
		}
		
		if(port)
		{
			if(enable)
				*port |= pinmask;
			else
				*port &= ~pinmask;
		}
		
		send_status();
	}
}

uint8_t get_channel_status() {
	uint8_t i, s = 0;
	for(i = 0; i < NUM_CHANNELS; i++)
		if(channels_active[i])
			s |= 1 << i;
	return s;
}

