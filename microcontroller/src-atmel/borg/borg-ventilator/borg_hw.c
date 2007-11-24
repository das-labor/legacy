
/* #include <avr/signal.h> */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"



//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	uint8_t tmp;
	
	//die Daten für die aktuelle Zeile auf die Spaltentreiber ausgeben
	PORTA = ~pixmap[plane][row][0];
	PORTC = ~pixmap[plane][row][1];
	
	tmp = ~pixmap[plane][row][2];
	
	PORTB = (PORTB & 0xf0) | (0x0f & tmp);
	PORTD = (PORTD & 0x0f) | (0xf0 & tmp);

}


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	//Watchdog zurücksetzen
	wdt_reset();
	
	//Die aktuelle Zeile in der aktuellen Ebene ausgeben
	if(row < NUM_ROWS)
		rowshow(row, plane);
	
	//Zeile und Ebene inkrementieren
	if(++row == (NUM_ROWS+1) ){
		TIMSK = 0x00; //Timer interrupt off
		GICR = (1<<INT1); //LS Int on
		row = 0;
		PORTA = 0xff;
		PORTC = 0xff;
		PORTB = (PORTB & 0xf0) | (0x0f);
		PORTD = (PORTD & 0x0f) | (0xf0);
	
		//if(++plane==NUMPLANE) plane=0;
	}
}

SIGNAL(SIG_INTERRUPT1)
{
	GICR = 0;
	TCNT0 = 0;		// reset timer
	TIFR = 0x02;	//reset pending int
	TIMSK = 0x02;	// Compare match Interrupt on
}


void timer0_off(){
	cli();
	TCCR0 = 0x00;
	sei();
}


// Den Timer, der denn Interrupt auslöst, initialisieren
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
	TCCR0 = 0x0C;	// CTC Mode, clk/256
	TCNT0 = 0;	// reset timer
	OCR0  = 0xff;	// Compare with this value
	//TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	DDRA = 0xff;
	DDRC = 0xff;
	DDRB |= 0x0f;
	DDRD |= 0xf0;
	
	PORTA = 0xff;
	PORTC = 0xff;
	PORTB |= 0x0f;
	PORTD |= 0xf0;
	
	

	
	timer0_on();

	MCUCR = (1<<ISC11) ;
	GICR = (1<<INT1); //LS Int on
		
	//Watchdog Timer aktivieren
	wdt_reset();
//	wdt_enable(0x00);	// 17ms Watchdog
}
