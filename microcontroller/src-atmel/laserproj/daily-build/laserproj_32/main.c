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

#include "spi.h" //SPI hardware
#include "kb.h"  //external PS/2 keyboard


#include <avr/pgmspace.h>

#include "ks0108.h"
//#include "arial_bold_14.h"
//#include "corsiva_12.h"
#include "arial8.h"

/*
	laser beamer
	made by siro 2010
	using timer0 as PWM to generate a voltage between 0 - 5 V
	using timer1 as 16bit counter to messure drive speed

	Photo diode detecting start of line
	triggering interrupt

	time to display controlled by software

	photo diode detecting start of frame

	Pin configuration:
	PORTA		LCD DataPort
	PC3		// GLCD D/I
	PC4		// GLCD R/W
	PC5		// GLCD EN
	PC6		// GLCD CS1
	PC7		// GLCD CS2
	PB3 OC0 	PWM output
	PB0 		  power on key detection
	PB1 T1  	pwm_messurement
	PB5		    motor enable
	PD2 INT0 	keyboard CLK
			black +DATA
			red	GND
			white +5V (300mA)
			green CLK
	PD3 		INT1 disc laser sensor
	PD4 		keyboard DATA
	PD5		Power supply grounding
*/


void init(void)
{

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//******************
	//timer0 PWM mode
	//OCR0 duty cycle
	OCR0 = 0x00;
	//fast pwm anschalten und einstellen ob beim hochzaehlen an und beim runterzaehlen aus gemacht wird oder andersrum
	TCCR0= (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);

	//******************
	//timer1 16 bit external counter, for pwm_v
	TCCR1B=(1<<ICNC1)|(1<<CS12)|(1<<CS11); //cancel noise,clock source T1 pin, falling edge

	//set interrupts
	//should never overflow
	TIMSK=(1<<TOIE1); //Timer1 Overflow Interrupt Enable

	//******************
	//timer2 set to 1s
	TCCR2=(1<<CS22)|(1<<CS21)|(1<<CS20);		//clkT2S/1024 (From prescaler)
	//timer2 interupt every 1/61,035 s
	OCR2=0;

	TIMSK=(1<<TOIE2); //Timer2 Overflow Interrupt Enable

	//DDRC&=~((1<<0) | (1<<5));
	//PORTC|=(1<<0) | (1<<5);


	//******************
	// interrupt on INT0 pin falling edge (sensor triggered)
	//  MCUCR |= (1<<ISC01) | (1<<ISC00);

	//******************
	// interrupt on INT1 pin falling edge (sensor triggered)
	//  MCUCR |= (1<<ISC11) | (1<<ISC10);

	//  GICR |= (1<<INT0)|(1<<INT1);

	//******************
	//init SPI for laser

	//MOSI PIN output ?
	//SPI_DDR = (1<<SPI_PIN_MOSI);
	//DORD: LSB/MSB first ?
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);    /* Enable SPI, Master, set clock rate fck/8 */
	//SPSR = (1<<SPI2X);

	//******************
	//Port B all inputs only PortB pin3 pwm output, PIN5 motor enable
	DDRB=(1<<3)|(1<<5);
	PORTB = ~(1 << 5);	//Internal Pullups on all pins, motor enable = off

	//******************
	//Port D all inputs, except pin 5
	DDRD=(1 << 5);
	PORTD = (1 << 5);	//Powersupply enable/disable -> pullup pin5

	//******************
	//Port A all outputs
	DDRA=0xFF;
	PORTA = 0xFF;	//Internal Pullups all on

	// Initialize the LCD
	ks0108Init(0);

	// Select a font
	ks0108SelectFont(arial8, ks0108ReadFontData, BLACK);
	// Set a position
	ks0108GotoXY(0,0);
	// Print some text
	ks0108Puts_P(PSTR("- Laserbeamer 2010 by siro -"));

	//LCD off
	PORTA=0;
	PORTC=0;

}

	//rotations per minute
	volatile unsigned int rpm_v=0;
	volatile unsigned int timecounter=0; //for getting a 1 second reference
	volatile unsigned int rpm_h=0;
	volatile unsigned int target_rpm_v=0;
	volatile unsigned int target_rpm_h=0;
	volatile unsigned int fps = 1; 
	volatile unsigned char device_on = 0;

	//slew rate 
	#define rpm_interval_v 20
	#define mirrorcount_v 4

