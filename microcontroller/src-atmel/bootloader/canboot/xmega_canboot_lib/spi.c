
#include <avr/io.h>
#include "../config.h"
#include "spi.h"

unsigned char spi_data(unsigned char c)  __attribute__ ((noinline)) __attribute__ ((section (".bootloader")));

unsigned char spi_data(unsigned char c)
{
	SPIC.DATA = c;
	while (!((SPIC.STATUS) & SPI_IF_bm));
	return(SPIC.DATA);
}

void spi_init()  __attribute__ ((noinline)) __attribute__ ((section (".bootloader")));

void spi_init(){
	//PORTB |= (1<<SPI_PIN_MISO); //MISO pullup for debugging
		
	//set output SPI pins to output
	#ifdef SPI_PIN_SS_AVR
		SPI_PORT.DIRSET = _BV(SPI_PIN_MOSI) | _BV(SPI_PIN_SCK) | _BV(SPI_PIN_SS) | _BV(SPI_PIN_SS_AVR);
	#else
		SPI_PORT.DIRSET = _BV(SPI_PIN_MOSI) | _BV(SPI_PIN_SCK) | _BV(SPI_PIN_SS);
	#endif


	//set Slave select high
	spi_release_ss();


	SPIC.CTRL = SPI_CLK2X_bm | SPI_ENABLE_bm | SPI_MASTER_bm ;
	

}