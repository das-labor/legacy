#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

#include "rfm12.h"

extern rfm12_control_t ctrl;

int main ( void )
{	
	PORTB = 0x0f; //Pullups on
	PORTD = 0x60;

	DDRD = _BV(PD3) | _BV(PD1); //set not connected pins to output (save power)
	
	ACSR |= (1<<ACD);//disable analog comparator to save power

	DDRD |= 0x01;    //rfm reset
	_delay_ms(10);
	_delay_ms(10);
	DDRD &= ~0x01;
		
	//init the rfm module
	_delay_ms(250);
	rfm12_init();
	sei();
	
	//set the wakeup timer to 10 ms
	rfm12_set_wakeup_timer(10);
	

	uint8_t joy = 0;
	uint8_t joy_old = 0;
	uint8_t ticker = 0;
	
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
		
		if((joy ^ joy_old) || (ticker == 50))
		{
			ticker = 0;
			
			//use rfm12_start_tx instead of rfm12_tx  to avoid memcpy for 1 byte
			*rf_tx_buffer.buffer = joy;
			if(RFM12_TX_ENQUEUED == rfm12_start_tx (0x69, 1))
			{
				joy_old = joy;
				
				while(ctrl.rfm12_state != 0)
				rfm12_tick();
			}					
		}

		rfm12_tick();
		
		ticker ++;
		
		while(rfm12_get_batt_status() == RFM12_BATT_LOW)
		{
			joy = 0x3;	
			while(rfm12_tx (1, 0x69, &joy) != RFM12_TX_ENQUEUED);
			rfm12_tick();
			_delay_ms(1000);
		}
		
		
		//sleep
		//set_sleep_mode(SLEEP_MODE_PWR_DOWN);	
		
		
		while(!ctrl.wkup_flag){
			sleep_mode();
		}
		ctrl.wkup_flag = 0;
	}
}
