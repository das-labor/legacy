#include "spike_hw.h"

uint32_t readint() {
	uint32_t val = 0, i;
    uint8_t c;
    for (i = 0; i < 8; i++) {
        val <<= 4;
        c = uart_getchar();
        if (c <= '9')
    	   val |= (c - '0') & 0xf;
        else
    	   val |= (c - 'A' + 0xa) & 0xf;    
    }
    return val;
}

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

void memtest()
{
	volatile int *p;
	for (p=(int *)SRAM_START; p<(int *)(SRAM_START+SRAM_SIZE); p++) {
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
	volatile int *p;
	// Initialize stuff
	uart_init();
	//irq_enable();

	uart_putstr("\r\n** SPIKE BOOTLOADER **\n\r");
	memtest();
	for(;;) {
		uint32_t start, size, checksum, help;
		uart_putchar('>');
		uint8_t c = uart_getchar();

		switch (c) {
    		case 'r':  // reset
    			jump(0x00000000);
    		case 'u':  // Upload programm
      			/* read start */
    			start = readint();
    			/* read size */
    			size  = readint();
    			checksum = 0;
    			for (p = (int *) start; p < (int *) (start+size); p++) {
    				*p = readint();
    				checksum += *p;
    			}
    			writeint(checksum); 
    			uart_putstr(".\r\n");
    			break;
    		case 'g': // GO
    			start = readint();
    			jump(start);
    			uart_putstr("XXXX");		
    			break;
    		case 'v': // view memory 
    		  start = readint();
    		  size  = readint();
    		  help = 0;
    		  for (p = (int *) start; p < (int *) (size); p++) {
    				if (!(help++ & 4)) {
    				    uart_putstr("\r\n");
    				    writeint(p);
    				}
    				uart_putchar();    
    				writeint(*p);
    		  }
    		  break;
    		case 'e':
    		    while (1) {
    		      uart_putchar(uart_getchar());
    		    }
    		    break;
    		}
	}
}
