#include <avr/io.h>
#include <inttypes.h>

#include <avr/interrupt.h>

#include "uart.h"
#include "config.h"

#define BOOTLOADER_SECTION    __attribute__ ((section (".bootloader")))

int bootloader(void) BOOTLOADER_SECTION __attribute__ ((naked)); 


#define muh ((uint8_t *)0x9000)

int bootloader(void){
	
	cli();
	
	asm volatile(
		"out 0x3d, %A0\n\t"
		"out 0x3e, %B0\n\t"
		::"w" (RAMEND)
	);
	
	uint8_t command, data;
	uint16_t address, size;
	
	uint8_t x;
	
	uart_init();
		
	while(1){
		command = uart_getc();
	
		switch (command){
			case 'u'://upload
				address = getword();
				size = getword();
				for(;size--;address++){
						BANK_REG = (address >>8) + 0x200;
						data = uart_getc();
						BANK_DATA[address&0xFF] = data;
				}
				break;
			case 'b'://binary download
				address = getword();
				size = getword();
				for(;size--;address++){
					data = pgm_read_byte(address);
					uart_putc(data);
				}
				break;
			case 'd':
				address = getword();
				size = getword();
				for(;size--;address++){
						uart_putc (*((uint8_t *) address));
				}
				break;
			
			
			case 'm':
				for(x = 0;x != 0xfe ;x+=2){
					muh[x] = x;
					muh[x+1] = 0xff;
				};
				
				for(x = 0;x != 0xfe ;x+=2){
					if(muh[x] != x){
						uart_putc('f');
						uart_putc(x);
					}
				};
				
				break;
				
			case 't':
				for(size=0;size<256;size++){
					uart_putc(size);
				}
				
			//case 'd'://dump
			//	address = getword();
			//	hexdump((uint8_t*)address, uart_getc());
			//	break;
			case 'j':{//jump
				void (*fp) (void);
				address = getword();
				//hexdump((uint8_t *)&address, 2);
				fp = (void (*) (void))address;
				fp();
			}
			case 'r'://ready
				uart_putc('r');
				break;
		}
	}
}
