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
    //uart_putstr("SRAM MEMTEST WRITE\n\r");
	//writeint(0x12345678);
	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE); p++) {
		writeint((int) p);
		uart_putstr("\n\r");
		*p = (int) p;  
	}
	
    uart_putstr("SRAM MEMTEST READBACK\n\r");
	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE); p++) {
		if (*p != (int)p) {
			uart_putstr("SRAM MEMTEST ERROR\n\r");
		}
	}
}

int main(int argc, char **argv)
{
	char test;
	
  	sram0[100] = '5';
	test    = sram0[100];
 	if (test == '5') {
 	  irq_mask();
 	}	
	
	// Initialize stuff
	uart_init();
	//irq_enable();

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
			uart_putstr(".\r\n");
			break;
		case 'g':
			uart_putchar('u');
			uart_putchar(':');
			start = readint();
			writeint(start); uart_putchar('.');
			irq_disable();
			jump(start);
			uart_putstr("XXXX");		
			break;
		}
	}
}

