#include <8052.h>
#include "uart.h"

void uart_init(){
	SCON = 0x50;			/* uart in mode 1 (8 bit), REN=1 */
	T2CON &= 0xF0;			/* EXEN2=0; TR2=0; C/T2#=0; CP/RL2#=0; */
	T2CON |= 0x30;			/* RCLK = 1; TCLK=1; */
	TH2=0xFF;
	TL2=0xEC;
	RCAP2H=0xFF;
	RCAP2L=0xEC;			/* 38400 Baud at 24.576MHz */
	TR2 = 1;			/* Timer 2 run */
	TI = 1;
	RI = 0;
}

byte uart_getc(){
	byte tmp;
	while(!RI);
	tmp = SBUF;
	RI = 0;
	return(tmp);
}

void uart_putc(byte c){
	while(!TI);
	TI = 0;
	SBUF = c;
}

void uart_putstr(byte * p){
	while(*p){
		uart_putc(*p);
	}
}


