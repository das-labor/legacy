/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>

#include "./uart.h"   //rs232 
#include "./config.h" //not needed ?
#include "./util.h"   //sleep, not needed ?

#include "kb.h"  //external PS/2 keyboard


#include <avr/pgmspace.h>

#include "ks0108.h" //include display header file
#include "arial8.h" //include font header file

/*
	lasercontrol
	made by siro 2010

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

void init(void)
{

	//******************
	// interrupt on INT1 pin falling edge (sensor triggered)
	MCUCR |=_BV(ISC11)|_BV(ISC11); //falling edge interrupt

	GICR |= (1<<INT1); //enable INT1 interrupt

	//******************
	DDRA=0xFF; //all pins output
	PORTA = 0x00;	//Internal Pullups all off

	//******************
	DDRB=_BV(PB1)|_BV(PB2)|_BV(PB3); //pin 1,2,3 output
	PORTB = 0x00;	//Internal Pullups all off

	//******************
	DDRC=_BV(PC2)|_BV(PC3)|_BV(PC4)|_BV(PC5)|_BV(PC6)|_BV(PC7); //pin 2,3,4,5 output
	PORTC = _BV(PC6)|_BV(PC7);//Internal Pullups all off

	//******************
	DDRD=_BV(PD5)|_BV(PD7); //pin 2,5,7 output
	PORTD =_BV(PD3)|_BV(PD5);	//Pin5 on -> powersupply off, pullups keyboard connector


//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//******************
	//timer0 
	OCR0 = 0;
  TCCR0= _BV(CS00); //prescaler 1:1 
  TIMSK|= _BV(OCIE0); //Timer0 OCR Match Interrupt Enable

	//******************
	//timer1 16 bit external counter, for pwm_v
	//TCCR1B=(1<<ICNC1)|(1<<CS12)|(1<<CS11); //cancel noise,clock source T1 pin, falling edge

	//set interrupts
	//should never overflow
	//	TIMSK=_BV(OCIE0); //Timer0 OCR Match Interrupt Enable

	//******************
	//timer2 RTC
  ASSR=_BV(AS2); //AS2 On (RTC enable)
  TCCR2=_BV(CS22)|_BV(CS20); //clkT2S/128 (From prescaler)
	TCNT2=0;
  while(ASSR&0x07) //Wait until TC0 is updated 
		asm("nop");
		
	TIMSK|=_BV(TOIE2); //timer2 overflow interrupt enable
  //timer2 interupt every 1 s

	//enable power save
	PORTD&=~(_BV(PD2)|_BV(PD4)); //keyboard off
	PORTA=0;			               //LCD off
	PORTB&=~(_BV(PB1)|_BV(PB2)); //LCD off
	PORTC&=~(_BV(PC3)|_BV(PC4)|_BV(PC5)); //LCD off
  device_on = 0; // device is off
  hmotor_on = 0; //turn of hmotor (frequency = 0)
}

int main (void)
{
	init();		//init ports

	ks0108SelectFont(arial8, ks0108ReadFontData, BLACK);  //select Display Font
	ks0108SetTransparency(OPAQUE);  //set font opaque

	if(hmotor_speed > 0)
    virtTimerA=62500/hmotor_speed; //set virtTimerA
	
	char buffer[2]; //general purpose buffer
	
	sei();  //global enable interrupts 

	while (1) {
		
		//check if power-on key pressed
		if((PIND & _BV(PD6)) == 0){
	 			//change main powersupply (toggle PIN5)
			if((PORTD & _BV(PD5)) != 0)
			{
				PORTD&=~ _BV(PD5);	//set PIN5 = 0
				wait(500);
				
				ks0108Init(0);		//reinit LCD
				init_kb();        //reinit keyboard
				
				device_on = 1;
			}
			else
			{
				PORTD&=~(_BV(PD2)|_BV(PD4)); //keyboard off
				device_on = 0;
				PORTA=0;			               //LCD off
				PORTB&=~(_BV(PB1)|_BV(PB2)); //LCD off
				PORTC&=~(_BV(PC3)|_BV(PC4)|_BV(PC5)); //LCD off
				wait(1);
				PORTD|= _BV(PD5);	//set PIN5 = 1, power supply off
			}
			while((PIND & _BV(PD6)) == 0)
				wait(10);
				
		}
		if(device_on == 1){

		//test if button is pressed
		switch(getcharx()){
			case 'w':	
				if(hmotor_speed < 24000)
				  hmotor_speed++;
				break;
			case 's':	
				if(hmotor_speed > 0)
				  hmotor_speed--;
				break;
		  case 'e':
				if(hmotor_speed < 24000)
					hmotor_speed*=10;
				break;
		  case 'd':
				if(hmotor_speed > 10)
					hmotor_speed/=10;
				break;
		  case 'r':
			  hmotor_speed = 1;
			  break;
			
				}
		if(hmotor_speed > 0)
      virtTimerA=62500/(hmotor_speed*2); //set virtTimerA

		if(drawLCD == 1){
			drawLCD  = 0;
			ks0108ClearScreen();				
			ks0108GotoXY(0,0);  // Set a position

			ks0108Puts_P(PSTR("- Laserprojector 2010 by siro -"));  			// Print some text

      ks0108GotoXY(0,11);  // Set a position
	    
			ks0108Puts_P(PSTR("Hmotor speed o: "));  			// Print some text
			
			utoa(hmotor_speed,&buffer[0],10);
			ks0108Puts(&buffer[0]);

			ks0108GotoXY(0,22);  // Set a position
			
			ks0108Puts_P(PSTR("Hmotor speed i: "));  			// Print some text
			utoa(1,&buffer[0],10);
			ks0108Puts(&buffer[0]);

			ks0108GotoXY(0,33);  // Set a position
			
			ks0108Puts_P(PSTR("Hmirror reflections i: "));  			// Print some text
			utoa(hmirror_reflections_buf,&buffer[0],10);
			ks0108Puts(&buffer[0]);

		}
		
	
   }
	}

}


//called every F_CPU / 256 = 62500 hz
ISR (TIMER0_COMP_vect)
{
	virtTimerA_counter--;
  if(virtTimerA_counter == 0)
  {
		virtTimerA_counter = virtTimerA;
	  //toggle PORTC
	  if((PORTC & _BV(PC2)) > 0)
		  PORTC&=~_BV(PC2); //turn off PC2
	  else
		  PORTC|=_BV(PC2);  //turn on PC2
  }
}

//called every second
ISR (TIMER2_OVF_vect)
{
  drawLCD = 1;
	hmirror_reflections_buf = hmirror_reflections;
	hmirror_reflections = 0;
	//simple LED blink
  if((PORTD & _BV(PD7))>0)
		PORTD&=~ _BV(PD7);
  else
    PORTD|= _BV(PD7);
}

ISR (INT1_vect)
{
	hmirror_reflections ++; //increment counter
}

