
#include <avr/io.h>

extern uint8_t bus_read_helper();

uint8_t bus_read(uint16_t addr){
	SPDR = addr>>8;
	while((SPSR & (1<<SPIF)) == 0);
	SPDR = (uint8_t)addr;
	while((SPSR & (1<<SPIF)) == 0);
	
	return bus_read_helper();
	
}

void spi_init(){
	//DORD = 1 -> LSB first
	SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA);


}
