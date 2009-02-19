#include "config.h"

#include <avr/io.h>
#include <setjmp.h>

#include "joystick/joystick.h"

//this buffer is declared in main
extern jmp_buf newmode_jmpbuf;

#ifdef CAN_SUPPORT
#  include "can/borg_can.h"
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

#ifdef CAN_SUPPORT
		bcan_process_messages();
#endif

#ifdef JOYSTICK_SUPPORT
		if (waitForFire) {
			//PORTJOYGND &= ~(1<<BITJOY0);
			//PORTJOYGND &= ~(1<<BITJOY1);		
			if (JOYISFIRE) {
				longjmp(newmode_jmpbuf, 43);
			}
		}
#endif

		while(!(TIFR&0x80));	//wait for compare match flag
		TIFR=0x80;		//reset flag
	}
}
