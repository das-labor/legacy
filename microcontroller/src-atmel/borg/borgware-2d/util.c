#include "config.h"

#include <avr/io.h>
#include <setjmp.h>

#ifdef JOYSTICK_SUPPORT
#  include "joystick/joystick.h"
	unsigned char waitForFire;
#endif

#ifdef RFM12_SUPPORT
	#include "rfm12/borg_rfm12.h"
#endif

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
	//TCCR2 = 0x0D;	//CTC Mode, clk/128
	//OCR2 = (F_CPU/128000);	//1000Hz

	TCCR1B = (1<<WGM12) | 4;//CTC Mode, clk/256
	OCR1A = (F_CPU/256000);	//1000Hz


	for(;ms>0;ms--){

#ifdef CAN_SUPPORT
		bcan_process_messages();
#endif

#ifdef RFM12_SUPPORT
		borg_rfm12_tick();
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

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
		while(!(TIFR1&(1<<OCF1A)));	//wait for compare match flag
		TIFR1=(1<<OCF1A);		//reset flag
#else
		while(!(TIFR&(1<<OCF1A)));	//wait for compare match flag
		TIFR=(1<<OCF1A);		//reset flag
#endif
	}
}
