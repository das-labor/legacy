#include <avr/io.h> 
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include "timer.h"
#include "hardware.h"
#include "lutzmacro.h"

// #include "sounder.h"


// for printf
//int uart_putchar(char c, FILE *stream);
//static FILE mystdout = FDEV_SETUP_STREAM( uart_putchar, NULL, _FDEV_SETUP_WRITE );
//int uart_putchar( char c, FILE *stream ) { SUART_putchar(c); return 0; }

u8 dcf_prot[60];

u8 idx, par;
void pushbit(u8 val){
	dcf_prot[idx++] = val;
	par ^= val;
}

void pushparity(void){
	dcf_prot[idx++] = par;
	par = 0;
}


void pushbits(u8 val, u8 len){
	u8 x, g;
	
	g = ((val/10)<<4) | (val%10);
	
	for(x=0;x<len;x++){
		pushbit(g & 0x01);
		g>>=1;
	}
}
//00000000000000000100111101100110010011000110110010

void set_dcf(u8 year, u8 month, u8 day, u8 dayow, u8 hour, u8 min){
	par = 0;
	idx = 21;
	pushbits(min, 7);
	pushparity();
	pushbits(hour, 6);
	pushparity();
	pushbits(day, 6);
	pushbits(dayow, 3);
	pushbits(month, 5);
	pushbits(year, 8);
	pushparity();
}
 

int main(void) {
	u8 c;
	unsigned long t1handle=0, t2handle=0;
	
	unsigned char dcf_count = 0;
	int i;

	// init
	DDRA = DDRA_INIT;
	DDRB = DDRB_INIT;
	DDRC = DDRC_INIT;
	DDRD = DDRD_INIT;
	PORTA = PORTA_INIT;
	PORTB = PORTB_INIT;
	PORTC = PORTC_INIT;
	PORTD = PORTD_INIT;
	timer_init();
	sei();					// Ab hier sind Interrupts aktiv
	
	//stdout = &mystdout;
	//SUART_puts("Reset!\n\r" );
	
	TCCR1A = 0x50;
	TCCR1B = 0x09;
	OCR1A = (F_CPU / (77500 * 2));
	// An OCR1A fällt das 77,5kHz Signal raus, es wird gepulst über DDRD5
	
	// 100ms = 0, 200ms = 1
	
	for(i = 0; i<60; i++) {
		dcf_prot[i] = 0;
	}
	
	dcf_prot[59] = 2;
	dcf_prot[17] = 1;
	dcf_prot[20] = 1;

	u8 minute = 34;
	set_dcf(3, 3, 3, 3, 13, minute);
	
	t1handle = timer_getvalue();
	for(;;) {
		// Jede Sekunde was tun
		dcf_count++;
		if(dcf_count>=60){
			minute++;
			if(minute >= 60) minute = 0;
			set_dcf(3, 3, 3, 3, 13, minute);
			dcf_count = 0;
		}
		
		while(timer_elapsed_time(t1handle) < TIMER_MS(1000));
		t1handle = timer_getvalue();

		switch(dcf_prot[dcf_count]) {
			case 0:
				DCF_off();
				L1_LED_red_on();
				t2handle = 0;
				while(!timer_autodelay(&t2handle, TIMER_MS(92)));
				L1_LED_red_off();
				DCF_on();			
				break;
			
			case 1:
				DCF_off();
				L1_LED_red_on();
				t2handle = 0;
				while(!timer_autodelay(&t2handle, TIMER_MS(200)));
				L1_LED_red_off();
				DCF_on();
				break;
			
			case 2:		// Sync-Pulse
			//	DCF_off();
			//	L1_LED_red_on();
				t2handle = 0;
				while(!timer_autodelay(&t2handle, TIMER_MS(100)));
				L1_LED_red_off();
				DCF_on();
				break;
			
		}

		
	}
}

