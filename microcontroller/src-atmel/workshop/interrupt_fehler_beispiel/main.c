
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>


SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char toggle=0;
	if((PORTC & 0x01) != toggle){//wir überprüfen uns selbst: toggle sollte immer PORTC.0 entsprechen
	
					//wenn wir hier ankommen, dann ist also irgendwo ein Denkfehler drinn....
		PORTC ^= 0x80;		//LED7 invertieren
	}
	
	PORTC ^= 1;		//LED0 invertieren
	toggle ^= 1;		//PORTC und toggle immer gleichmässig ändern
	
}

void timer0_on(){
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR0 = 0x0D;	// CTC Mode, clk/1024
	TCNT0 = 0;	// timer zurücksetzen
	OCR0  = 0x10;	// Hiermit vergleichen
	TIMSK = 0x02;	// Interrupt bei TCNT0 == OCR0
}


int main (void){
	DDRC = 0xFF;	//Alle LED Pins als Ausgang

	volatile unsigned char x;
	
	timer0_on();  	//timer configurieren
	
	sei();		//Interrupts aktivieren
	
	while(1){
		for(x=0;x<254;x++);     //warten
		
		PORTC |= 0x30;		//LED 4 und 5 anschalten
		
		for(x=0;x<254;x++);     //warten
		
		PORTC &= ~0x30;		//LED 4 und 5 ausschalten
		
	}
}
