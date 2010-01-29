
#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
// Diese #defines werden nun durch menuconfig gesetzt

// 16 Spalten insgesamt direkt gesteuert, dafür 2 Ports
#define COLPORT1  PORTA
#define COLDDR1   DDRA

#define COLPORT2  PORTC
#define COLDDR2   DDRC

// Der andere Port übernimmt die Steuerung der Schieberegister
#define ROWPORT PORTD
#define ROWDDR   DDRD
// Clock und reset gehen gemeinsam an beide Schieberegister
// der reset pin ist negiert
#define PIN_MCLR  PD4  
#define PIN_CLK  PD5
//das dier sind die individuellen Dateneingänge für die Schieberegister
#define PIN_DATA1 PD6
#define PIN_DATA2 PD7
*/

#define COLDDR1  DDR(COLPORT1)
#define COLDDR2  DDR(COLPORT2)
#define ROWDDR   DDR(ROWPORT)

#ifdef __AVR_ATmega644P__
/* more ifdef magic :-( */
#define OCR0 OCR0A
#define SIG_OUTPUT_COMPARE0 SIG_OUTPUT_COMPARE0A
#endif


//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	//Die Zustände von der vorherigen Zeile löschen
	COLPORT1 = 0;
	COLPORT2 = 0;
	
	//kurze Warteschleife, damit die Treiber auch wirklich ausschalten
	unsigned char i;
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	
	
	if (row == 0){
		//Zeile 0: Das erste Schieberegister initialisieren
		ROWPORT&= ~(1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_DATA1);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_DATA1);
		
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
	}else if(row == 8){
		//Zeile 8: Das Zweite Schieberegister initialisieren
		ROWPORT&= ~(1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_DATA2);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_DATA2);
	}else{
		//In jeder anderen Zeile einfach nur einen weiter schieben
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
	}
	
	//ncoh eine Warteschleife, damit die Zeilentreiber bereit sind
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	
	//die Daten für die aktuelle Zeile auf die Spaltentreiber ausgeben
	COLPORT1 = pixmap[plane][row][0];
	COLPORT2 = pixmap[plane][row][1];
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

#ifdef __AVR_ATmega644P__
        TCCR0A = 0x00;
        TCCR0B = 0x00;
#else
	TCCR0 = 0x00;
#endif
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

#ifdef __AVR_ATmega644P__
        TCCR0A = 0x02;  // CTC Mode
        TCCR0B = 0x03;  // clk/64
	TCNT0  = 0;	// reset timer
	OCR0   = 20;	// Compare with this value
	TIMSK0 = 0x02;	// Compare match Interrupt on
#else
	TCCR0 = 0x0B;	// CTC Mode, clk/64
	TCNT0 = 0;	// reset timer
	OCR0  = 20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
#endif
}

void borg_hw_init(){
	//Spalten Ports auf Ausgang
	COLDDR1 = 0xFF;
	COLDDR2 = 0xFF;
	
	//Pins am Zeilenport auf Ausgang
	ROWDDR = (1<<PIN_MCLR) | (1<<PIN_CLK) | (1<< PIN_DATA1) | (1<<PIN_DATA2);
	
	//Alle Spalten erstmal aus
	COLPORT1 = 0;
	COLPORT2 = 0;
	
	//Schieberegister für Zeilen zurücksetzen
	ROWPORT = 0;
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
