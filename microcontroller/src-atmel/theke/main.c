#include <avr/interrupt.h>
#include <avr/signal.h>

#include "mood-can.h"
#include "mood-hw.h"
#include "util.h"

int main (void){
	mood_init();
	mcan_init();
	sei();

	bright[0]=0;
	bright[1]=0;
	bright[2]=0;

	while(1){
		mcan_process_messages();
	}
	
}
