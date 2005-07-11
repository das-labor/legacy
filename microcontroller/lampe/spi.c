#include <avr/io.h>

#define PORT_SPI PORTB
#define PIN_SPI PINB
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
}


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
