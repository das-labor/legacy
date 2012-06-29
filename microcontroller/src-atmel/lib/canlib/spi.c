#include <avr/io.h>

#include "spi.h"
#include "../config.h"

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega168__) | defined(__AVR_ATmega8__) | defined(__AVR_ATmega88__)
	#define AVR_SS_BIT PB2
	#define AVR_MOSI_BIT PB3
	#define AVR_MISO_BIT PB4
	#define AVR_SCK_BIT PB5
#elif defined(__AVR_ATmega32__) | defined(__AVR_ATmega644__) | defined(__AVR_ATmega324__)
	#define AVR_SS_BIT PB4
	#define AVR_MOSI_BIT PB5
	#define AVR_MISO_BIT PB6
	#define AVR_SCK_BIT PB7
#elif defined(__AVR_ATmega128__) | defined(__AVR_ATmega102__) | defined(__AVR_AT90CAN128__)
	#define AVR_SS_BIT PB0
	#define AVR_MOSI_BIT PB2
	#define AVR_MISO_BIT PB3
	#define AVR_SCK_BIT PB1
#endif

#ifndef AVR_SS_BIT
	#error The AVR_SS_BIT is not defined for your AVR. Please add your AVR here.
#endif


void spi_init()
{
#ifdef XMEGA
	/* configure MOSI, SCK, lines as outputs */
	AVR_SPI_PORT.OUTSET = _BV(AVR_MOSI_BIT) | _BV(AVR_SCK_BIT) | _BV(AVR_SS_BIT); // mosi, sck, avr-ss to output
	
	XMEGA_SPI.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm ;

#else	
	/* configure MOSI, SCK, lines as outputs */
	DDRB |= _BV(AVR_MOSI_BIT) | _BV(AVR_SCK_BIT) | _BV(AVR_SS_BIT); // mosi, sck, avr-ss to output

	SPCR = _BV(MSTR) | _BV(SPE); // Master Mode,  Enable SPI
	SPSR = _BV(SPI2X); // Double speed on
#endif

}



uint8_t spi_send(uint8_t data)
{
#ifdef XMEGA
	XMEGA_SPI.DATA = data;
	while (!((XMEGA_SPI.STATUS) & SPI_IF_bm));
	return(XMEGA_SPI.DATA);
#else
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
	return SPDR;
#endif
}

