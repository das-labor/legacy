//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw2.h"

//An diesen Pins ist das Schieberegister
//für die Spalten angeschlossen.
#define COLPORT  PORTC
#define COLDDR   DDRC
#define BIT_DAT	4 //Daten-Eingang
#define BIT_CLK 5 //Takt-Eingang



//An diesem Port sind die Zeilentreiber (8) angeschlossen.
//Ein Null-Pegel schaltet den jeweiligen Transistor an.
#define ROWPORT PORTD
#define ROWDDR   DDRD

#define ROWPORT2 PORTC
#define ROWDDR2 DDRC
						// 8       10
unsigned char pixmap[NUM_ROWS][NUM_COLS];

#define xmappot 6	
unsigned char xmap[1<<xmappot];
//unsigned char xmap[] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,2,18,10,26,6,22,14,30,4,20,12,28,8,24,16,32};

/*
inline void rowshow(unsigned char row, unsigned char brightness){
	static uint8_t rowmask = 0xFF;
	uint8_t x;
	uint16_t linemap, tmp;
	
	//alle Zeilentreiber aus
	ROWPORT |= 0xF3;
	ROWPORT2 |= 0x0C;
	
    // Zeilenwerte berechnen
	linemap = 0; tmp = 1;
	for(x=0;x<NUM_COLS;x++) {
		if(pixmap[row][x] >= brightness) linemap |= tmp;
		tmp <<= 1;
	}
	//linemap = 2;
	
	if (row == 0){
		rowmask = 0x7F; //0111 1111
	}else{
		rowmask >>= 1;
		rowmask |= 0x80;
	}
	
	TCNT0 = 0x100-(40 + (brightness>>0));



	//Die fünf bits für das 2. Display in das
	//Schieberegister takten
	tmp = 32;
	for(x=0;x<10;x++){
		if(tmp & linemap){
			COLPORT &= ~(1<<BIT_DAT);
		}else{	
			COLPORT |= (1<<BIT_DAT);
		}
		tmp<<=1;
		if(tmp & 0x400) tmp = 1;
		
		COLPORT |= (1<<BIT_CLK);
		COLPORT &= ~(1<<BIT_CLK);
	}
	
	//nächste Zeile anschalten
	ROWPORT &= rowmask | 0x0C;
	ROWPORT2 &= rowmask | 0xF3;
}

extern uint8_t schmuh;

// ISR(SIG_OVERFLOW0)
void TIMER0_OVF_vect(void) __attribute__((interrupt));
void TIMER0_OVF_vect(void)
{
	static unsigned char bright = 1;
	static unsigned char row = 0;
	
//	id this a vector
	
	rowshow(row, xmap[bright-1]);
	//rowshow(row, bright);
	
	if(++row == NUM_ROWS){
		row = 0;
		bright+=1;
		if(bright>=(1<<xmappot)) bright = 1;
	//	if(++plane==NUMPLANE) plane=0;
	}
	
	schmuh = 1;
}
*/


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
	TCCR0 = 0x02;	// clk/?
	TCNT0 = 0xFF-20;// reset timer
	TIMSK |= (1<<TOIE0);
}


void borg_hw_init(){
	int i,j;
	// Alle Zeilentransistoren aus.
	ROWPORT |= 0xF3;
	// Port für Zeilentransistoren auf Ausgang
	ROWDDR |= 0xF3;
	
	ROWPORT2 |=0x0C;
	ROWDDR2 |=0x0C;

	//Signale für Schieberegister auf Ausgang
	COLDDR |= (1<<BIT_CLK) | (1<<BIT_DAT);
	COLPORT &= ~(1<<BIT_CLK);
	
	// xmap-Tabelle berechnen
	for(i=0;i<(1<<xmappot);i++) {
		xmap[i] = 0;
		for(j=0;j<xmappot;j++) {
			xmap[i] += (i & (1<<j))?(1<<(xmappot-j-1)):0;
		}
	}
	
	timer0_on();
}
