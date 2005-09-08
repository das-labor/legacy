#ifndef SPI_H
#define SPI_H

#include <avr/boot.h>
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


//void spi_init() BOOTLOADER_SECTION;

#define spi_clear_ss() SPI_PORT |= (1<<SPI_PIN_SS)
#define spi_set_ss() SPI_PORT &= ~(1<<SPI_PIN_SS)



unsigned char spi_data(unsigned char c) BOOTLOADER_SECTION;





#endif
