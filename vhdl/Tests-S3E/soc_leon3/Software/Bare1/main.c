#include "spike_hw.h"
#include "scope.h"




int main(int argc, char **argv)
{
	uint32_t state, c, error;
	
	uart_init();
	tic_init();
	irq_enable();


	scope_init(0, 1, 2, 8);
	scope_start();
	scope_wait();
	uart_getchar();
	scope_send();
	for(;;) halt();

	state = 0x01; error = 0;
	while(1) {
		c = (unsigned char)uart_getchar();
		if ( (state & 0xff) != c  ) {
			gpio0->oport = 0xff;
			error = 1;
			scope_wait();
			tic_disable();
			scope_send();
			uart_putchar(c);
//			uart_putchar(state & 0xff );
			for(;;) halt();
//			scope_send();
//			gpio0->oport = 0x00;
			c = state & 0xff;
		}
//			uart_putchar(c);
//			uart_putchar(state && 0xff );
		state++;
		if (scope_done()) {
			if (error) {
				scope_send();
				for(;;)
					halt();
			} else {
				scope_start();
			}
		}
//		uart_putchar(c);
	}

	for(;;)
		halt();

//	uart_putstr("Ich habe erfolgreich gebootet\r\n");
/*
	direction = 1;
	led = 0;
	for(;;) {
		if ( led == 0 ) {
			direction = 1;
			led = 1;
		} else if ( led == 7 ) {
			direction = -1;
			led = 6;
		} else {
			led += direction;
		};

		sleep(100);
		gpio0->oport = (1 << led);
	}
*/


/*
	uint32_t *p;
	while (1) {
		// Memtest -- write
		*oport = 0x01;
		for( p=(uint32_t *)RAMSTART; p<(uint32_t *)RAMEND; p++ )
			*p = (uint32_t)p;

		// Memtest -- read
		*oport = 0x02;
		for( p=(uint32_t *)RAMSTART; p<(uint32_t *)RAMEND; p++ ) {
			if (*p != (uint32_t)p)
				*oport = 0xff;
		}
	}
*/
}

