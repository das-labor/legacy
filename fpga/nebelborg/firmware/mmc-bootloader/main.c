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

FATFS fs;				/* File system object */
FIL fil;


int main()
{
	WORD fsize;
	FRESULT fresult;
	
	
 	// Initialize stuff
	uart_init();
	
	// Initialize TIC
	isr_init();
	tic_init();
	irq_set_mask( 0x00000002 );
	irq_enable();

	uart_putstr("Bootloader\n\r");
	
	if(0 & 0x40){
		uart_putstr("switch 6 on -\n\r");
		goto uartmode;
	}
	
	uart_putstr("Initing MMC...\n\r");
		
	memset(&fs, 0, sizeof(FATFS)); 	/* Clear file system object */
	FatFs = &fs;					/* Assign it to the FatFs module */	

	fresult = f_open(&fil, "sys/firmware.bin", FA_READ|FA_OPEN_EXISTING);
		
	if(fresult){
		switch(fresult){
			case FR_NO_FILE:
			case FR_NO_PATH:
				uart_putstr("sys/firmware.bin\nnot found.\n\r");
				break;
			case FR_NOT_READY:
				uart_putstr("no MMC/SD card.\n\r");
				break;
			case FR_NO_FILESYSTEM:
				uart_putstr("no FAT\nfilesystem.\n\r");
				break;
		}
		goto uartmode;
	}
	
	uart_putstr("Loading firmware\n\r");
	
	while(1);
	f_read (&fil, (uint8_t*)0x200, 0xc000, &fsize);

	fp();
	
uartmode:
	
	uart_putstr("Uart mode.\r");	
	
	while(1);
	

}
