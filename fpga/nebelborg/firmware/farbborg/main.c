#include "spike_hw.h"
#include "testAnim.h"

int main()
{
    int i, tmp;
    // Initialize stuff
	uart_init();

	// Say Hello!
	uart_putstr( "** Farborg **\n" );
	msleep( 3000 );

	// Initialize TIC
	isr_init();
	tic_init();
	irq_set_mask( 0x00000002 );
	irq_enable();

	
	// me be init of the pwm table
	for (i = 0; i < 256; i++) {
		tmp = i;
		if (tmp < 14) 
		  tmp = 14;		
		PWMTABLE[i] = tmp;
	}
	
	*((uint32_t *) 0xf0041d54) = 0x23;
	
	for (i = 0; i < 1024; i++) {
		PIXMAP[i] = 0x80;
	}
	
	display_loop((void *) 0);
    
    return 0;
}
