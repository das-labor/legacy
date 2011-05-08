
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


//Dieser Interrupt wird je nach Ebene mit 50kHz 31,25kHz oder 12,5kHz ausgeführt
SIGNAL(SIG_OUTPUT_COMPARE0)
{	
	//Watchdog zurücksetzen
	wdt_reset();
	
	
	COLPORT1 = (pixmap[0][0][0] & 0x0f) | (pixmap[0][1][0] << 4);
	COLPORT2 = (pixmap[0][2][0] & 0x0f) | (pixmap[0][3][0] << 4);
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
	
	//Alle Zeilen ausgänge an bei gigaborg
	ROWPORT |= (1<<PIN_DATA) | (1<<PIN_MCLR);
	uint8_t x;
	for(x=0;x<16;x++){
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
	}
	
	
	timer0_on();

	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}
