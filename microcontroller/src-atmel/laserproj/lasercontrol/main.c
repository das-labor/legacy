/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>

#include "./uart.h"   //rs232 
#include "./config.h"
#include "ICcom.h"
#include "kb.h"  //external PS/2 keyboard

#include <avr/pgmspace.h>

#include "ks0108.h" //include display header file
#include "arial8.h" //include font header file

/*
	lasercontrol
	made by siro 2011

	Pin configuration:
	PORTA		LCD DataPort
  
	PB4   free to use
	PB3   lasersafty
	PB2		// GLCD CS1
	PB1		// GLCD CS2
	PB0   free to use
	
	PC7   crystal
  PC6   crystal
  PC5		// GLCD EN
	PC4		// GLCD R/W
	PC3		// GLCD D/I
	PC2   frequency for H-motor
  PC1   I2c
  PC0   I2c

	PD0   free to use
  PD1   free to use
	PD2   keyboard CLK
			black +DATA
			red	GND
			white +5V (300mA)
			green CLK

	PD3 	free to use
	PD4 	keyboard DATA
  PD5   Power supply grounding
  PD6   power switch
  PD7   Status LED
*/

volatile unsigned char device_on = 0; //main energy off on reset
volatile unsigned char laser_on = 0;
volatile unsigned int hmotor_speed = 1;
volatile unsigned char hmotor_on = 0;

volatile unsigned int hmirror_reflections = 0;
unsigned int hmirror_reflections_buf = 0;
volatile unsigned char drawLCD = 0;
volatile unsigned int virtTimerA=0;
volatile unsigned int virtTimerA_counter = 0;

/* prototypes */

void disable_safty(void);
void enable_safty(void);
void stop_timer1(void);
void start_timer1(void);
void stop_timer2(void);
void start_timer2(void);

void init(void)
{

	//******************
	DDRA=0xFF; //all pins output
	PORTA = 0x00;	//Internal Pullups all off

	//******************
	DDRB=0; //can be ignored, nothing here
	PORTB = 0x00;	//Internal Pullups all off

	//******************
	DDRC=0xFF; //output
	PORTC = 0;//Internal Pullups all off

	//******************
	DDRD=_BV(PD6); //PD6 PIN_POWER
	PORTD =0;	//Internal Pullups all off


//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)


}

int main (void)
{
	uint8_t main_delay=0;
	init();		//init ports
	uart_init(); //init uart
		
	while((PORTD & _BV(PIN_POWER)))	 //wait for Powerbutton to be pressed
		__asm("nop");
		
	start_timer2();	//init timer2
	start_timer1();	//init timer1
	
	sei();  //global enable interrupts 

	while (1) {
		main_delay++;	 //internal counter

		if(uart_rxbuffer_notempty())		//check for byte arrived on uart_init
				icc_handle_uart();		//handle it
			
	
				if(!(main_delay%10)){
					if(!(PORTD & _BV(PIN_POWER)))	 //check if powerbutton is pressed
					{
							while(!(PORTD & _BV(PIN_POWER)))	//loop while powerbutton is pressed
									__asm("nop");
						stop_timer2();	//stop timer 2
						stop_timer1();  //stop timer 1
					}
			
				}
				if(!(main_delay%50)){	 //simple led blink every 500ms
					PORTD^=_BV(PIN_BLINKLED); //toggle PIN_BLINKLED
				}

				if(main_delay >= 100){
					main_delay=0;
				}

			_delay_ms(10);
	}


}

void start_timer2(void)
{
	TCCR2=_BV(COM20)|_BV(WGM21)|_BV(CS22)|_BV(CS21)|_BV(CS20);		//toggle on OCR match, Clear on OCR match, prescaler 1:1024
	OCR2=19;	//400 Hz
	TCNT2=0;
}

void stop_timer2(void)
{
	TCCR2=0;	//reset
	OCR2=0;	//reset
	TCNT2=0; //reset
}

void start_timer1(void)
{
	TCCR1A=_BV(COM1A1)|_BV(WGM11)|_BV(WGM10);		//set pin on 0,clear at top, Fast PWM 10 bit
	TCCR1B=_BV(WGM12)|_BV(CS11)|_BV(CS10); //1:64 prescaler
	OCR1A=0;		//of no PWM
	TCNT1=0;		//reset
}

void stop_timer1(void)
{
	TCCR1A=0;		//reset
	TCCR1B=0; //reset
	OCR1A=0;		//reset
	TCNT1=0;		//reset
}

void enable_safty(void)//disallow laser on
{
				PORTC|=_BV(PC5);
}

void disable_safty(void)		//allow laser on
{
				PORTC|=_BV(PC5);
}


