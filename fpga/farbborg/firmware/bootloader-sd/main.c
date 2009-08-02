/**
 * Bootloader for soc-lm32
 */

#include <stddef.h>
#include "spike_hw.h"
#include "ff.h"

#include "diskio.h"

void* memset (void *str, int c, size_t len)
{
  register char *st = str;

  while (len-- > 0)
    *st++ = c;
  return str;
}

void *memcpy (void *destaddr, void const *srcaddr, size_t len)
{
  char *dest = destaddr;
  char const *src = srcaddr;

  while (len-- > 0)
    *dest++ = *src++;
  return destaddr;
}

int
memcmp(const void* ab1, const void* ab2, size_t n)
{
	register const unsigned char*	b1 = (const unsigned char*)ab1;
	register const unsigned char*	b2 = (const unsigned char*)ab2;
	register const unsigned char*	e = b1 + n;

	while (b1 < e)
		if (*b1++ != *b2++)
			return(*--b1 - *--b2);
	return(0);
}

// prototypes 
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

/*
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
*/

FATFS fs;				/* File system object */
FIL fil;
//DIR dir;

int main()
{
	WORD fsize;
	FRESULT fresult;
	int8_t  *p;
//	int32_t *p32, i, tmp;	
	
 	// Initialize stuff
	//uart_init();
	//dispInit();
	
	// Initialize TIC
	//isr_init();
	//tic_init();
	//irq_set_mask( 0x00000002 );
	//irq_enable();
	
	uart_putstr("init\n\r"); //?
		
	memset(&fs, 0, sizeof(FATFS)); 	/* Clear file system object */
	FatFs = &fs;	                /* Assign it to the FatFs module */	

	const char firmware[] = "sys/firmware.bin";

	//scan_files("");

	fresult = f_open(&fil, firmware,  FA_READ|FA_OPEN_EXISTING);
		
	if(fresult){
		switch(fresult){
			case FR_NO_FILE:
			case FR_NO_PATH:
				uart_putstr(firmware);
				uart_putstr(" not found.\n\r");
				break;
			case FR_NOT_READY:
				uart_putstr("no card found.\n\r");
				break;
			case FR_NO_FILESYSTEM:
				uart_putstr("no FAT-FS\n\r");
				break;
		}
		goto uartmode;
	}

	uart_putstr("loading\n\r");
	
	f_read (&fil, (uint8_t*) 0x40000000, 64*1024 - 1, &fsize);
	
	jump(0x40000000);		
uartmode: 
	uart_putstr("\r\n** SPIKE BOOTLOADER **\n");
	for(;;) {
		uint32_t start, size, checksum, help;
		uart_putchar('>');
		uint32_t c = uart_getchar();

		switch (c) {
    		case 'r': // reset
    			jump(0x00000000);
    			break;

    		case 'u': // Upload programm
      			checksum = 0;
      			// read size 
    			size  = readint(2, (uint8_t *) &checksum);
    			size -= 5;
      			// read start
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
    		}
	}

	//while (1);
}

