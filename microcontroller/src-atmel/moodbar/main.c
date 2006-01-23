#include <avr/interrupt.h>
#include <avr/signal.h>

#include "mood-can.h"
#include "util.h"

uint8_t bright[20][4];

int main (void)
{
	da_init();
	mcan_init();
	sei();

	while(1){
		mcan_process_messages();
	}
}
