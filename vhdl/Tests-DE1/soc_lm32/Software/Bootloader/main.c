#include "spike_hw.h"

uint32_t readint()
{
	uint32_t val;

	val = (uint8_t)uart_getchar();
	val <<= 8; val += (uint8_t)uart_getchar();
	val <<= 8; val += (uint8_t)uart_getchar();
	val <<= 8; val += (uint8_t)uart_getchar();

	return val;
}

void writeint(uint32_t val)
{
	int i, digit;

	for(i=0; i<8; i++) {
		digit = (val & 0xf0000000) >> 28;
		if (digit < 0xA) 
			uart_putchar( '0'+digit );
		else
			uart_putchar( 'A'+(digit-10) );
		val <<= 4;
	}
}

void memtest()
{
	int *p;

	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE-1); p++) {
		*p = (int)p;
	}

	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE-1); p++) {
		if (*p != (int)p) {
			uart_putchar('S');
			uart_putchar('R');
			uart_putchar('A');
			uart_putchar('M');
			uart_putchar('\r');
			uart_putchar('\n');
				
		}
	}
}


int main(int argc, char **argv)
{
	// Initialize stuff
	uart_init();
	//irq_mask();
	//irq_enable();
	
	uart_putchar('S');
	uart_putchar('p');
	uart_putchar('i');
	uart_putchar('k');
	uart_putchar('e');
	uart_putchar('\r');
	uart_putchar('\n');
	//uart_putstr("\r\n** SPIKE BOOTLOADER **\n\r");
	memtest();
	for(;;) {
		uint32_t start, size, checksum;
		char c = uart_getchar();
		char *p;

		switch (c) {
		case 'r':
			jump(0x00000000);
		case 'u':
			//uart_putstr("u:");
			uart_putchar('u');
			uart_putchar(':');
			/* read start */
			start = readint();
			writeint(start);
		    uart_putchar(':');
			/* read size */
			size  = readint();
			writeint(size); 
			uart_putchar(':');

			checksum = 0;
			for(p=(char *)start; p<(char *)(start+size); p++) {
				c = uart_getchar();
				*p = c;
				checksum += (unsigned char)c;
			}
			writeint(checksum); 
			//uart_putstr(".\r\n");
			uart_putchar('.');
			uart_putchar('\r');
			uart_putchar('\n');
			break;
		case 'g':
			uart_putchar('u');
			uart_putchar(':');
			start = readint();
			writeint(start); uart_putchar('.');
			irq_disable();
			jump(start);
			//uart_putstr("XXXX");
			uart_putchar('X');
			uart_putchar('X');
			uart_putchar('X');
			uart_putchar('X');			
			break;
		}
	}
}

