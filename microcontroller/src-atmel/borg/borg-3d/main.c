#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "uart.h"

int main (void){
	clear_screen(0);
	borg_hw_init();
	uart_init();
	sei();
	clear_screen(0);
	for(;;) {
		//serialStream();
		gameOfLife();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		waves();
        spirale();
		spirale2();
		matrix();
	 	snake3dJoystick();
		test3();
		coolAnim();
		growingCubeFilled();
		growingCubeFilled();
		coolAnim();
		test2();
		test2();
		test2();
		test2();
		snake3d();	
	}
}
