
#include <setjmp.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "config.h"
#include "borg_hw/borg_hw.h"
// #include "can/borg_can.h"
#include "random/prng.h"
#include "random/persistentCounter.h"
#include "display_loop.h"
#include "pixel.h"

#ifdef JOYSTICK_SUPPORT
	#include "joystick/joystick.h"
#endif

#ifdef CAN_SUPPORT
	#include "can/borg_can.h"
#endif

#ifdef RFM12_SUPPORT
	#include "rfm12/borg_rfm12.h"
#endif

int main (void){
	clear_screen(0);

#ifdef RANDOM_SUPPORT
	srandom32(percnt_get());
	percnt_inc();
#endif

#ifdef RFM12_SUPPORT
	borg_rfm12_init();
#endif

	borg_hw_init();

#ifdef CAN_SUPPORT
	bcan_init();
#endif

#ifdef JOYSTICK_SUPPORT
	joy_init();	
#endif

#ifdef UART_SUPPORT
	uart_init();
#endif

	sei();

	display_loop();

	return 0;
}
