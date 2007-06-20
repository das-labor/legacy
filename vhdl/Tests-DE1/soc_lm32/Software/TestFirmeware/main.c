#include "spike_hw.h"

inline void writeint(uint32_t val)
{
	uint32_t i, digit;

	for (i=0; i<8; i++) {
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

char glob[] = "Dies ist ein globaler String ";

void main()
{
    char test[] = "Dies ist ein lokaler String ";
    char *str = test;
    
    test[0] = 'd';
    glob[0] = 'd';
    
 	// Initialize stuff
	uart_init();
	
	for (;*str; str++) {
	   uart_putchar(*str);
	}
	
	str = glob;
	for (;*str; str++) {
	   uart_putchar(*str);
	}
	//uart_putstr(test);
	
	writeint(get_cfg());
	uart_putchar(' ');

	writeint(*((uint32_t *) get_sp()+1));
	uart_putchar(' ');
	//writeint(getra());
		
    uart_putchar(' ');

	//irq_enable();
	//test();
    //setra(0xB0000000);
	//writeint(getra());
	
	uart_putchar(' ');
	//writeint(getra());

	uart_putstr("\r\nHello World !!! \r\n");
	while (1) { // echo test
	   uart_putchar(uart_getchar());
	}
}

