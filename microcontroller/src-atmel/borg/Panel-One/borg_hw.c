
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

//Datenport für das Panel
#define COLPORT  PORTA
#define COLDDR   DDRA

#define CTRLPORT PORTC
#define CTRLDDR   DDRC

// PINs on CTRLPORT
#define PIN_EO3  PC5
#define PIN_CP4  PC4
#define PIN_SHOW PC3
#define PIN_CP1  PC2
#define PIN_CP2  PC1
#define PIN_CP3  PC0

//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void busywait() {
	unsigned char i;
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
}


//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	CTRLPORT |= (1<<PIN_SHOW);//blank
	
	COLPORT  = pixmap[plane][row][0];
	busywait();
	CTRLPORT |=  (1<<PIN_CP1);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP1);
	busywait();

	COLPORT  = pixmap[plane][row][1];
	busywait();
	CTRLPORT |=  (1<<PIN_CP2);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP2);
	busywait();

	COLPORT  = row;
	busywait();
	CTRLPORT &= ~(1<<PIN_SHOW);
	busywait();
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
		switch(plane){
			case 0: 
					OCR0 = 5;
					break;
			case 1:
					OCR0 = 12;
					break;
			case 2:
					OCR0 = 20;
					break;
		}
	}
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
	TCCR0 = 0x0C;	// CTC Mode, clk/64
	TCNT0 = 0;	// reset timer
	OCR0  = 20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	//Pins am Zeilenport auf Ausgang
	CTRLDDR  |= (1<<PIN_EO3) | (1<<PIN_CP4) | (1<<PIN_SHOW) | (1<<PIN_CP1) | (1<<PIN_CP2) | (1<<PIN_CP3);
	CTRLPORT |= (1<<PIN_EO3);
	
	//Alle Spalten erstmal aus
	//Spalten Ports auf Ausgang
	COLDDR  = 0xFF;
	COLPORT = 0x00;
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
