//transmit data

#include <avr/io.h>
#include "../../rfm12.h"

uint8_t txTestData[] = {0,1,2,3,5,7,9,201};

int main()
{
	DDRC = 0xff;
	
	uint16_t i;
	
	//init rfm12
	rfm12_init();
	

	while(1)
	{
		//blow out data
		rfm12_tx(8, txTestData);

		//wait some centuries
		for(i=0; i < 65535; i++)
		{
			asm volatile("NOP");
		}
	}

	return 0;
}
