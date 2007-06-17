#include "spike_hw.h"

uint32_t readint(uint8_t nibbles, uint8_t* checksum) {
	uint32_t val = 0, i;
    uint8_t c;
    for (i = 0; i < nibbles; i++) {
        val <<= 4;
        c = uart_getchar();
        if (c <= '9')
    	   val |= (c - '0') & 0xf;
        else
    	   val |= (c - 'A' + 0xa) & 0xf; 
    	if (i & 1)
    	   *checksum += val;      
    }
    return val;
}

void writeint(uint8_t nibbles, uint32_t val)
{
	uint32_t i, digit;

	for (i=0; i<8; i++) {
	    if (i >= 8-nibbles) {
    		digit = (val & 0xf0000000) >> 28;
    		if (digit >= 0xA) 
      			uart_putchar('A'+digit-10);
      		else
    			uart_putchar('0'+digit);
	    }
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
		*p = 0;
	}
	uart_putstr("SRAM MEMTEST OK\n\r");
}

int main(int argc, char **argv)
{
	volatile int8_t  *p;
	volatile int32_t *p32;
	uint8_t checksum;
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
    		case 'r': // reset
    			jump(0x00000000);
    			break;
    		case 'u': // Upload programm
      			checksum = 0;
      			/* read size */
    			size  = readint(2, (uint8_t *) &checksum);
    			size -= 5;
      			/* read start */
    			start = readint(8, (uint8_t *) &checksum);
    			for (p = (int8_t *) start; p < (int8_t *) (start+size); p++) {
    				*p = readint(2, (uint8_t *) &checksum);
    			}
    			writeint(2, ~checksum);
    			break;
    		case 'g': // go
    			start = readint(8, (uint8_t *) &checksum);
    			jump(start);
    			uart_putstr("XX");		
    			break;   
    		case 'v': // view memory 
    		  start = readint(8, (uint8_t *) &checksum);
    		  size  = readint(8, (uint8_t *) &checksum);
    	      help = 0;
    		  for (p32 = (int32_t *) start; p32 < (int32_t *) (size); p32++) {
    				if (!(help++ & 3)) {
    				    uart_putstr("\r\n[");
    				    writeint(8, (uint32_t) p32);
    				    uart_putchar(']');    
    				}
    				uart_putchar(' ');    
    				writeint(8, *p32);
    		  }
    		  break;
    		case 'e': // echo test
    		    while (1) {
    		      uart_putchar(uart_getchar());
    		    }
    		    break;
    		case 'E':
    		    checksum = 0;
    		    while (1) {
    		          readint(8, (uint8_t *) &checksum);
    		          writeint(2, checksum); 
    		    }
    		    break;
    		}
	}
}
