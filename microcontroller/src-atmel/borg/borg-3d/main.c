#include <setjmp.h>
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "uart.h"
#include "borg_can.h"
#include "scrolltext2.h"

jmp_buf newmode_jmpbuf;

int main (void){
	clear_screen(0);
	borg_hw_init();
	uart_init();
	sei();
	bcan_init();
	clear_screen(0);
	for(;;) {
        movingArrows();
        growingCubeFilled();
        scrolltext("<<--== BORG 3D WIDERSTAND ist ZWECKLOS !!!", 0, 120);
        growingCubeFilled();
        //scrolltext("So sieht fetter Text aus.", 1, 120);
	joern1();
		growingCubeFilled();
		gameOfLife(1, 400);
		growingCubeFilled();
		gameOfLife(0, 400);
		growingCubeFilled();
		growingCubeFilled();
		gameOfLife(2, 150);
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		spirale();
		//spirale2();
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
		feuer();
	}
}

