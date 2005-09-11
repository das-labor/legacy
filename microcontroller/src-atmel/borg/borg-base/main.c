
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"


unsigned char borg_mode = 0;

int main (void){
	clear_screen(0);
	borg_hw_init();
	sei();
	init_can();	

	for(;;){
		matrix();
		schachbrett(20);
		spirale(20);
		fadein();
		joern1();
		test1();
		snake();
		labor_borg();
		feuer();
		labor_borg();
//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
}
