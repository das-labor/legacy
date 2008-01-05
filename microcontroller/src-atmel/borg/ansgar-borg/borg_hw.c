
//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

//An diesen Pins ist das Schieberegister
//für die Spalten angeschlossen.
#define SHIFTPORT  PORTD
#define SHIFTDDR  DDRD
#define CLOCKPORT PORTC
#define CLOCKDDR DDRC
#define BIT_DAT	7 //Daten-Eingang
#define BIT_CLK 0 //Takt-Eingang



//An diesem Port sind die Zeilentreiber angeschlossen.
//Ein Eins-Pegel schaltet den jeweiligen Transistor an.
#define COLPORT PORTD
#define COLDDR DDRD

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane){
	uint8_t x, tmp;
	
	//alle Spaltentreiber aus
	COLPORT &= ~0x7F;
	
	if (row == 0){
		//turn on first row
		SHIFTPORT |= (1<<BIT_DAT);
		CLOCKPORT |= (1<<BIT_CLK);
		CLOCKPORT &= ~(1<<BIT_CLK);
		SHIFTPORT &= ~(1<<BIT_DAT);
	}else{
		//shift shiftregister 1 pos
		CLOCKPORT |= (1<<BIT_CLK);
		CLOCKPORT &= ~(1<<BIT_CLK);
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
	
	tmp = pixmap[plane][row][0];
	
	COLPORT |= tmp & 0x7F;
}

extern uint8_t schmuh;

void SIG_OVERFLOW0 (void) __attribute__((interrupt));
void SIG_OVERFLOW0(void)
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
	// Alle Spaltentransistoren aus.
	COLPORT |= 0x00;
	// Port für Zeilentransistoren auf Ausgang
	COLDDR |= 0xFF;
	
	
	//Signale für Schieberegister auf Ausgang
	CLOCKDDR |= (1<<BIT_CLK);
	SHIFTDDR |= (1<<BIT_DAT);
	CLOCKPORT &= ~(1<<BIT_CLK);
	
	timer0_on();
}
