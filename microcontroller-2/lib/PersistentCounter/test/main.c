/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
 
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> /* random(), malloc(), free() */
#include <string.h> /* memset */
 #include <avr/eeprom.h> 
#include "uart.h"
#include "persistentCounter.h"


uint8_t EEMEM do_not_use=0x24;

#define OFFSET 1

/******************************************************************************/

void wait(int ms){
	TCCR2 = 0x0C;				/* CTC Mode, clk/64 */
	OCR2 = 125;					/* 1000Hz */
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	/* wait for compare matzch flag */
		TIFR=0x80;				/* reset flag */
	}
}

/******************************************************************************/

void dump_counter(){
	uint8_t buffer[170];
	uint8_t i;
	eeprom_read_block(buffer, (void*)OFFSET, 170);
	uart_putstr("\r\nBuffer: ");
	for (i=0; i<170; i+=16){
		uart_putstr("\r\n\t");
		uart_hexdump(buffer+i, (i+16<=170)?16:170-i);
	}
	
}

/******************************************************************************/
int main(){
	DDRC = 0xFF;
	PORTC = 0x01;
	DDRB = 0x00;
	PORTB = 0XFF;
	
	eeprom_write_word((void*)(0+OFFSET), 0xFFFF);
	eeprom_write_word((void*)(2+OFFSET), 0xFFFF);
	
	uart_init();
	percnt_init();
	
	uint32_t i;
	
	for(i=0; i<1024; ++i){
			uart_putstr("\r\ni = ");
			uart_hexdump(&i, 4);
		if(i!=percnt_get()){
		//	uart_putstr("\r\ni = ");
		//	uart_hexdump(&i, 4);
			dump_counter();
			while(1)
				;
		}
		percnt_inc();
	}

	while(1)
	;
	return 0;
}



