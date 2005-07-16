#include <avr/io.h>

#define PORT_SPI PORTB
#define PIN_SPI PINB
#define DDR_SPI DDRB

#define PIN_MOSI PB5
#define PIN_MISO PB6
#define PIN_SCK PB7
#define PIN_SS PB4

//config is in spi.h
#include "spi.h"

void spi_init(){
	//set output SPI pins to output
	DDR_SPI |= (1<<PIN_MOSI) | (1<<PIN_SCK) | (1<<PIN_SS);

	//set Slave select high
	PORT_SPI |= (1<<PIN_SS);
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
	unsigned char x, d;
	for(x=0;x<8;x++){
		if(c & 0x80){
			PORT_SPI |= (1<<PIN_MOSI);
		}else{
			PORT_SPI &= ~(1<<PIN_MOSI);	
		}
		PORT_SPI |= (1<<PIN_SCK);
		d<<=1;
		d |= (PIN_SPI & (1<<PIN_MISO))?1:0;
		PORT_SPI &= ~(1<<PIN_SCK);
		c<<=1;
	}
	return d;
}
#endif
