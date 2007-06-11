
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "borg_hw.h"

#include "client.h"

/* Steckerbelegung Flachbandkabel am Panel
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
 * in Richtung Panel, Nr. 3 treibt von den Tastenausg�ngen auf den Datenbus.
 * Die EOs von 1,2 und 4 liegen fest auf GND.
 *
 * Die LEDs sind in einer 12*16 Matrix angeordnet
 * Die Werte f�r die LED spalten Werden mit CP1 und CP2 in die 
 * Latches �bernommen (insgesammt 16 Spalten)
 * Die Nummer der Zeile wird beim l�schen von /show �bernommen.
 *
 * Die Tasten sind in einer 8*8 Matrix angeordnet.
 * �ber Latch 4 werden die Zeilen einzeln auf high gesetzt, �ber 
 * Latch 3 k�nnen dann die Spalten gelesen werden.
 * 
 */

//Datenport f�r das Panel
#define DATAPORT  	PORTC
#define DATADDR   	DDRC
#define DATAPIN		PINC

#define CTRLPORT 	PORTD
#define CTRLDDR   	DDRD

// PINs on CTRLPORT
#define PIN_CP4  	PD2
#define PIN_SHOW 	PD3
#define PIN_CP1  	PD4
#define PIN_CP2  	PD5
#define PIN_CP3  	PD6
#define PIN_EO3  	PD7

//Der Puffer, in dem das aktuelle Bild gespeichert wird
uint8_t pixmap[NUM_ROWS][LINEBYTES];

volatile uint8_t keys[8];

KeyboardMsg_t KeyboardMsg;

static inline void busywait() {
	//unsigned char i;
	//for(i=0;i<20;i++){
	//	asm volatile("nop");
	//}
}


//Eine Zeile anzeigen
static inline void rowshow(unsigned char row){
	CTRLPORT |= (1<<PIN_SHOW);//blank
	
	DATAPORT  = pixmap[row][0];
	busywait();
	CTRLPORT |=  (1<<PIN_CP1);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP1);
	busywait();

	DATAPORT  = pixmap[row][1];
	busywait();
	CTRLPORT |=  (1<<PIN_CP2);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP2);
	busywait();

	DATAPORT  = row;
	busywait();
	CTRLPORT &= ~(1<<PIN_SHOW);
}

static inline void checkkeys(uint8_t row){
	static uint8_t mask;
	if(row == 0){
		mask = 1;
	}else{
		uint8_t tmp, new, x;
		uint8_t r = row-1;
		//read keyboard cols into latch
		DATADDR = 0;
		CTRLPORT &= ~(1<<PIN_EO3);
		CTRLPORT |= (1<<PIN_CP3);
		busywait();
		CTRLPORT &= ~(1<<PIN_CP3);
		busywait();
		tmp = DATAPIN;
		new = tmp & ( ~keys[r]);
		if(new){
			for(x=0;x<8;x++){
				if(new & 0x01){
					if (AvrXTestMessageAck ((MessageControlBlock*)&KeyboardMsg)){
						KeyboardMsg.key = (r<<4) | x;
						AvrXIntSendMessage(&ClientQueue, (MessageControlBlock*)&KeyboardMsg);
						break;
					}
				}
				new >>= 1;
			}
		}
		keys[r] = tmp;
		CTRLPORT |= (1<<PIN_EO3);
		busywait();
		DATADDR = 0xFF;
	}
	
	DATAPORT  = mask;
	mask <<= 1;
	busywait();
	CTRLPORT |= (1<<PIN_CP4);
	busywait();
	CTRLPORT &= ~(1<<PIN_CP4);
}

uint8_t key_is_pressed(uint8_t key){
	if(key[(key>>4)] & (1<<(key & 0x07))) return 1;
	return 0;
}

static inline void borg_int()
{
	static unsigned char row = 0;
	
	//Watchdog zur�cksetzen
	wdt_reset();
	
	//Die aktuelle Zeile in der aktuellen Ebene ausgeben
	rowshow(row);

	if(row<9) checkkeys(row);
	
	//Zeile und Ebene inkrementieren
	if(++row == NUM_ROWS){
		row = 0;
	}
}

AVRX_SIGINT(SIG_OUTPUT_COMPARE0)
{
    IntProlog();                // Save interrupted context, switch stacks
    borg_int();
	AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};


void timer0_off(){
	cli();

	TCCR0 = 0x00;
	sei();
}


// Den Timer, der denn Interrupt ausl�st, initialisieren
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
	TCCR0 = (1<<CS02)|(1<<WGM01);// Set Timer0 to CPUCLK/256 CTC mode
	OCR0  = F_CPU/256/TICKRATE;	// Set Compare to tickrate
	TIMSK |= (1<<OCIE0);	// Compare match Interrupt on
	
}

void borg_hw_init(){
	//Pins am Zeilenport auf Ausgang
	CTRLPORT |= (1<<PIN_EO3)|(1<<PIN_SHOW);
	CTRLDDR  |= (1<<PIN_EO3) | (1<<PIN_CP4) | (1<<PIN_SHOW) | (1<<PIN_CP1) | (1<<PIN_CP2) | (1<<PIN_CP3);
	
	//Alle Spalten erstmal aus
	//Spalten Ports auf Ausgang
	DATADDR  = 0xFF;
	DATAPORT = 0x00;

	//Timer an
	timer0_on();
	
	//Watchdog Timer aktivieren
	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
	
	AvrXAckMessage((MessageControlBlock*)&KeyboardMsg);
}
