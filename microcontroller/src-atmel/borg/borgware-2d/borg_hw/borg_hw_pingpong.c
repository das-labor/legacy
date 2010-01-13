
#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

#define PIN_DATA PB4
#define PIN_CLK	 PB3
#define PIN_STR  PB2



//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


//zur nächsten Zeile weiterschalten
inline void nextrow(uint8_t row){

	//Die Zustände von der vorherigen Zeile löschen
	PORTC &= 0xF0;
	PORTD &= 0x0F;
	PORTB &= 0xFC;
	
	//kurze Warteschleife, damit die Treiber auch wirklich ausschalten
	
	unsigned char i;
	for(i=0;i<10;i++){
		asm volatile("nop");
	}
	
	if (row == 0){
		//Zeile 0: Das erste Schieberegister initialisieren
		PORTB &= ~(1<<PIN_DATA); // zeile ist aktiv auf low
		PORTB |= (1<<PIN_CLK);
		PORTB &= ~(1<<PIN_CLK);
		PORTB |= (1<<PIN_DATA);
	}
	else {
		//In jeder anderen Zeile einfach nur einen weiter schieben
		PORTB |= (1<<PIN_CLK);
		PORTB &= ~(1<<PIN_CLK);
	}
	
	//noch eine Warteschleife, damit die Zeilentreiber bereit sind
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
}



//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	//Je nachdem, welche der Ebenen wir Zeichnen, die Zeile verschieden lange Anzeigen
	switch (plane){
		case 0:
			TCNT0 = 0x100-12;
			break;
		case 1:
			TCNT0 = 0x100-20;
			break;
		case 2:
		 	TCNT0 = 0x100-50;
	}
	
	uint8_t tmp, tmp1;
	//die Daten für die aktuelle Zeile auf die Spaltentreiber ausgeben
	
	tmp  = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];
	
	PORTC = ( PORTC & 0xF0 ) | (tmp & 0x0F);
	PORTD = ( PORTD & 0x0F ) | (tmp & 0xF0);
	PORTB = ( PORTB & 0xFC ) | (tmp1 & 0x03);
	
}


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OVERFLOW0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	//Watchdog zurücksetzen
	wdt_reset();
	
	//Zeile und Ebene inkrementieren
	if(++plane==NUMPLANE){
		plane=0;
		if(++row == NUM_ROWS){
			row = 0;
		}
		nextrow(row);
	}
	
	//Die aktuelle Zeile in der aktuellen Ebene ausgeben
	rowshow(row, plane);
}


void timer0_off(){
	cli();
/*
	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;
*/
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
	TCCR0 = 0x03;	// clk/64
	TCNT0 = 0xFF-20;	// reset timer
	TIMSK |= (1<<TOIE0);	// Compare match Interrupt on
}

void borg_hw_init(){

	// Nötige Pins auf Ausgang
	DDRC=0x0F; // PC0-3 - Row 0-3
	DDRD=0xF0; // PCD-7 - Row 4-7
	DDRB=0x1F; // PB0-1 - Row 8-9  PB2-3 - STR, CLK, d
	
	// Alle Spalten erstmal aus, clk aus, d und str an
	PORTC &= 0xF0;
	PORTD &= 0x0F;
	PORTB = ( PORTB & 0xE0 ) | 0x14;
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
