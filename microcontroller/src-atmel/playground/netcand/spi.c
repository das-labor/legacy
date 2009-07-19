#include <avr/io.h>
#include "spi.h"

static void spi_wait_busy(void);

void spi_init(void)
{

    /* configure MOSI, SCK, lines as outputs */
    DDR_CONFIG_OUT(SPI_MOSI);
    DDR_CONFIG_OUT(SPI_SCK);
    DDR_CONFIG_IN(SPI_MISO);


    DDR_CONFIG_OUT(SPI_CS_MCP);
    /* Enable the pullup */
    PIN_SET(SPI_CS_MCP);



    /* set all CS high (output) */
    DDR_CONFIG_OUT(SPI_CS_NET);
    /* Enable the pullup */
    PIN_SET(SPI_CS_NET);


    /* initialize spi link to rfm12 module */
    DDR_CONFIG_OUT(SPI_CS_RFM12);
    /* Enable the pullup */
    PIN_SET(SPI_CS_RFM12);

    /* enable spi, set master and clock modes (f/2) */
   // _SPCR0 = _BV(_SPE0) | _BV(_MSTR0);
   SPCR = (0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);
      	
   // _SPSR0 = _BV(_SPI2X0);
   SPSR = (1<<SPI2X);

}

static void spi_wait_busy(void)
{

#   ifdef SPI_TIMEOUT
    uint8_t timeout = 200;

    while (!(_SPSR0 & _BV(_SPIF0)) && timeout > 0)
        timeout--;

    if (timeout == 0)
        debug_printf("ERROR: spi timeout reached!\r\n");
#   else
    while (!(_SPSR0 & _BV(_SPIF0)));
#   endif

}

uint8_t noinline spi_send(uint8_t data)
{

    _SPDR0 = data;
    spi_wait_busy();

    return _SPDR0;

}

