#include "spike_hw.h"

void main()
{
	char test;
	
	// Initialize stuff
	uart_init();
	//irq_enable();

	uart_putstr("Testprogramm running !!!! s");
	for(;;) {
	}
}

