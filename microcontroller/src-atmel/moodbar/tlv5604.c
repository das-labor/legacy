#include <avr/io.h>


#define PORT_DA PORTC
#define DDR_DA DDRC
#define PIN_FS PC0
#define PIN_DAT PC2
#define PIN_SCK PC4


static inline void spi_out(uint8_t data){
	uint8_t x;
	for(x=0;x<8;x++){
		if(data & 0x80){
			PORT_DA |= (1<<PIN_DAT);
		}else{
			PORT_DA &= ~(1<<PIN_DAT);
		}
		data <<= 1;
		PORT_DA &= ~(1<<PIN_SCK);
		PORT_DA |= (1<<PIN_SCK);
	}
	
}

void da_init(){
	DDR_DA |= (1<<PIN_FS) | (1<<PIN_DAT) | (1<<PIN_SCK);
	PORT_DA |= (1<<PIN_FS);
	PORT_DA |= (1<<PIN_SCK);
}

void da_set(uint8_t channel, uint16_t value){
	PORT_DA &= ~(1<<PIN_FS);
	spi_out((value>>6) | (channel<<6) );
	spi_out(value<<2);
	PORT_DA |= (1<<PIN_FS);
}
