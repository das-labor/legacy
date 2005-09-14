#include <setjmp.h>
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "uart.h"
#include "borg_can.h"


jmp_buf newmode_jmpbuf;

int main (void){
	clear_screen(0);
	borg_hw_init();
	uart_init();
	sei();
	bcan_init();
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
	 	//snake3dJoystick();
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

