#ifndef SPI_h
#define SPI_h


//this enables the use of Hardware SPI on ATMEGA 
//#define SPI_HARDWARE


void spi_init();

#ifdef SPI_HARDWARE
inline unsigned char spi_data(unsigned char c){
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
	c = SPDR;
	return (c);
}
#else
unsigned char spi_data(unsigned char c);
#endif


#endif
