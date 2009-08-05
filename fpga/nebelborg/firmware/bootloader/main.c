/**
 * Bootloader for soc-lm32
 */

#include "spike_hw.h"

#define PIXMAP   ( (uint32_t *) 0xF0020000)
#define PWMTABLE ( (uint32_t *) 0xF0021000)
//#define MAGIC    (*(uint8_t *) 0xF0021D54)

/* prototypes */
void writeint(uint8_t nibbles, uint32_t val);
uint32_t readint(uint8_t nibbles, uint8_t* checksum);

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
	volatile uint32_t *p;

	uart_putstr("\r\nMEMTEST...");

	for (p=(uint32_t *)RAM_START; p<(uint32_t *)(RAM_START+RAM_SIZE); p++) {
		*p = (uint32_t) p;  
	}
	
	uart_putstr("...");

	for (p=(uint32_t *)RAM_START; p<(uint32_t *)(RAM_START+RAM_SIZE); p++) {
		if (*p != (uint32_t)p) {
			uart_putstr("\r\nMEMTEST ERROR: ");
			writeint(8,(uint32_t)p);
		}
	}
	uart_putstr("OK\n\r");
}


int main(int argc, char **argv)
{
	int8_t  *p;
	int32_t *p32, i, tmp;

	// Initialize stuff
	uart_init();
	//memtest();

	PIXMAP[0]   = 0x11;
	PIXMAP[1]   = 0x12;
	PWMTABLE[0] = 0x21;
	PWMTABLE[1] = 0x22;
	//MAGIC[0]    = 0x33;
	
    // me be init of the pwm table
	for (i = 0; i < 256; i++) {
		tmp = i;
		if (tmp < 14) 
		  tmp = 14;		
		PWMTABLE[i] = tmp;
	}
	
	uart_putstr("\r\n\r\n** SPIKE BOOTLOADER **\n");
	for(;;) {
		uint32_t start, size, checksum, help;
		uart_putchar('>');
		uint8_t c = uart_getchar();

		switch (c) {
    		case 'r': // reset
    			jump(0x00000000);
    			break;
			case 'm': // Memory Test
				memtest();
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
