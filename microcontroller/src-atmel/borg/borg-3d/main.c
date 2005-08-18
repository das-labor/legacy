
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
	

	clear_screen(0);

	//setpixel3d((pixel3d){0,0,0}, 3);
	

	for(;;) {
		test2();
		test2();
		test2();
		test2();
		test1();
		snake3d();	
		
	}
}
