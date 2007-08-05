/*#######################################################################################
Decodieren eines RC Summen Signals 
#######################################################################################*/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) 04.2007 Holger Buss
// + only for non-profit use
// + www.MikroKopter.com
// + see the File "License.txt" for further Informations
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "rc.h"


volatile int PPM_in[11];
volatile int PPM_diff[11];  // das diffenzierte Stick-Signal
volatile unsigned char NewPpmData = 1;

volatile uint8_t SenderOkay;

//############################################################################
//zum decodieren des PPM-Signals wird Timer1 mit seiner Input
//Capture Funktion benutzt:
void rc_sum_init (void)
//############################################################################
{
	TCCR1B=(1<<CS11)|(1<<CS10)|(1<<ICES1)|(1<<ICNC1); //timer1 prescale 64


// PWM
//TCCR1A = (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
//TCCR1B |= (1 << WGM12);
//OCR1B = 55;
    
    TIMSK |= _BV(TICIE1);
    return;
}

//############################################################################
//Diese Routine startet und inizialisiert den Timer für RC
SIGNAL(SIG_INPUT_CAPTURE1)
//############################################################################

{
	static unsigned int AltICR;
    signed int signal;
	static int index;		
	  	
	signal = (unsigned int) ICR1 - AltICR;		
	AltICR = ICR1;	
	
    //Syncronisationspause?
	if ((signal > 1500) && (signal < 8000))	 
    {
        index = 0;		
        NewPpmData = 0;  // Null bedeutet: Neue Daten
    } 
 	else 
    {
        if(index < 10)
        {
            if((signal > 250) && (signal < 687)) 
            { 
                signal -= 466;
                // Stabiles Signal
                if(abs(signal - PPM_in[index]) < 6)
				{ 
					if(SenderOkay < 200)
						SenderOkay += 10;
				}
                signal = (3 * (PPM_in[index]) + signal) / 4;
                //373 entspricht ca. 1.5ms also Mittelstellung
                PPM_diff[index] = signal - PPM_in[index];
                PPM_in[index] = signal;
            }
            index++;  
         //if(index == 5) PORTD |= 0x20; else PORTD &= ~0x20;  // Servosignal an J3 anlegen
         //if(index == 6) PORTD |= 0x10; else PORTD &= ~0x10;  // Servosignal an J4 anlegen
         //if(index == 7) PORTD |= 0x08; else PORTD &= ~0x08;  // Servosignal an J5 anlegen
		}
	}
}
