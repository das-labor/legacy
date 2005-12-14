#include <avr/io.h>

int main(){
	DDRC = 0xFF;
	PORTB = 0x0F;

	while(1){
		PORTC = PINB;
	}

}
