
#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
// Diese #defines werden nun durch menuconfig gesetzt

 // 16 Spalten insgesamt direkt gesteuert, dafür 2 Ports
#define COLPORT1  PORTC
#define COLDDR1   DDRC

#define COLPORT2  PORTA
#define COLDDR2   DDRA

// Der andere Port übernimmt die Steuerung der Schieberegister
#define ROWPORT PORTD
#define ROWDDR   DDRD
// Clock und reset gehen gemeinsam an beide Schieberegister
// der reset pin ist negiert
#define PIN_MCLR  PD4  
#define PIN_CLK  PD6
//das dier sind die individuellen Dateneingänge für die Schieberegister
#define PIN_DATA PD7
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


//zur nächsten Zeile weiterschalten
inline void nextrow(uint8_t row){
	//Die Zustände von der vorherigen Zeile löschen
	COLPORT1 = 0;
	COLPORT2 = 0;
	
	//kurze Warteschleife, damit die Treiber auch wirklich ausschalten
	
	unsigned char i;
	for(i=0;i<10;i++){
		asm volatile("nop");
	}
	
	if (row == 0){
		//Zeile 0: Das erste Schieberegister initialisieren
#ifndef INVERT_ROWS
		ROWPORT&= ~(1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_MCLR);
		ROWPORT|= (1<<PIN_DATA);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_DATA);
#else
		ROWPORT&= ~(1<<PIN_DATA);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT|= (1<<PIN_DATA);

#endif
	}else{
		//In jeder anderen Zeile einfach nur einen weiter schieben
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
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
			OCR0 = 3;
			break;
		case 1:
			OCR0 = 4;
			break;
		case 2:
			OCR0 = 22;
	}
	
	uint8_t tmp, tmp1;
	//die Daten für die aktuelle Zeile auf die Spaltentreiber ausgeben
	
#ifndef INTERLACED_ROWS
	tmp  = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];
#else
	row = (row>>1) + ((row & 0x01)?8:0 );
	tmp  = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];
#endif
#ifdef REVERSE_COLS
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33)<< 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55)<< 1); //0xaa = 10101010, 0x55 = 1010101
	COLPORT2 = tmp;
	tmp = tmp1;
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33) << 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55) << 1); //0xaa = 10101010, 0x55 = 1010101
	COLPORT1 = tmp;
#else
#ifdef INTERLACED_COLS
	static uint8_t interlace_table[16] = {
		0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15, 0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55
	};
	COLPORT1 = interlace_table[tmp&0x0f]  | (interlace_table[tmp1&0x0f]<<1);
	tmp>>=4; tmp1>>=4;
	COLPORT2 = interlace_table[tmp]  | (interlace_table[tmp1]<<1);
#else
	COLPORT1 = tmp;
	COLPORT2 = tmp1;
#endif
#endif	
	
}


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OUTPUT_COMPARE0)
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
        TCCR0B = 0x04;  // clk/256
	TCNT0  = 0;	// reset timer
	OCR0   = 20;	// Compare with this value
	TIMSK0 = 0x02;	// Compare match Interrupt on
#else
	TCCR0 = 0x0C;	// CTC Mode, clk/256
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
	ROWDDR = (1<<PIN_MCLR) | (1<<PIN_CLK) | (1<< PIN_DATA);
	
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
