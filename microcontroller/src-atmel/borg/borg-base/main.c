
#include <setjmp.h>
#include <avr/interrupt.h>

#include "config.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char mode = 0;

	clear_screen(0);
	borg_hw_init();
	bcan_init();	
	sei();

	mode = setjmp(newmode_jmpbuf);

	
	for(;;){
		switch(mode++) {
		case 0:
			off();
			break;
		case 1:
			schachbrett(20);
			break;
		case 2:
			spirale(20);
			break;
		case 3:
			fadein();
			break;
		case 4:
			joern1();
			break;
		case 5:
			test1();
			break;
		case 6:
			snake();
			break;
		case 7:
			break;
		case 8:
			feuer();
			break;
/*
		case 9:
			//matrix();
*/
			break;
		default:
			mode = 1;
		}
	}
}
