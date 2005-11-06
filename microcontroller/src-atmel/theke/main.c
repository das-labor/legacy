#include <avr/interrupt.h>
#include <avr/signal.h>

#include "mood-can.h"
#include "mood-hw.h"
#include "util.h"

uint8_t bright[20][4];

void bright_calc();


int main (void)
{
	mood_init();
	PORTC = 0x01;
	mcan_init();
	PORTC = 0x03;
	sei();
	PORTC = 0x07;

	bright[6][0] = 0;
	bright[6][1] = 0;
	bright[6][2] = 0;
	bright[6][3] = 65;
	bright_calc();



	while(1){
		mcan_process_messages();
	}
}


void bright_init() 
{
	uint8_t i,j;

	for (i=0; i<20; i++)
		for(j=0; j<4; j++)
			bright[i][j] = 0;

	bright_calc();
}

void bright_calc()
{
	uint8_t led,row;
	uint8_t cycle;
	
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


			}
		}
	}
}

