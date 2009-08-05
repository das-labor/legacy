/**
 * 
 */

#include "spike_hw.h"
#include "ff.h"

#include "diskio.h"

#include <stdio.h>

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

void writeint8(uint8_t val)
{
	uint8_t i, digit;

	for (i=0; i<2; i++) {
		digit = (val & 0xf0) >> 4;
		if (digit >= 0xA) 
			uart_putchar('A'+digit-10);
		else
			uart_putchar('0'+digit);
		val <<= 4;
	}
}


void hexdump(unsigned char * addr, uint32_t size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		writeint8(*addr++);
		uart_putstr(" ");
		if(++x == 16){
			uart_putstr("\r\n");
			x = 0;
		}
	}
}


FATFS fs;				/* File system object */
FIL fil;


int main()
{
	WORD fsize;
	FRESULT fresult;
	
	
 	// Initialize stuff
	uart_init();
	dispInit();
	
	// Initialize TIC
	isr_init();
	tic_init();
	irq_set_mask( 0x00000002 );
	irq_enable();

	uart_putstr("Bootloader>\n\r");

	dispInit();
	
	dispPrint("** Bootloader **\n");
	
	if(0 & 0x40){
		dispPrint("switch 6 on -\n");
		goto uartmode;
	}
	
	dispPrint("Initing MMC...\n");
		
	memset(&fs, 0, sizeof(FATFS)); 	/* Clear file system object */
	FatFs = &fs;					/* Assign it to the FatFs module */	

	fresult = f_open(&fil, "sys/firmware.bin", FA_READ|FA_OPEN_EXISTING);
		
	if(fresult){
		switch(fresult){
			case FR_NO_FILE:
			case FR_NO_PATH:
				dispPrint("sys/firmware.bin\nnot found.\n");
				break;
			case FR_NOT_READY:
				dispPrint("no MMC/SD card.\n");
				break;
			case FR_NO_FILESYSTEM:
				dispPrint("no FAT\nfilesystem.\n");
				break;
		}
		goto uartmode;
	}
	
	dispPrint("Loading firmware\n");
	
	while(1);
	f_read (&fil, (uint8_t*)0x200, 0xc000, &fsize);

	fp();
	
uartmode:
	
	dispPrint("Uart mode.");	
	
	while(1);
	

}
