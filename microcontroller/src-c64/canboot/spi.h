#ifndef SPI_H
#define SPI_H

#include "c64_util.h"
#include "config.h"


unsigned char spi_init();

unsigned char spi_data(unsigned char c);



#define spi_clear_ss() outb(SPI_PORT, inb(SPI_PORT) | (1<<SPI_PIN_SS))
#define spi_set_ss() outb(SPI_PORT, inb(SPI_PORT) & ~(1<<SPI_PIN_SS))


#endif
