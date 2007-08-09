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

char glob[] = "Global";

volatile uint32_t *p;
volatile uint8_t *p2;

void main()
{
    char test2[] = "Lokalerstr";
    char *str = test2;
    uint32_t i;
    
    for (i = 0; i < 4; i++)
        test2[i] = 'l';
    glob[0]  = 'g';
    
 	// Initialize stuff
	uart_init();
	
	test();
	
	uart_putchar('\n');    
    uart_putchar('\r');	
	i = 1;
	for (p2 = 0xb000FF00; p2 < 0xb0010000;  p2++) {
	   test_sp(p2, i++);   
	}
	
	for (p = 0xb000FF00; p < 0xb0010000;  p++) {
	   writeint(p);
	   uart_putchar(' ');
	   writeint(*p);
	   uart_putchar('\n');    
       uart_putchar('\r');	
	}
	
	uart_putchar('\n');    
    uart_putchar('\r');	
	
	for (;*str; str++) {
	   uart_putchar(*str);
	}
	
	str = glob;
	for (;*str; str++) {
	   uart_putchar(*str);
	}
	//uart_putstr(test);
	
	//writeint(get_cfg());
	//uart_putchar(' ');

	writeint(get_sp());
	uart_putchar(' ');
	//writeint(getra());
		
    uart_putchar(' ');
	//irq_enable();
	//test();
    //setra(0xB0000000);
	writeint(get_gp());
	
	uart_putchar(' ');
	//writeint(getra());

	uart_putstr("\r\nHello World !!! \r\n");
    
	while (1) { // echo test
	   uart_putchar(uart_getchar() - 1);
	}

}
