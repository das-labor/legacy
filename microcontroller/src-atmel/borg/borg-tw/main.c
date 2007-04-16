
#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "config.h"
#include "scrolltext.h"
// #include "programm.h"
// #include "matrix.h"
#include "borg_hw.h"
#include "pixel.h"
#include "prng.h"
// #include "persistentCounter.h"

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char mode;

	clear_screen(0);
	// srandom(percnt_get());
	// percnt_inc();
	borg_hw_init();
	sei();

	mode = setjmp(newmode_jmpbuf);
	
	for(;;){
		switch(mode++) {
		case 1:
			scrolltext(">+:bp10b0d50/#Hallo #<;bp10b0d50/#Labor");
/*			{   char a[34]; 
				sprintf_P(a,PSTR("</# counter == %lu "), percnt_get());
				scrolltext(a);
			} */
			break;
/*		case 2:
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
			break; */  
		default:
			break;
		}
	}
}
