#include <avr/io.h>
#include <setjmp.h>

#include "config.h"
#include "joystick.h"

extern jmp_buf newmode_jmpbuf;

#ifdef BORG_CAN
#  include "borg_can.h"
#endif

#ifndef F_CPU
#define F_CPU 16000000
#endif

extern uint8_t borg_can_setmode;


static void check_stuff(){
	#ifdef BORG_CAN
		if(borg_can_setmode != 0){
			uint8_t m = borg_can_setmode;
			borg_can_setmode = 0;
			longjmp(newmode_jmpbuf, m);
		}
	#endif
	#ifdef BORG_JOYSTICK
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
	#endif
}


void wait(int ms){
	if(TIFR & (1<<TOV2)){
		TIFR = (1<<TOV2);
		if(ms > (256/16)){
			ms -= (256/16);
		}else{
			ms = 0;
		}
	}
	check_stuff();
	while(ms--){
		while(TCNT2 < 16);
		TCNT2 -= 16;
		if(TCNT2 == 0)
			check_stuff();
	}
	TCNT2 = 0;
	TIFR = (1<<TOV2);
}

void timer_init(){
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
	TCCR2 = 0x07;	//clk/1024
	//about 16 ticks/ms for 16MHz clock
	OCR2 = 16;
}
