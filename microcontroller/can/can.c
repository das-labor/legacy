#include <avr/io.h>
#include "spi.h"

//for slave select
#define PORT_SPI PORTB
#define PIN_SS PB4


//Registers
#define CANSTAT 0x0E
#define CANCTRL 0x0F


//Command Bytes
#define RESET 0xC0
#define READ 0x03
#define READ_RX_BUFFER 0x90
#define WRITE 0x02
#define LOAD_TX_BUFFER 0x40
#define RTS 0x80
#define READ_STATUS 0xA0
#define RX_STATUS 0xB0
#define BIT_MODIFY


void mcp_reset(){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(RESET);
	PORT_SPI |= (1<<PIN_SS);
}

void mcp_write(unsigned char reg, unsigned char data){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	spi_data(data);
	PORT_SPI |= (1<<PIN_SS);
}

void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		spi_data(buf[x]);
	}
	PORT_SPI |= (1<<PIN_SS);
}

unsigned char mcp_read(unsigned char reg){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char d = spi_data(0);
	PORT_SPI |= (1<<PIN_SS);
	return d;
}

void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		buf[x] = spi_data(0);
	}
	PORT_SPI |= (1<<PIN_SS);
}


void can_init(){
	mcp_reset();
}

