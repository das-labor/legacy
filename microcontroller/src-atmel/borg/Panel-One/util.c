#include <avr/io.h>
#include <setjmp.h>
extern jmp_buf newmode_jmpbuf;


#include "config.h"



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
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}

}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char random(){
	static uint32_t muh = 0xAAAA;
	unsigned char x;
	for(x=0;x<8;x++){
	muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,27) ^ BIT_S(muh,31);
	}
	
	return (unsigned char) muh;
}
