#include "spike_hw.h"
#include "testAnim.h"
#include "displayLoop.h"
#include "fileParser.h"

int main()
{
    int i, tmp;
	
    // Initialize stuff
	uart_init();

	// Say Hello!
	uart_putstr( "** Farborg **\n" );
	//msleep( 3000 );

	initFileReader();
		
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
		PWMTABLE[i]   = tmp;
	}	
		
	for (i = 0; i < 1024; i++) {
		PIXMAP[i] = 0;
	}
	FARBBORG_EN = 0x23;
	display_loop((void *) 0);
    
    return 0;
}
