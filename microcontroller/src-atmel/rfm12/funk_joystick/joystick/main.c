#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

#include "rfm12.h"


int main ( void )
{	
	PORTB = 0x1f; //Pullups on
	PORTD = 0x60;
	
	ACSR |= (1<<ACD);//disable analog comparator to save power

	DDRD |= 0x01;    //rfm reset
	_delay_ms(10);
	_delay_ms(10);
	DDRD &= ~0x01;
	
	DDRD |= (1<<PD4);
	PORTD |= (1<<PD4);
	
	uint16_t x=200;
	
	while(x--)
		_delay_ms(1);

	rfm12_init();
	rfm12_set_wakeup_timer(10);
	
	sei();

	uint8_t joy = 0;
	uint8_t joy_old = 0;
	
	while (42)
	{
		//PB0 Up
		//PB1 Down
		//PB2 Left
		//PB3 Right
		//
		//PD5 Fire1
		//PD6 Fire2
		
		//-> F2 F1 RT LF DN UP
		
		joy = ( (PINB & 0x0f) | ((PIND & 0x60)>>1) );
		joy ^= 0x3f;
		
		if(joy ^ joy_old)
		{
			if(RFM12_TX_ENQUEUED == rfm12_tx (2, 0x69, &joy)){
				joy_old = joy;
			}
		}

		rfm12_tick();
		
		#define __BV _BV
//		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
	}
}
