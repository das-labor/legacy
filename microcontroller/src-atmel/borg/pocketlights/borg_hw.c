#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"
#include "buttons.h"
#include "fifo.h"

// 16 Spalten insgesamt direkt gesteuert, dafür 2 Ports
#define COLPORT1  		PORTB
#define COLDDR1   		DDRB

#define COLPORT2  		PORTA
#define COLDDR2   		DDRA

#define COLPORT3  		PORTC
#define COLDDR3   		DDRC

#define ROWPORT 			PORTD
#define ROWDDR   			DDRD

// Der Puffer, in dem das aktuelle Bild gespeichert wird
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

//Eine Zeile anzeigen
inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t mask = 0x01;
	
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
	COLPORT3 = pixmap[plane][row][0];
	COLPORT2 = pixmap[plane][row][1];	
}


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	// Zwei Statusbytes für die Button Pins. --> Vergleich; Übergang 1->0 = button_release = button gedrückt
	static unsigned char button_pin_state = 0;
	static unsigned char old_button_pin_state=0;
	
	//Watchdog zurücksetzen
	wdt_reset();
	
	// Die aktuelle Zeile in der aktuellen Ebene ausgeben
	rowshow(row, plane);
	
	// *** Pinstatus an aktueller Zeile von links ins Byte schieben (damit Button_status 0 an Bit 0 steht)
	//shift byte nach rechts
	button_pin_state>>=1;
	//wenn signal am Buttonpin (aktuell 3), dann wird das höchste Bit auf 1 gesetzt
	if((PINB & (1<<BUTTONPIN)))
		button_pin_state |= 0x80;
		
	//Zeile und Ebene inkrementieren, Buttons abfragen
	if(++row == NUM_ROWS){	
		// Nach der letzten Helligkeitsstufe (plane), also nach einem kompletten Frame, Buttonpins speichern
		if(++plane==NUMPLANE){
			plane=0;
		}
		row =0;
		
		// registriert Button DOWN
		if((~old_button_pin_state) & button_pin_state){
			_inline_fifo_put(&fifo, button_pin_state);
		}
		
		// registriert Button RELEASE
/*		if(old_button_pin_state & (~button_pin_state)){
			//_inline_fifo_put (fifo_t *f, const uint8_t data)
		}
*/
		
		// Pinstate als "alt" speichern
		old_button_pin_state=button_pin_state;
		button_pin_state=0;
	}
}

void timer0_off(){
	cli();

	COLPORT1 = 0;
	COLPORT2 = 0;
	COLPORT3 = 0;
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
	
	// Alle Zeilen aus
	ROWPORT = 0;
	
	init_buttons();
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
