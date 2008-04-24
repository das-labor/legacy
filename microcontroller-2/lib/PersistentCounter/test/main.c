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
#include "percnt2.h"


uint8_t EEMEM do_not_use=0x24;

#define OFFSET 1

/******************************************************************************/

void wait(int ms){
	while(ms--)
		_delay_ms(1);
}

/******************************************************************************/

void dump_counter(){
	uint8_t buffer[176];
	uint8_t i;
	eeprom_read_block(buffer, (void*)OFFSET, 176);
	uart_putstr("\r\nBuffer: ");
	for (i=0; i<176; i+=16){
		uart_putstr("\r\n\t");
		uart_hexdump(buffer+i, (i+16<=176)?16:176-i);
	}
	
}

/******************************************************************************/
int main(){
	DDRC = 0xFF;
	PORTC = 0x01;
	DDRB = 0x00;
	PORTB = 0XFF;
	
	uart_init();
	percnt_init(0);
	
	uint32_t i,c;
	
	c = percnt_get(0);
	
	for(i=0; i<1024; ++i){
			uart_putstr("\r\nc = ");
			uart_hexdump(&c, 4);
		if(c!=percnt_get(0)){
			dump_counter();
			while(1)
				;
		}
		++c;
		percnt_inc(0);
	}

	while(1)
	;
	return 0;
}



