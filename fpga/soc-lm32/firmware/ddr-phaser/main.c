/**
 * DDR Phase Shifter
 */

#include "spike_hw.h"

/******************************************************************************
 * Control DDR phase shift
 */
inline int ps_ready()
{
	return (gpio0->in & (1 << 16));  // return 1 when ready, 0 otherwise
}

inline void ps_inc()
{
	uint32_t tmp;

	while ( ! ps_ready() );

	tmp  = gpio0->out;
	gpio0->out = tmp | (1 << 16);
	gpio0->out = tmp;
	
	while ( ! ps_ready() );
}

inline void ps_dec()
{
	uint32_t tmp;

	while ( ! ps_ready() );

	tmp  = gpio0->out;
	gpio0->out = tmp | (1 << 17);
	gpio0->out = tmp;
	
	while ( ! ps_ready() );
}

/******************************************************************************
 * Simple memory tester 
 *
 * Returns -1 on error, 0 when OK.
 */
inline int memtest(uint32_t size)
{
	volatile uint32_t *p;
	int res = 0;

	for (p=(uint32_t *)RAM_START; p<(uint32_t *)(RAM_START+size); p++) {
		*p = (uint32_t) p;  
	}
	
	for (p=(uint32_t *)RAM_START; p<(uint32_t *)(RAM_START+size); p++) {
		if (*p != (uint32_t)p) {
			res = -1;
		}
	}

	return res;
}


void scan_phase(int dir, uint32_t size)
{
	int i;

	for (i=0; i<=255; i++) {
		if (memtest(size) == 0)
			uart_putchar('O');
		else
			uart_putchar('-');

		if (dir == 1) ps_inc(); else ps_dec();
	}
}



int main(int argc, char **argv)
{
	uint32_t scan_size = 16*1024;    // scan 16 kByte

	// Initialize stuff
	uart_init();

	uart_putstr("\r\n\r\n** SOC-LM32 DDR PAHSER **\n\n");
	uart_putstr("'O' => Memtest OK ; '-' => Memtest failed\n\n");
	uart_putstr("Press [u] for upward scan, [d] for downward\n");

	for(;;) {
		uint8_t c = uart_getchar();

		switch (c) {
			case 'u': // scan upward
				scan_phase(1, scan_size);
				break;
			case 'd': // scan downward
				scan_phase(-1, scan_size);
				break;
		}
	}
}

