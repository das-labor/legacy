#include <avr/interrupt.h>
#include <avr/signal.h>

#include "mood-can.h"
#include "util.h"
#include "tlv5604.h"

uint8_t bright[20][4];

extern pl_param_t pl;

int main (void)
{
	da_init();
	mcan_init();
	sei();

	while(1){
		//mcan_process_messages();
		unsigned char x;
		for(x=0;x!=255;x++){
			da_set(0, bright_calc(&pl, x));
			wait(20);
		}
		for(x=0;x!=255;x++){
			da_set(0, bright_calc(&pl, 255-x));
			wait(20);
		}
	}
}
