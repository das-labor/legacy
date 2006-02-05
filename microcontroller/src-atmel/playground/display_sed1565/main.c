//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include "spi.h"
#include "sed1565.h"


#include "data.h"

int main (void)
{	
	DDRC = 0xFF; 	//Port C all outputs
	
	sed1565_init();
	sed1565_bitmap(bild);
}

