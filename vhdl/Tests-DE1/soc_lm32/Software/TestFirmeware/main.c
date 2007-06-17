#include "spike_hw.h"

void writeint(uint32_t val)
{
	uint32_t i, digit;

	for(i=0; i<8; i++) {
		digit = (val & 0xf0000000) >> 28;
		if (digit >= 0xA) 
			uart_putchar('A'+digit-10);
		else
			uart_putchar('0'+digit);
		val <<= 4;
	}
}

void test2() {
    uart_putchar('2');   
}

void test() {
    uart_putchar('b');
    test2();
    uart_putchar('a');
} 

void main()
{
	// Initialize stuff
	uart_init();
	
	writeint(getsp());
	uart_putchar(' ');
	writeint(getra());
	uart_putchar(' ');
	writeint(getr0());
	
	//irq_enable();
	test();

	writeint(getsp());
	uart_putchar(' ');
	writeint(getra());
	uart_putchar(' ');
	writeint(getr0());

	uart_putstr("Hello World\r\n");
	while (1) { // echo test
	   uart_putchar(uart_getchar());
	}
}

