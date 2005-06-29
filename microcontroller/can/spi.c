#include <avr/io.h>

#define PORT_SPI PORTB
#define DDR_SPI DDRB
#define PIN_MOSI PB5
#define PIN_MISO PB6
#define PIN_SCK PB7
#define PIN_SS PB4


void spi_init(){
	//set output SPI pins to output
	DDR_SPI |= (1<<PIN_MOSI) | (1<<PIN_SCK) | (1<<PIN_SS);

	//set Slave select high
	PORT_SPI |= (1<<PIN_SS);

	//          0             1
	//DORD   msb first     lsb first
	//CPOL   low idle      high idle
	//CPHA   rising edge  fallling edge
	
	//SPR0    SPR1
	// 0        0       ck/4
	// 0        1       ck/16
	// 1        0       ck/64
	// 1        1       ck/128
	
	//Interrupt disable, SPI enable, MSB first, master, clk low idle, data valid on rising edge, ck/128
	SPCR = (0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);

	//Double speed off
	SPSR = (0<<SPI2X);
}

unsigned char spi_data(unsigned char c){
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
	c = SPDR;
	return (c);
}
