/**
 * Read RFIDs with IM283 and output to Uart with 19200 Baud.
 */

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>

#include "config.h"
#include "uart.h"

//for stdout
int put(char c, FILE * foo){
	uart_putc(c);
	return 0;
}


#define PORT_RFID PORTA
#define PIN_RFID  PINA
#define DDR_RFID  DDRA


#define BIT_RESTART PA0
#define BIT_FOUND   PA1
#define BIT_SCK     PA2
#define BIT_SDT     PA3

#define BV _BV

void rfid_init(){
	DDR_RFID |= BV(BIT_RESTART) | BV(BIT_SCK);
}


void rfid_read(uint8_t * data){
	uint8_t x;
	
	PORT_RFID |= BV(BIT_RESTART);
	_delay_us(1);
	PORT_RFID &= ~BV(BIT_RESTART);
	_delay_us(1);
	
	//wait for RFID found
	while(!(PIN_RFID & BV(BIT_FOUND)));
	
	uint8_t msk = 0x80;
	uint8_t dp = 0;
	uint8_t d = 0;
	
	for(x=0;x<40;x++){
		if(PIN_RFID & BV(BIT_SDT)){
			d |= msk;
		}
		
		msk >>= 1;
		if(msk == 0){
			msk = 0x80;
			data[dp++] = d;
			d = 0;
		}
		
		PORT_RFID |= BV(BIT_SCK);
		_delay_us(1);
		PORT_RFID &= ~BV(BIT_SCK);
		_delay_us(1);
	}
}



int main (void)
{	
	uint8_t data[5];
	uint16_t count=0;
	
	uart_init();
	
	//setupt stdout
	fdevopen(put, 0);

	printf("RFID>\r\n");
	
	rfid_init();
	
	while(1){
		rfid_read(data);
		printf("Read %04d: %02X%02X%02X%02X%02X\r\n", count++, data[0], data[1], data[2], data[3], data[4] );
	};	
}
