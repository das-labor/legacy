#ifndef _SPI_H
#define _SPI_H

#ifdef USE_FEMTOOS
#include "femtoos_code.h"
#else
#include <stdint.h>
#endif




/* prototypes */
extern void spi_init();
extern uint8_t spi_send(uint8_t data);

#endif
