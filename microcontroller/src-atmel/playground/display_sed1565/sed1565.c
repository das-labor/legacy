#include <avr/io.h>
#include "spi.h"
#include "sed1565.h"

#define PORT_CTRL PORTC
#define DDR_CTRL DDRC
#define PIN_LED PC0
#define PIN_E PC1
#define PIN_A0 PC2
#define PIN_CS PC3
#define PIN_RST PC4


void scommand(unsigned char data){
	PORT_CTRL &= ~(1<<PIN_A0);
	spi_data(data);
	PORT_CTRL &= ~(1<<PIN_CS);
	PORT_CTRL |= (1<<PIN_E);
	PORT_CTRL &= ~(1<<PIN_E);
	PORT_CTRL |= (1<<PIN_CS);
}

void sdata(unsigned char data){
	PORT_CTRL |= (1<<PIN_A0);
	spi_data(data);
	PORT_CTRL &= ~(1<<PIN_CS);
	PORT_CTRL |= (1<<PIN_E);
	PORT_CTRL &= ~(1<<PIN_E);
	PORT_CTRL |= (1<<PIN_CS);
}

void sed1565_init(){
	PORT_CTRL |= (1<<PIN_CS)|(1<<PIN_LED);
	DDR_CTRL |= (1<<PIN_E)|(1<<PIN_A0)|(1<<PIN_CS)|(1<<PIN_RST)|(1<<PIN_LED);
	PORT_CTRL |= (1<<PIN_RST);

	scommand(0x40);
	scommand(0xA0);
	scommand(0xA3);
	scommand(0xC0);
	scommand(0x2F);
	scommand(0x20);
	scommand(0xAC);
	scommand(0x81);
	scommand(0x20);
	scommand(0xA4);
	scommand(0xAF);
	scommand(0xA6);
}

void sed1565_bitmap(PGM_P bmp){
	unsigned char x, y;
	for(y=0;y<8;y++){	
		scommand(0xB0|y);
		scommand(0x10);
		scommand(0x00);
		for(x=0;x<128;x++){
			sdata(pgm_read_byte(bmp++));
		}
	}
}
