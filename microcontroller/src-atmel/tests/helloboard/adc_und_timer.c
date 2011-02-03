/* helloboard.c
This is a simple Demo for the Labor Board.
It displays various different light patterns on the LEDs,
and allows the pattern to be selected, and the speed to be controled
with the 4 keys.*/ 

#include <inttypes.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "util.h"

volatile unsigned int delay=500;
volatile uint8_t k = 0;

void foo()
{
	PORTC = 0x00;
}
// TIMER0_OVF_vect
ISR (INT1_vect)
{
	static uint8_t k = 9;
	cli();
	PORTC |= 0xFF;

	sei();
}

ISR(TIMER0_COMP_vect)
{
	OCR0 = k;
}

ISR(TIMER0_OVF_vect)
{
	ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect)
{
	static uint16_t v;
	static uint8_t k=0;
	v += ADC;
	k++;
	if (k < 4)
		return;
	
	v /= 4;
	PORTC = v >> 2;
	k = 0;
	v = 0;
}

int main (void)
{	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x01;   //1 LED on to start of Patterns

	DDRD &= ~(_BV(PD3));
	PORTD |= _BV(PD3);
	
	cli();
	MCUCR &= ~(_BV(ISC10));
	MCUCR |= _BV(ISC11);
	
	GICR |= _BV(INT1);
	GIFR |= _BV(INTF1);

	TCCR0 = _BV(CS02) | _BV(CS00);
	TIMSK |= _BV(OCIE0) | _BV(TOIE0);
	GIFR |= _BV(OCF0) | _BV(TOV0);
	OCR0 = 0;

	ADMUX = _BV(REFS0) | _BV(REFS1);
	ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1)
		| _BV(ADPS2) | _BV(ADIE);


	sei();
	while (1);

	unsigned char maxmode=10, mode=0, oldmode=0, ud=0, x, y;
	const unsigned char fade=10, fadespeed=5;	

	{
		uint8_t ud;
		
	}

	
	for ( ;; ){		//for ever
		
		wait(delay);

		if (mode!=oldmode){   // if mode has been changed
			PORTC = 0x01;    // 1 LED on to start of Patterns
			oldmode=mode;
			ud=0;
		}

		switch(mode){
		case 0:
                        //knight rider mode
			switch (ud){
				case 0: 		//shift light left
					if( (PORTC<<=1) == 0x80 ){ //until bit7 is reached
						ud = 1;
					}
					break;
				case 1:			//shift bit right
					if( (PORTC>>=1) == 0x01 ){ //until bit0 is reached
						ud = 0;
					}
					break;
			}
			break;
		case 1:
			//scroll mode
			switch (ud){
				case 0:
					PORTC|=(PORTC<<1);//shift additional ones in from left
					if(PORTC & 0x80)  //until bit7 is reached
						ud = 1;
					break;
				case 1:
					PORTC<<=1;	//shift zeros in
					if (PORTC == 0){//until all zeros
						ud = 0;
						PORTC = 0x01;//start over
					}
					break;
			
			}
			break;
		case 2:
			//flash all
			switch (ud){
				case 0:
					PORTC=0xFF;//shift additional ones in from left
					ud = 1;
					break;
				case 1:
					PORTC=0;	//shift zeros in
					ud = 0;
					break;
			
			}
			break;
		case 3:
			//flash halb-halb
			switch (ud){
				case 0:
					PORTC=0xF0;
					ud = 1;
					break;
				case 1:
					PORTC=0x0F;	
					ud = 0;
					break;
			
			}
			break;
		case 4:
			//count
			PORTC = ud;
			ud++;		
			break;
		case 5:
                        /* fade in and out */
                        switch (ud){
                                case 0:
					/* fade in */
					for(x=0;x<=fade;x++)
					{
						for(y=0;y<=fadespeed;y++)
						{
						PORTC=0xff;
						wait(x);
						PORTC=0x00;
						wait(fade-x);
						};
					};
                                        ud=1;
					PORTC=0xff;
					break;
                                case 1:
					/* fade out */
                                        for(x=fade;x>0;x--)
                                        {
                                                for(y=0;y<=fadespeed+4;y++)
                                                {
                                                PORTC=0xff;
                                                wait(x);
                                                PORTC=0x00;
                                                wait(fade-x);
                                                };

					};
                                        ud=0;
					PORTC=0x00;
					wait(delay);
                                        break;

                        }
			break;
		case 6:
                        /* fade 10% */
                        for(x=0;x<=50;x++)
                        {
			PORTC=0xff;
                        wait(1);
                        PORTC=0x00;
                        wait(9);
                        };
                        break;
		case 7:
                        /* fade 30% */
                        for(x=0;x<=50;x++)
                        {
                        PORTC=0xff;
                        wait(3);
                        PORTC=0x00;
                        wait(7);
                        };
                        break;
		case 8:
                        /* fade 50% */
                        for(x=0;x<=50;x++)
                        {
                        PORTC=0xff;
                        wait(5);
                        PORTC=0x00;
                        wait(5);
                        };
                        break;
		case 9:
                        /* fade 70% */
                        for(x=0;x<=50;x++)
                        {
                        PORTC=0xff;
                        wait(7);
                        PORTC=0x00;
                        wait(3);
                        };
                        break;
		case 10:
                        /* fade 90% */
                        for(x=0;x<=50;x++)
                        {
                        PORTC=0xff;
                        wait(9);
                        PORTC=0x00;
                        wait(1);
                        };
                        break;
                }

		switch(PINB&0x0f){
			case 0x0E:	//Button 1 pressed
				if (delay < 1500){
					delay += 15;
				}
				wait(10);
				break;
			case 0x0D:	//Button 2 pressed
				if (delay > 5){
					delay -= 5;
				}
				wait(0);
				break;
			case 0x0B:	//Button 3 pressed
				oldmode=mode;
				if(mode==0)
					mode=maxmode;
				else
					mode--;
				wait(50);
				break;
			case 0x07:	//Button 4 pressed
				oldmode=mode;
				if (++mode>maxmode)
					mode=0;
				wait(50);
				break;
		} 
	
	
	}	
}
