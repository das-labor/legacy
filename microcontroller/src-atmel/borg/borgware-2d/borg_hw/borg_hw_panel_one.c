
#include"../autoconf.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/* Steckerbelegung Flachbandkabel am Panel
 * (die Nummerierung ist in wirklichkeit umgekehrt)
 * 
 * 1-3		GND
 * 4		+5V für Logic
 * 5-8		+12V
 * 9-10		GND
 * 11		CP3
 * 12		CP2
 * 13		CP1
 * 14		/show
 * 15		CP4
 * 16		/EO3
 * 17-18	GND
 * 19-26	D0-D7
 *
 * Und nochmal richtigrum:
 * 1 		D7
 * 2 		D6
 * 3 		D5
 * 4 		D4
 * 5 		D3
 * 6 		D2
 * 7 		D1
 * 8 		D0
 * 9 		GND
 * 10		GND
 * 11		/EO3
 * 12		CP4
 * 13		/show
 * 14		CP1
 * 15		CP2
 * 16		CP3
 * 17		GND
 * 18		GND
 * 19		+12V
 * 20		+12V
 * 21		+12V
 * 22		+12V
 * 23		+5V
 * 24		GND
 * 25		GND
 * 26		GND
 *
 * Es werden 4 40374 Latches benutzt. Nr. 1,2 und 4 treiben vom Datenbus
 * in Richtung Panel, Nr. 3 treibt von den Tastenausgängen auf den Datenbus.
 * Die EOs von 1,2 und 4 liegen fest auf GND.
 *
 * Die LEDs sind in einer 12*16 Matrix angeordnet
 * Die Werte für die LED spalten Werden mit CP1 und CP2 in die 
 * Latches übernommen (insgesammt 16 Spalten)
 * Die Nummer der Zeile wird beim löschen von /show übernommen.
 *
 * Die Tasten sind in einer 8*8 Matrix angeordnet.
 * Über Latch 4 werden die Zeilen einzeln auf high gesetzt, über 
 * Latch 3 können dann die Spalten gelesen werden.
 * 
 */

//Datenport für das Panel
#define COLPORT  PORTC
#define COLDDR   DDRC
#define COLPIN	PINC

#define CTRLPORT PORTD
#define CTRLDDR   DDRD

// PINs on CTRLPORT
#define PIN_EO3  PD7
#define PIN_CP4  PD2
#define PIN_SHOW PD3
#define PIN_CP1  PD4
#define PIN_CP2  PD5
#define PIN_CP3  PD6

//Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

volatile uint8_t keys[8];

inline void busywait() {
	//unsigned char i;
	//for(i=0;i<20;i++){
	//	asm volatile("nop");
	//}
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
}

inline void checkkeys(uint8_t row){
	static uint8_t mask;
	if(row == 0){
		mask = 1;
	}else{
		//read keyboard cols into latch
		COLDDR = 0;
		CTRLPORT &= ~(1<<PIN_EO3);
		CTRLPORT |= (1<<PIN_CP3);
		busywait();
		CTRLPORT &= ~(1<<PIN_CP3);
		busywait();
		keys[row-1] = COLPIN;
		CTRLPORT |= (1<<PIN_EO3);
		busywait();
		COLDDR = 0xFF;
	}
	
	COLPORT  = mask;
	mask <<= 1;
	busywait();
	CTRLPORT |= (1<<PIN_CP4);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP4);
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

	if( (plane == 2) && (row<9) ) checkkeys(row);
	
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
	CTRLPORT |= (1<<PIN_EO3)|(1<<PIN_SHOW);
	CTRLDDR  |= (1<<PIN_EO3) | (1<<PIN_CP4) | (1<<PIN_SHOW) | (1<<PIN_CP1) | (1<<PIN_CP2) | (1<<PIN_CP3);
	
	//Alle Spalten erstmal aus
	//Spalten Ports auf Ausgang
	COLDDR  = 0xFF;
	COLPORT = 0x00;
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