int main (void)
{
	init();		//init ports
	init_kb();	//init Keyboard routines

	char buffer[2];
	char menucounter = 0;

	//calculate target rpm (vertical DISC)
	target_rpm_v = 60 * mirrorcount_v * fps;

	sei();

	while (1) {

	if(device_on == 1){	//only if device is on
		//check target rpm vs current rpm, change PW if neccesarry
		if((target_rpm_v > rpm_v - rpm_interval_v) && OCR0 < 255)
		{
			OCR0++;
			//wait(10);
		}
	  	if((target_rpm_v < rpm_v + rpm_interval_v) && OCR0 > 0)
		{
			OCR0--;
			//wait(10);
		}

		//send rpm via RS232
		//uart_putstr(&rpm);


		menucounter++;
		wait(50);

		//test if button is pressed
		switch(getcharx()){
			case 'w':	//Button 1 pressed
				target_rpm_v+=5;
				if(target_rpm_v>10000)
					target_rpm_v=10000;
				break;
			case 's':	//Button 3 pressed
				target_rpm_v-=5;
				if(target_rpm_v<0)
					target_rpm_v=0;
				break;
				}

		if(menucounter > 1){
			menucounter=0;

			//ks0108ClearScreen();

			// Set a position
			ks0108GotoXY(0,0);
			// Print some text
			ks0108Puts_P(PSTR("- Laserbeamer 2010 by siro -"));
			// Set a position
			ks0108GotoXY(0,10);
			// Print some text
			ks0108Puts_P(PSTR("rpm_v: "));

			itoa(rpm_v,buffer,10);
			ks0108Puts(&buffer[0]);

			// Set a position
			ks0108GotoXY(40,10);
			// Print some text
			ks0108Puts_P(PSTR("target_rpm_v: "));

			itoa(target_rpm_v,buffer,10);
			ks0108Puts(&buffer[0]);

			// Set a position
			ks0108GotoXY(0,20);
			// Print some text
			ks0108Puts_P(PSTR("rpm_v: ") );

			itoa(rpm_v,buffer,10);
			ks0108Puts(&buffer[0]);

			//ks0108GotoXY(0,30);
			//ks0108Puts_P(PSTR("current test: ") );

			//ks0108SetTransparency(OPAQUE);
			//	itoa(test,buffer,10);
			//ks0108Puts(&buffer[0]);

			ks0108SetTransparency(TRANSPARENT);
			ks0108GotoXY(0,40);
			ks0108Puts_P(PSTR("OCR0: ") );

			ks0108SetTransparency(OPAQUE);
			itoa(OCR0,buffer,10);
			ks0108Puts(&buffer[0]);
			ks0108SetTransparency(TRANSPARENT);

		 }
    		}
	}

}


//update rpm every 1/61,035 s
//genauigkeit ?
ISR (TIMER2_OVF_vect)
{
  timecounter++;
	//call this once a second
  if(timecounter>(F_CPU/(0x40000))){
		rpm_v=(unsigned int)(((long)rpm_v/2 + (long)TCNT1)/2)*60;
		timecounter=0;
	}
	TCNT1 = 0;
	
	//check if power-on key pressed
		if((PINB & _BV(PB0)) == 0){
	 			//change main powersupply (toggle PIN5)
			if((PORTD & _BV(PD5)) > 0)
			{
				PORTD&=~ _BV(PD5);	//set PIN5 = 0
				device_on = 1;
				wait(100);
				ks0108Init(0);		//reinit LCD
			}
			else
			{
				PORTD|= _BV(PD5);	//set PIN5 = 1
				device_on = 0;
				PORTA=0;			//LCD off
				PORTC=0;
			}
			while((PINB & _BV(PB0)) == 0);
		}
}


