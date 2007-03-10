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

	for (p=(int *)0x80000000; p<(int *)0x8000ffff; p++) {
		*p = (int)p;
	}

	for (p=(int *)0x80000000; p<(int *)0x8000ffff; p++) {
		if (*p != (int)p) {
			uart_putstr("DDR TEST FAILED\n");
		}
	}
}


int main(int argc, char **argv)
{
	// Initialize stuff
	uart_init();
	irq_enable();

	uart_putstr("\r\n** SPIKE BOOTLOADER **\n\r");
	memtest();
	for(;;) {
		uint32_t start, size, checksum;
		char c = uart_getchar();
		char *p;

		switch (c) {
		case 'r':
			jump(0x00000000);
		case 'u':
			uart_putstr("u:");
			/* read start */
			start = readint();
			writeint(start); uart_putchar(':');
			/* read size */
			size  = readint();
			writeint(size); uart_putchar(':');

			checksum = 0;
			for(p=(char *)start; p<(char *)(start+size); p++) {
				c = uart_getchar();
				*p = c;
				checksum += (unsigned char)c;
			}
			writeint(checksum); uart_putstr(".\r\n");
			break;
		case 'g':
			uart_putstr("g:");
			start = readint();
			writeint(start); uart_putchar('.');
			irq_disable();
			jump(start);
			uart_putstr("XXXX");
			break;
		}
	}
}

