
#include "../config.h"

#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

/*
// Diese #defines werden nun durch menuconfig gesetzt

//An diesen Pins ist das Schieberegister
//für die Spalten angeschlossen.
#define COLPORT  PORTC
#define COLDDR   DDRC
#define BIT_DAT	4 //Daten-Eingang
#define BIT_CLK 5 //Takt-Eingang

//An diesem Port sind die Zeilentreiber angeschlossen.
//Ein Null-Pegel schaltet den jeweiligen Transistor an.
#define ROWPORT1 PORTD
#define ROWDDR1   DDRD

#define ROWPORT2 PORTC
#define ROWDDR2 DDRC
*/

#define COLDDR   DDR(COLPORT)
#define ROWDDR1  DDR(ROWPORT1)
#define ROWDDR2  DDR(ROWPORT2)

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t rowmask = 0xFF;
	uint8_t x, tmp;
	
	//alle Zeilentreiber aus
	ROWPORT1 |= 0xF3;
	ROWPORT2 |= 0x0C;
	
	for(x=0;x<10;x++){
		asm volatile ("nop");	
	}
	
	if (row == 0){
		rowmask = 0x7F; //0111 1111
	}else{
		rowmask >>= 1;
		rowmask |= 0x80;
	}
	
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
	
	
	
	//Die fünf bits für das 2. Display in das
	//Schieberegister takten
	tmp = pixmap[plane][row][0];
	for(x=0;x<3;x++){
		if(tmp & 0x20){
			COLPORT &= ~(1<<BIT_DAT);
		}else{	
			COLPORT |= (1<<BIT_DAT);
		}
		tmp>>=1;
		
		COLPORT |= (1<<BIT_CLK);
		COLPORT &= ~(1<<BIT_CLK);
	}
	
	tmp = pixmap[plane][row][1];
	for(x=0;x<2;x++){
		if(tmp & 0x01){
			COLPORT &= ~(1<<BIT_DAT);
		}else{	
			COLPORT |= (1<<BIT_DAT);
		}
		tmp>>=1;
		
		COLPORT |= (1<<BIT_CLK);
		COLPORT &= ~(1<<BIT_CLK);
	}
	
	
	//Die restlichen 5 bit auch ins Schieberegister
	tmp = pixmap[plane][row][0];
	for(x=0;x<5;x++){
		if(tmp & 0x01){
			COLPORT &= ~(1<<BIT_DAT);
		}else{	
			COLPORT |= (1<<BIT_DAT);
		}
		tmp>>=1;
		
		COLPORT |= (1<<BIT_CLK);
		COLPORT &= ~(1<<BIT_CLK);
	}
	//nächste Zeile anschalten
	ROWPORT1 &= rowmask | 0x0C;
	ROWPORT2 &= rowmask | 0xF3;
}

extern uint8_t schmuh;

ISR(TIMER0_OVF_vect)
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
	ROWPORT1 |= 0xF3;
	// Port für Zeilentransistoren auf Ausgang
	ROWDDR1 |= 0xF3;
	
	ROWPORT2 |=0x0C;
	ROWDDR2 |=0x0C;

	
	//Signale für Schieberegister auf Ausgang
	COLDDR |= (1<<BIT_CLK) | (1<<BIT_DAT);
	COLPORT &= ~(1<<BIT_CLK);
	
	timer0_on();
}
