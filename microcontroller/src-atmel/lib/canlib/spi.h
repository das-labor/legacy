#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>


/* prototypes */
extern void spi_init(void);
extern uint8_t spi_send(uint8_t data);

#endif
