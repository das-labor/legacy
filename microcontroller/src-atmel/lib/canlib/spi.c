#include <avr/io.h>

#include "spi.h"
#include "../config.h"


#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega168__) | defined(__AVR_ATmega8__) | defined(__AVR_ATmega88__)
	#define AVR_SS_BIT PB2
	#define AVR_MOSI_BIT PB3
	#define AVR_MISO_BIT PB4	
	#define AVR_SCK_BIT PB5
#elif defined(__AVR_ATmega32__) | defined(__AVR_ATmega644__)
	#define AVR_SS_BIT PB4
	#define AVR_MOSI_BIT PB5
	#define AVR_MISO_BIT PB6	
	#define AVR_SCK_BIT PB7

#else
	#error The AVR_SS_BIT is not defined for your AVR. Please add your AVR here.
#endif


void spi_init()
{

	/* configure MOSI, SCK, lines as outputs */
	DDRB |= _BV(AVR_MOSI_BIT) | _BV(AVR_SCK_BIT) | _BV(AVR_SS_BIT); // mosi, sck, avr-ss to output

	SPCR = _BV(MSTR) | _BV(SPE); // Master Mode,  Enable SPI
	SPSR = _BV(SPI2X); // Double speed on
}



uint8_t spi_send(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
	return SPDR;
}

