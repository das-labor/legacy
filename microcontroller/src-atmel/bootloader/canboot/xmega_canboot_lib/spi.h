
void spi_init();
unsigned char spi_data(unsigned char c);


#ifdef XMEGA
	#define spi_release_ss() SPI_PORT.OUTSET = _BV(SPI_PIN_SS)
	#define spi_assert_ss() SPI_PORT.OUTCLR = _BV(SPI_PIN_SS)
#else
	#define spi_release_ss() SPI_PORT |= _BV(SPI_PIN_SS)
	#define spi_assert_ss() SPI_PORT &= ~_BV(SPI_PIN_SS)
#endif
