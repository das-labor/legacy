/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
 
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> /* random(), malloc(), free() */
#include <string.h> /* memset */
#include <stdio.h>
#include <math.h>
#include "uart.h"

/******************************************************************************/

uint8_t getrandom(void){

	ADCSRA |= 0x40 | 0x80;        /* start conversion */
	while(ADCSRA & 0x40)   /* wait for conversation to complete */
		;
	return (uint8_t)ADC;
}

/******************************************************************************/

int main(){
	DDRC = 0xFF;
	PORTC = 0x01;
	DDRB = 0x00;
	PORTB = 0XFF;
	
	DDRA = 0x00;
	PORTA = 0x00;
	ADMUX = 0x40;  /* Vref=Avcc, ADC0 */
	ADCSRA = 0x83; /* turn ADC on, prescaler=8 */
	
	
	uint16_t stattable[256];
	
	memset(stattable, 0, 2*256);
	
	uart_init();
	uint8_t x;
	uint16_t i,j;
	uint32_t cnt=0;
start:
	uart_putstr("\r\n\r\n=================\r\n");

	for(;;){
		x=getrandom();
		cnt++;
		stattable[x]++;
		if(stattable[x]==0xffff)
			break;
	}
	
	char s[20];
	for(i=0; i<16; ++i){
		uart_putstr("\n\r\t");
	//	uart_hexdump(&(stattable[16*i]), 32);
		for(j=0; j<16; ++j){
			utoa(stattable[16*i+j], s, 10);
			uart_putstr(s);
			uart_putstr(",\t");
		}
	}
	
	uart_putstr("\n\r count = ");
	ultoa(cnt, s, 10);
	uart_putstr(s);
			
	/* calculate entropy */		
	double h=0.0, p; /* entropy */
	for(i=0; i<256;++i){
		p = (double)stattable[i]/(double)cnt;
		h += p * log(p);
	}
	h = -h;
	h *= 0.69314718055994530941723212145817656807550013436025; /* log(2) */
	
	cnt= h*1000000;
	uart_putstr("\n\r Entropy * 1000000 = ");
	ultoa(cnt, s, 10);
//	snprintf(s,20,"%f Bit", h);
	uart_putstr(s);
			
	
	uart_getc();
		/* start main again */
	goto start;	

	return 0;
}



