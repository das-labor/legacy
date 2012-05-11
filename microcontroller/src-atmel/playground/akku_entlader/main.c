
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
//#include "display.h"


#ifdef FAXFRONT
#include "faxfront.h"
#endif

#include "hd44780.h"
#define display_init hd44780_init
#define display_print hd44780_print
#define display_set_cursor hd44780_set_cursor
#define display_clear_line hd44780_clear_line


unsigned char DELAY1, DELAY2, DELAY3, RUN1, RUN2, RUN3, STOP1, STOP2, STOP3;

unsigned char STEP_FUZZ1;

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char timer1, timer2, timer3;		//these control the step frequency

	static unsigned char step1, step2, step3;		//state of the stepper motors
	static unsigned char ls1, ls2, ls3;
	static unsigned char bmode1, bmode2, bmode3;
	
	static unsigned char btable[]={0,60,47,41,37,34,31,28,26,24,23,21,20,19,18,17,16,15,15,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14};
	
	char reload1;
		
	
	if(RUN1){
		if( (step1%4==3) && (PIND&0x01) ){
			ls1 = 1;
			PORTC = (PORTC & 0x7F)|0x80;
		}
		
		if(!(--timer1)){
			reload1 = 1;
			timer1 = DELAY1;	//reload timer
	
			if(++step1 == 48){
				step1 = 0;
			}
			
			if( ((step1%4)==0) && ls1 ){
				ls1 = 0;
				
				if(!STEP_FUZZ1) STEP_FUZZ1 = step1;
				//PORTC = (PORTC & 0x07)|((step1<<1)&0xF8);
				PORTC&=0x7F;
				step1 = 0;
			}
			
			PORTC = (PORTC & 0xFC)| ( (step1%4) ^ (step1%4)>>1 ); //set motor to new position
			
			if(STOP1 != 0xFF){
				char tmp;
				if((char)(STOP1-step1)>=0){
					tmp = STOP1-step1;
				}else{
					tmp = (STOP1-step1)+48;
				}

				if (bmode1 == 1){
					DELAY1 = btable[tmp];
					if (DELAY1 == 0){
						RUN1 = 0;
						bmode1 = 0;
					}
				}else if (tmp  == 0){
					bmode1 = 1;
				}
				
			}
			
		}else{
			reload1 = 0;
		}
	}else{
		reload1 = 0;
	}

	if( RUN2 ){
		if( ((step2&0x03)==3) && (PIND&0x02) ){
			ls2 = 1;
			PORTC = (PORTC & 0x7F)|0x80;
		}
		if(!(--timer2)){
			if (reload1){timer2++;}else{
			timer2 = DELAY2;	//reload timer
	
			if(++step2 == 48){
				step2 = 0;
			}
			
			
			if( ((step2%4)==0) && ls2 ){
				ls2 = 0;
				step2 = 0;
				PORTC = (PORTC & 0x7F);
			}
			
			PORTC = (PORTC & 0xE7)| ( (step2%4) ^ (step2%4)>>1 )<<3; //set motor to new position
			
			if(STOP2 != 0xFF){
				char tmp;
				if((char)(STOP2-step2)>=0){
					tmp = STOP2-step2;
				}else{
					tmp = (STOP2-step2)+48;
				}
				
				if (bmode2 == 1){
					DELAY2 = btable[tmp];
					if (DELAY2 == 0){
						RUN2 = 0;
						bmode2 = 0;
					}
				}else if ( tmp == 0  ){
					bmode2 = 1;
				}
				
			}
			}
			
		}
	}
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
	TCCR0 = 0x0A;	// CTC Mode, clk/8
	TCNT0 = 0;	// reset timer
	OCR0 = 0xFF;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void timer0_off(){
	TIMSK = 0;
}


void init_Ports(){
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Pullups on on Button pins
	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x0;	//Power off
	
	DDRD = 0;	//Port D to Inputs
	PORTD = 0xFF;	//Pullups on
}

void wait(int ms){
/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
		CS22 CS21 CS20
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/32
		 1    0    0       clk/64
		 1    0    1       clk/128
		 1    1    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR2 = 0x0C;	//CTC Mode, clk/64
	OCR2 = 125;	//1000Hz
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}
}



int main (void){

	init_Ports();
	display_init();
	
	timer0_on();
	sei();

	fax_init();
	
	char y;
	for(y=0;y<11;y++){

		fax_led_on(y);
		wait(50);
	}

	for(y=0;y<11;y++){
		fax_led_off(y);
		wait(50);
	}
	
	for(;;){
	
		display_clear_line(0);
		display_print("Hallo");
		
		display_clear_line(1);
		display_print("Welt");
		
		
		wait(1500);
		
		
		wait (1000);
		
	}
}
