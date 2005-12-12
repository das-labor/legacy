#include <avr/interrupt.h>
#include <avr/signal.h>

#include "mood-can.h"
#include "mood-hw.h"
#include "util.h"

uint8_t bright[20][4];

void bright_calc(uint8_t module);



void bright_init() 
{
	uint8_t i,j;

	for (i=0; i<20; i++) {
		for(j=0; j<4; j++)
			bright[i][j] = 0x88;
		bright_calc(i);
	}
}


void bright_calc(uint8_t module)
{
	uint8_t led, rbright,row,mask;
	uint8_t cycle;
	for (led=0;led<4;led++){
		// calc effective bright (val/32)^2
		rbright = (bright[module][led]/32); 
		rbright *= rbright;

		//2 Stränge pro Port
		row  = module & 0x03;
		mask = 1 << led;
//		if (module & 0x04) 
//			mask <<= 4;
		mask = 0xff;
		
		// set mask to bright_a
		for(cycle=0; cycle<64; cycle++) {
			if (cycle < rbright)
				bright_a[cycle][row] |= mask;
			else
				bright_a[cycle][row] &= ~mask;
		}
	};


/* old stuff
	
	for(cycle=0; cycle < 64; cycle++) {
		for(row=0; row < 4; row++) {
			bright_a[cycle][row] = 0;
			for(led=0; led < 4; led++) {
				// port 1 low nibble
				if (bright[row][3-led] > cycle)
					bright_a[cycle][row] |= 1 << led;

				// port 1 high nibble
				if (bright[4+row][3-led] > cycle)
					bright_a[cycle][row] |= 1 << (led+4);

				// port 2 low nibble 
				if (bright[row][3-led] > cycle)
					bright_b[cycle][row] |= 1 << led;

				// port 2 high nibble
				if (bright[4+row][3-led] > cycle)
					bright_b[cycle][row] |= 1 << (led+4);
*/

}

int main (void)
{
	bright_init();
	mood_init();
	mcan_init();
	sei();

	while(1){
		mcan_process_messages();
	}
}


