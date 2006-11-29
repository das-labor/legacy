#include "spike_hw.h"
#include "scope.h"

#define COUNT (8*4096-16)

/***************************************************************************
 * SCOPE LogicAnalyzer
 */
scope_t *scope0      = (scope_t *)0xF0100000;
uint8_t *scope0_data = (uint8_t *)0xF0110000;

void scope_init(int chan0, int chan1, int chan2, int chan3)
{
	scope0->channel0 = chan0;
	scope0->channel1 = chan1;
	scope0->channel2 = chan2;
	scope0->channel3 = chan3;
}

void scope_wait()
{
	while( ! (scope0->cr & SCOPE_SDONE) )
		halt();
}

void scope_send()
{
	int length, i;
	
	length = SCOPE_SIZE/2;  // in bytes

	// Write length
	uart_putchar( 'S' );
	uart_putchar( 4 );
	uart_putchar( (length >> 8) & 0xff );
	uart_putchar( (length >> 0) & 0xff );
	for(i=0; i<length; i++) {
		uart_putchar( scope0_data[i] );
	}
	uart_putchar( 'E' );
}

