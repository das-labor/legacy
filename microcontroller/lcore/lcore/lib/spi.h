#ifndef SPI_H
#define SPI_H

#include "config.h"

/**
 * this enables the use of Hardware SPI on ATMEGA 
 *
 * #define SPI_HARDWARE 1
 * #define SPI_PORT PORTB
 * #define SPI_PIN PINB
 * #define SPI_DDR DDRB
 * 
 * #define SPI_PIN_MOSI PB5
 * #define SPI_PIN_MISO PB6
 * #define SPI_PIN_SCK PB7
 * #define SPI_PIN_SS PB4
 *
 */


void spi_init();

#ifdef SPI_HARDWARE
inline unsigned char spi_data(unsigned char c){
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
	c = SPDR;
	return (c);
}
#else
unsigned char spi_data(unsigned char c);
#endif


#endif
