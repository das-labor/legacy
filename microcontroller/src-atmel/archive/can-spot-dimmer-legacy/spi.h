#ifndef SPI_H
#define SPI_H

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

unsigned char spi_data(unsigned char c);



#endif
