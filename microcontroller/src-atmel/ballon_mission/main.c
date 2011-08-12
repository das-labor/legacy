
#include <stdio.h>
#include "iopins.h"


void init_timer1(){
	TCCR1A = (1<<COM1B1);
	TCCR1B = (1<<WGM12)| 2; //clk/8, TOP = OCR1A
	OCR1A  = 0;
	OCR1B = 2000;
	SET_DDR(BUZZER_PIN);
}





int main(){
	init_timer1();
	_delay_ms(1000);
	OCR1A = 20;
	
	_delay_ms(1000);
	
	OCR1A = 0;


}
