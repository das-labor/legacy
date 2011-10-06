#include <avr/io.h>

.include "rfm12_common.S"

; read spi data into REG_SPI_DATA (see rfm12_common.S) 
; for lib-internal use only! this routine overwrites the
; REG_SPI_DATA register!
; REG_SPI_DATA is also used to set the command for the
; rfm12 spi read command.
rfm12_spi_read:
	cbi PORT_SPI, BIT_SS
	out SPDR, REG_SPI_DATA

rfm12_spi_read_wait:
	sbis SPSR, SPIF
	rjmp rfm12_spi_read_wait

	in REG_SPI_DATA, SPDR
	sbi PORT_SPI, BIT_SS

	ret
