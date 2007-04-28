#include <avr/io.h>
#include <setjmp.h>
extern jmp_buf newmode_jmpbuf;


#include "config.h"
#include "joystick.h"
#include <avr/eeprom.h>
#include "uart.h"

#ifndef F_CPU
#define F_CPU 8000000
#endif

char UartInTxt [SCROLLTEXT_STRING_SIZE];
uint8_t string_ready_state = 0;


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

		
		
/*		Joystickbehandlung Alt

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
*/
		//wait for compare match flag
		while(!(TIFR&0x80)){

			char input_char;
			static uint8_t Input_Char_Pointer;
			
			if (uart_getc_nb (& input_char)) {
				switch (input_char) {     
					case 'y' :   //Start of Text Code (0x02) // Break (0x00)
						Input_Char_Pointer = 0;
						string_ready_state = 0;
						break;
				
					case 0x0D :   //Cariiage Return (CR)
						
						string_ready_state = 1;
						UartInTxt[Input_Char_Pointer + 1] = 0; // Set NULL Termination
						break;
			
					default :
						UartInTxt[Input_Char_Pointer] = input_char;
						Input_Char_Pointer++;
						break;
				}
			}
	
		}
		TIFR=0x80;		//reset flag
	}
}
#define BIT_S(var,b) ((var&(1<<b))?1:0)

/* not used in favour of stdio's random 
unsigned char random(){
	static uint32_t muh = 0xAAAA;
	unsigned char x;
	for(x=0;x<8;x++){
	muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,27) ^ BIT_S(muh,31);
	}
	
	return (unsigned char) muh;
}
*/
