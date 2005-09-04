
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"



int main (void){
	clear_screen(0);
	borg_hw_init();
	sei();
	
	for(;;){
		tetris();
		matrix();
		fadein();
		joern1();
		//test1();
		snake();
		schachbrett(20);
		spirale(20);
		labor_borg();
		feuer();
		

//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
}
