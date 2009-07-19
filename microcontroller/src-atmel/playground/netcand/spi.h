#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include "config.h"

#define noinline __attribute__((noinline))

/* prototypes */
void spi_init(void);
uint8_t noinline spi_send(uint8_t data);

#endif
