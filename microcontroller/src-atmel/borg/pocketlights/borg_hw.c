
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

// 16 Spalten insgesamt direkt gesteuert, dafür 2 Ports
#define COLPORT1  PORTB
#define COLDDR1   DDRB


#define COLPORT2  PORTA
#define COLDDR2   DDRA

#define COLPORT3  PORTC
#define COLDDR3   DDRC

#define ROWPORT PORTD
#define ROWDDR   DDRD


//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline uint8_t bitrot(uint8_t b){
	uint8_t x, r=0;
	for(x=0;x<8;x++){
		r<<=1;
		r |= b & 0x01;
		b>>=1;
	}
	return r;
}

//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t mask = 0x01;
	uint8_t tmp;

	//Die Zustände von der vorherigen Zeile löschen
	COLPORT1 &= ~0x03;
	COLPORT2 = 0;
	COLPORT3 = 0;
	
	//kurze Warteschleife, damit die Treiber auch wirklich ausschalten
	unsigned char i;
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	
	
	if (row == 0){
		mask = 0x01;
		//Je nachdem, welche der Ebenen wir Zeichnen, die Zeile verschieden lange Anzeigen
		switch (plane){
			case 0:
				OCR0 = 5;
				break;
			case 1:
				OCR0 = 8;
				break;
			case 2:
				OCR0 = 20;
		}
	}else{
		mask<<=1;
	}
	
	ROWPORT = mask;
	
	//ncoh eine Warteschleife, damit die Zeilentreiber bereit sind
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	
	//die Daten für die aktuelle Zeile auf die Spaltentreiber ausgeben
	COLPORT1 = (pixmap[plane][row][2] & 0x03 ) | (COLPORT1 & ~0x03);
	//tmp = (pixmap[plane][row][0] <<2 )| (pixmap[plane][row][1]>>6);
	COLPORT2 = bitrot(pixmap[plane][row][1]);
	COLPORT3 = (pixmap[plane][row][0]);
}


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	//Watchdog zurücksetzen
	wdt_reset();
	
	//Die aktuelle Zeile in der aktuellen Ebene ausgeben
	rowshow(row, plane);
	
	//Zeile und Ebene inkrementieren
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
	}
}


void timer0_off(){
	cli();

	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;

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
	TCCR0 = 0x0B;	// CTC Mode, clk/64
	TCNT0 = 0;	// reset timer
	OCR0  = 20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	//Spalten Ports auf Ausgang
	COLDDR1 |= 0x03;
	COLDDR2 = 0xFF;
	COLDDR3 = 0xFF;
	
	//Pins am Zeilenport auf Ausgang
	ROWDDR = 0xFF;
	
	//Alle Spalten erstmal aus
	COLPORT1 &= ~0x03;
	COLPORT2 = 0;
	COLPORT3 = 0;
	
	ROWPORT = 0;
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
