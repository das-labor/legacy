#include <avr/io.h>

#include "spi.h"
#include "../config.h"

extern void spi_init()
{

	/* configure MOSI, SCK, lines as outputs */
	DDRB |= _BV(MC_MOSI) | _BV(MC_CLK); // mosi, sck output

	SPCR = _BV(MSTR) | _BV(SPE); // Master Mode,  Enable SPI
	SPSR = _BV(SPI2X); // Double speed on
}

static void spi_wait_busy(void)
{

#ifdef SPI_TIMEOUT
    uint8_t timeout = 200;

    while (!(SPSR & _BV(SPIF)) && timeout > 0)
        timeout--;

#else
    while (!(SPSR & _BV(SPIF)));
#endif
}

extern uint8_t spi_send(uint8_t data)
{
    SPDR = data;
    spi_wait_busy();

    return SPDR;
}

