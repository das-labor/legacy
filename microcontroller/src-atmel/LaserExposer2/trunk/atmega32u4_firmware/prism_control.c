//prism_control.c
//used hardware
//* Timer4
#include "config.h"


//Timer4
void timer4_init()
{
	TCCR4A=_BV(COM4A1);		//COM, non-PWM Clear OC4A on Compare Match
	//TCCR4B=_BV(CS43)|_BV(CS42)|_BV(CS40);	//clk prescaller 1:4069
	OCR4A=0;
}

void prism_set_frequency(uint16_t freq)
{
	//16000000 / 8192 = 1953,125
	//freq = 1 -> OCR4A = 1953,125
	//freq = 1953,125 -> OCR4A = 0
	if((freq > 1953)||(freq < 2))
		return;
	OCR4A = (1953/freq);	
}

uint16_t prism_calibrate(uint16_t bufferlength, uint8_t delay)
{
	//set timer4 frequency: f
	//wait
	//messure deflections per second: d
	//bits per deflection to send: ((bufferlength + delay) * 8)
	//bitsrate of SPI: 1Mbit
	//adjust f that d == 1Mbit/ ((bufferlength + delay) * 8)
	//1hz = ((bufferlength + delay) * 8) / d
	uint16_t distance=0xFFFF;
	uint16_t temp;
	uint16_t freq;
	
	prism_set_frequency(1);
	prism_start();
	
	//configure Timer3
	STOP_TIMER3
	TCCR3A=0;
	TIFR3=0;
	TCNT3=0;
		
	//prepare INT2
	DISABLE_INT2
	CONFIGURE_INT2
	//free running mode
	
	for(freq=2;freq < MASQ0MB11RR_MAX_FREQ; freq++)
	{
		prism_set_frequency(freq);
		_delay_ms(5000);	//wait 5sek
		
		//clear timer3 overvlow int
		TIFR3=0;
		//count syncs on INT2
		CLEAR_HSYNC_PIN	
		while(!HSYNC_PIN)
			asm volatile ("nop");	//wait for hsync
		
		//start Timer3
		TIFR3=0;
		TCNT3=0;
		START_TIMER3_1
messure:
		CLEAR_HSYNC_PIN	
		while(!HSYNC_PIN)
			asm volatile ("nop");	//wait for hsync	
			
		//stop Timer3
		STOP_TIMER3
		if(TIFR3 & _BV(TOV3))	//timer3 overflowed ?
		{
			TIFR3=0;
			TCNT3=0;
			TCCR4B=(TCCR4B&0x07)<<1;	//incr. prescaller
			goto messure;
		}
		if(TCCR4B&0x07)	//messurement valid ?
		{
			uint32_t time;
			//time between to syncs is now in TCNT3
			//don't forget the prescaller
			if(TCCR4B&0x07 == _BV(CS30))
			{
				time=TCNT3;
			}
			elseif(TCCR4B&0x07 == _BV(CS31))
			{
				time=TCNT3<<3;
			}
			elseif(TCCR4B&0x07 == _BV(CS32))
			{
				time=TCNT4<<8;
			}
			// time/16Mhz !== ((bufferlength + delay) * 8)/1Mbits
			// time/16 !== ((bufferlength + delay)* 8)
			//distance := abs(time/16 -  ((bufferlength + delay)* 8));
			temp = abs(time/16 -  ((bufferlength + delay)* 8));
			if(temp <= distance)
			{
				distance = temp;
			}
			else
			{	//distance increased, break
				break;
			}
		}
		
	}
	prism_stop();
	return freq;
}

#define START_TIMER3_1 TCCR3B = _BV(CS30);	//prescaler to 1:1
#define START_TIMER3_8 TCCR3B = _BV(CS31);	//prescaler to 1:8
#define START_TIMER3_256 TCCR3B = _BV(CS32);	//prescaler to 1:256
#define STOP_TIMER3 TCCR3B = 0;
void prism_start(void)
{
	TCCR4B=_BV(CS43)|_BV(CS42)|_BV(CS41);	//clk prescaller 1:8192
}

void prism_stop(void)
{
	TCCR4B=0;	//timer4 off
}