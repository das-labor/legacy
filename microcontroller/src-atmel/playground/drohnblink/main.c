
#include <avr/io.h>



void mydelay(){
	uint8_t x;
	for(x=0;x<100;x++){
		asm volatile("nop");
	}
}

#define DAT_H PORTD |= (1<<PD7)
#define STR_H PORTD |= (1<<PD6)
#define CLK_H PORTD |= (1<<PD5)


#define DAT_L PORTD &= ~(1<<PD7)
#define STR_L PORTD &= ~(1<<PD6)
#define CLK_L PORTD &= ~(1<<PD5)



int main(){


	DDRD = 0xff;
	DDRC = 0xff;

//	DDRB = 0x1f;
//	PORTB = 0x1f;	

	PORTD = 0xff;

	while(1){
		mydelay();
		PORTC = 0x20;
		mydelay();
		PORTC = 0x10;

		mydelay();
		PORTC = 0x08;
	}

	mydelay();
	STR_H;



	while(1);
}
