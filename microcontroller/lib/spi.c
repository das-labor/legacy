#include <avr/io.h>
#include "spi.h"

void spi_init(){
	//set output SPI pins to output
	SPI_DDR |= (1<<SPI_PIN_MOSI) | (1<<SPI_PIN_SCK) | (1<<SPI_PIN_SS);

#ifdef SPI_HARDWARE
	//          0             1
	//DORD   msb first     lsb first
	//CPOL   low idle      high idle
	//CPHA   rising edge  fallling edge
	
	//SPR0    SPR1
	// 0        0       ck/4
	// 0        1       ck/16
	// 1        0       ck/64
	// 1        1       ck/128
	
	//Interrupt disable, SPI enable, MSB first, master, clk low idle, data valid on rising edge, ck/4
	SPCR = (0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

	//Double speed on
	SPSR = (1<<SPI2X);
#endif
}

#ifndef SPI_HARDWARE
unsigned char spi_data(unsigned char c){
	unsigned char x, d=d;
	for(x=0;x<8;x++){
		if(c & 0x80){
			SPI_PORT |= (1<<SPI_PIN_MOSI);
		}else{
			SPI_PORT &= ~(1<<SPI_PIN_MOSI);	
		}
		SPI_PORT |= (1<<SPI_PIN_SCK);
		d<<=1;
		if(SPI_PIN & (1<<SPI_PIN_MISO)){
			d|=1;
		}
		SPI_PORT &= ~(1<<SPI_PIN_SCK);
		c<<=1;
	}
	return d;
}
#else
unsigned char spi_data(unsigned char c) {
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
	c = SPDR;
	return (c);
}
#endif
