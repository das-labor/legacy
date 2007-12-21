#include <avr/io.h>
#include <setjmp.h>
extern jmp_buf newmode_jmpbuf;

#ifdef BORG_CAN
#  include "borg_can.h"
#endif

#ifndef F_CPU
#define F_CPU 16000000
#endif

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
	TCCR2 = 0x0D;	//CTC Mode, clk/128
	OCR2 = (F_CPU/128000);	//1000Hz 
	for(;ms>0;ms--){

#ifdef BORG_CAN
		bcan_process_messages();
#endif
		if (waitForFire) {
			//PORTJOYGND &= ~(1<<BITJOY0);
			//PORTJOYGND &= ~(1<<BITJOY1);		
		#ifndef RHEINFIRECONTROL	
			if (JOYISFIRE) {
				longjmp(newmode_jmpbuf, 43);
			}
		#else
			if (JOYISUP) {
				waitForFire=0;
				longjmp(newmode_jmpbuf, 43);
			}
		#endif
		}

		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}

}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

