#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttons.h"
#include "fifo.h"
#include <setjmp.h>

#define BUF_SIZE 10

uint8_t buffer[BUF_SIZE];
fifo_t fifo;
 
extern jmp_buf newmode_jmpbuf;

void button_record(int button){
	fifo_put (&fifo, button);
}

SIGNAL(SIG_OUTPUT_COMPARE2){
	int8_t result;
	if((result = fifo_get_nowait(&fifo)) != -1){
		switch(result){
			case BUTTON_SELECT: 	longjmp(newmode_jmpbuf, 50);break;
			case BUTTON_START: 	longjmp(newmode_jmpbuf, 51);break;
			default:				break;
		}
	}
}

// Den Timer, der denn Interrupt auslöst, initialisieren
void timer2_on(){
/* 	TCCR2: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR2 = 0x0B;	// CTC Mode, clk/64
	TCNT2 = 0;	// reset timer
	OCR2  = 200;	// Compare with this value
	TIMSK = 0xB0;	// Compare match Interrupt on
}

void init_buttons(){
	// Button Input initialisieren, pull-up aus, weil pull-down in der Schaltung
	BUTTONDDR  &= ~(1<<BUTTONPIN);
	BUTTONPORT &=  ~(1<<BUTTONPIN);
	fifo_init (&fifo, buffer, BUF_SIZE);  
	timer2_on();
}
