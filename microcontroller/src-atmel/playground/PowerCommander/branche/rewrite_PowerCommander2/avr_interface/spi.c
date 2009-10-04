/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include "spi.h"


void spi_init(){
	/*
		set output SPI pins to output
	*/
	SPI_DDR |= _BV(SPI_PIN_MOSI) | _BV(SPI_PIN_SCK) | _BV(SPI_PIN_CS);

#ifdef SPI_HARDWARE
	/*          0             1
		DORD   msb first     lsb first
		CPOL   low idle      high idle
		CPHA   rising edge  fallling edge
	
		SPR0    SPR1
		 0        0       ck/4
		 0        1       ck/16
		 1        0       ck/64
		 1        1       ck/128
	*/
	
	/*
		Interrupt disable, SPI enable, MSB first, master, clk low idle, data valid on rising edge, ck/4
	*/
	SPCR |= _BV(SPE) | _BV(MSTR);

	/*
		Double speed on
	*/
	SPSR = _BV(SPI2X);
#endif
}

#ifndef SPI_HARDWARE
unsigned char spi_data(unsigned char c)
{
	unsigned char x, d = d;
	for (x = 0; x < 8; x++)
	{
		if (c & 0x80)
		{
			SPI_PORT |= _BV(SPI_PIN_MOSI);
		}
		else
		{
			SPI_PORT &= ~_BV(SPI_PIN_MOSI);	
		}
		SPI_PORT |= _BV(SPI_PIN_SCK);
		d <<= 1;
		if (SPI_PIN & _BV(SPI_PIN_MISO))
			{
				d |= 1;
			}
		SPI_PORT &= ~_BV(SPI_PIN_SCK);
		c <<= 1;
	}
	return d;
}
#else
unsigned char spi_data(unsigned char c)
{
	SPDR = c;
	while (!(SPSR & _BV(SPIF)));
	c = SPDR;
	return (c);
}
#endif
