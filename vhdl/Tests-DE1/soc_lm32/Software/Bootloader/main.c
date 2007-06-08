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
		if (digit >= 0xA) 
			uart_putchar('A'+digit-10);
		else
			uart_putchar('0'+digit);
		val <<= 4;
	}
}

void memtest()
{
	volatile int *p;
	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE); p++) {
		//writeint((int) p);
		//uart_putstr("\n\r");
		*p = (int) p;  
	}
	
	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE); p++) {
		if (*p != (int)p) {
			uart_putstr("SRAM MEMTEST ERROR\n\r");
		}
	}
	uart_putstr("SRAM MEMTEST OK\n\r");
}

int main(int argc, char **argv)
{
	char test;
	
	// Initialize stuff
	uart_init();
	//irq_enable();

	uart_putstr("\r\n** SPIKE BOOTLOADER **\n\r");
	memtest();
	for(;;) {
		uint32_t start, size, checksum, help;
		unsigned char c = uart_getchar();
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
    			for (p = (int *) start; p < (int *) (start+size); p++) {
    				help  = (c = uart_getchar()) << 24;
    				checksum += c;
    				help += (c = uart_getchar()) << 16;
                    checksum += c;
                    help += (c = uart_getchar()) << 8;
                    checksum += c;
                    help += (c = uart_getchar());
    				checksum += c;
    				*p = help;
    			}
    			writeint(checksum); 
    			uart_putstr(".\r\n");
    			break;
    		case 'g':
    			uart_putchar('g');
    			uart_putchar(':');
    			start = readint();
    			writeint(start); uart_putchar('.');
    			irq_disable();
    			jump(start);
    			uart_putstr("XXXX");		
    			break;
    		case 'e':
    		    while (1) {
    		      uart_putchar(uart_getchar());
    		    }
    		    break;
    		}
	}
}

