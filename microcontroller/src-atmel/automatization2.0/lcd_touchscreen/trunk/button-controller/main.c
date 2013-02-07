
#include <avr/io.h>
#include <util/delay.h>


void set_leds(uint8_t msk){
	//PB4:0,PD6
	msk &= 0x3F;
	PORTB = (PORTB & 0xE0) | (msk >> 1);
	PORTD = (PORTD & 0xBF) | ((msk & 0x01)<<6);

}

int main(){
	
	DDRD = 0x40;
	DDRB = 0x1F;
	
	PORTA = 0x03;
	PORTD = 0x3C;
	
	uint8_t buttons;
	 
	
	while(1){
		//PD5:2, PDA0:1
		buttons = (PIND & 0x3C) | ((PINA & 0x02)>>1) | ((PINA & 0x01)<<1) ;
		set_leds(buttons);
	}
	
	while(1){
		uint8_t msk = 1;
		do{
			set_leds(msk);
			_delay_ms(50);
			msk <<= 1;
			
		}while(msk);
	}

}