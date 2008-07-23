/* spi.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <avr/io.h>
#include "spi.h"

void spi_init(void){
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
