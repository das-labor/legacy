
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

//An diesen Pins ist das Schieberegister
//für die Spalten angeschlossen.
#define COLPORT  PORTC
#define COLDDR   DDRC
#define BIT_DAT	5 //Daten-Eingang
#define BIT_CLK 4 //Takt-Eingang



//An diesem Port sind die Zeilentreiber angeschlossen.
//Ein Null-Pegel schaltet den jeweiligen Transistor an.
#define ROWPORT PORTD
#define ROWDDR   DDRD

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t rowmask = 0xFF;
	uint8_t x, tmp;
	
	//alle Zeilentreiber aus
	ROWPORT = 0x00;
	
	for(x=0;x<10;x++){
		asm volatile ("nop");	
	}
	
	if (row == 0){
		rowmask = 0x40; //0100 0000
	}else{
		rowmask >>= 1;
	}
	
	switch (plane){
		case 0:
			TCNT0 = 0x100-2;
			break;
		case 1:
			TCNT0 = 0x100-8;
			break;
		case 2:
			TCNT0 = 0x100-20;
	}
	
	
	
	//Die fünf bits für das 2. Display in das
	//Schieberegister takten
	tmp = pixmap[plane][row][0];
	for(x=0;x<8;x++){
		if(tmp & 0x01){
			COLPORT |= (1<<BIT_DAT);
		}else{	
			COLPORT &= ~(1<<BIT_DAT);
		}
		tmp>>=1;
		
		COLPORT |= (1<<BIT_CLK);
		COLPORT &= ~(1<<BIT_CLK);
	}
	
	//nächste Zeile anschalten
	ROWPORT = rowmask;
}

extern uint8_t schmuh;

SIGNAL(SIG_OVERFLOW0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	rowshow(row, plane);
	
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
	}
	
	schmuh = 1;
}



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
	TCNT0 = 0xFF-20;// reset timer
	TIMSK |= (1<<TOIE0);
}


void borg_hw_init(){
	// Alle Zeilentransistoren aus.
	ROWPORT = 0x00;
	// Port für Zeilentransistoren auf Ausgang
	ROWDDR = 0xFF;
	
	//Signale für Schieberegister auf Ausgang
	COLDDR |= (1<<BIT_CLK) | (1<<BIT_DAT);
	COLPORT &= ~(1<<BIT_CLK);
	
	timer0_on();
}
