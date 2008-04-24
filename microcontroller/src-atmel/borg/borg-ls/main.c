
#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "matrix.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"
#include "prng.h"
#include "percnt2.h"

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char mode;

	percnt_init(0);
	clear_screen(0);
	srandom(percnt_get(0));
	percnt_inc(0);
	borg_hw_init();
	bcan_init();	
	sei();
	timer_init();

	mode = setjmp(newmode_jmpbuf);
	
	for(;;){
		switch(mode++) {
		case 1:
			scrolltext((unsigned char*)scrolltext_text);
			{   char a[40];  
				sprintf_P(a,PSTR("</# counter == %lu "), percnt_get(0)); 
				scrolltext((unsigned char*)a); 
			} 
			break;
		case 2:
			spirale(5);
			break;
		case 3:
			joern1();
			break;
		case 4:
			snake();
			break;
		case 5:
			schachbrett(20);
			break;
		case 6:
			feuer();
			break;
		case 7:
			matrix();
			break;
		case 8:
			random_bright(200);
			mode = 1;
			break;
		case 31:
			test_level1();
			break;
		case 32:
			test_level2();
			break;
		case 33:
			test_level3();
			break;
		case 34:
			fadein();
			break;
		case 35:
			test1();
			break;
		case 0xFF:
			off();
			break;
		default:
			break;
		}
	}
}
